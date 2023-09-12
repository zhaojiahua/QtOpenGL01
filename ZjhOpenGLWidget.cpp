#include "ZjhOpenGLWidget.h"

ZjhOpenGLWidget::ZjhOpenGLWidget(QWidget *parent)	: QOpenGLWidget(parent)
{
	ui.setupUi(this);
	tempTimer.start(20);
	connect(&tempTimer, &QTimer::timeout, this, &ZjhOpenGLWidget::Change);
	icosahedron =new ZjhIcosahedron();
	defaultCamera = new ZjhCamera();
}

ZjhOpenGLWidget::~ZjhOpenGLWidget()
{}

void ZjhOpenGLWidget::DrawShape(ZjhShape inshape)
{
	if (mShape != inshape) {//如果绘制内容不一样需要重新向GPU发送顶点数据,否则不需要
		mShape = inshape;
		SendGPUPointsData();
		update();//触发重绘制函数 
	}
}

void ZjhOpenGLWidget::SetWireFrame(bool wireframe)
{
	makeCurrent();
	if (wireframe)	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	doneCurrent();
	update();
}

void ZjhOpenGLWidget::initializeGL()
{
	//初始化OpenGL函数,将Qt里面的函数指针指向相应的显卡提供的函数
	initializeOpenGLFunctions();
	//创建数据缓冲和数据表述,并向GPU传送数据
	// 创建VAO VBO EBO
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);
	//绑定好数据缓冲区之后向GPU传送数据
	SendGPUPointsData();

	{
	//手动创建 编译和链接shader
	//QFile vshaderfile("./shader_sources/vShader_test.txt", this);
	//QFile fshaderfile("./shader_sources/fShader_test.txt", this);
	//vshaderfile.open(QFile::ReadOnly);
	//fshaderfile.open(QFile::ReadOnly);
	//const GLchar* vshaderSource = vshaderfile.readAll();
	//const GLchar* fshaderSource = fshaderfile.readAll();

	//GLuint vshader = glCreateShader(GL_VERTEX_SHADER);//创建vertex shader
	//glShaderSource(vshader, 1, &vshaderSource, NULL);//连接vertex shader源码
	//glCompileShader(vshader);//编译shader
	//GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);//创建fragment shader
	//glShaderSource(fshader, 1, &fshaderSource, NULL);//连接fragment shader源码
	//glCompileShader(fshader);//编译shader

	//mShader_pro = glCreateProgram();//创建shader程序
	////添加shader程序的vertex shader和fragment shader
	//glAttachShader(mShader_pro, vshader);//添加vshader
	//glAttachShader(mShader_pro, fshader);//添加fshader
	////链接shader(将shader program链接到渲染管线)
	//glLinkProgram(mShader_pro);
	////链接完成后就可删掉临时的vertex shader 和fragment shader
	//glDeleteShader(vshader);
	//glDeleteShader(fshader);
	//glGetProgramiv(mShader_pro, GL_LINK_STATUS, &success);
	}

	//Qt封装好的shaderprogram类
	GLint success;
	mQShader_pro.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/QtOpenGL01/shader_sources/vShader_test.txt");
	mQShader_for_Icosahedron.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/QtOpenGL01/shader_sources/vShader_Icosahedron.txt");
	mQShader_pro.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/QtOpenGL01/shader_sources/fShader_test.txt");
	mQShader_for_Icosahedron.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/QtOpenGL01/shader_sources/fShader_Icosahedron.txt");
	success=mQShader_pro.link();
	//捕捉链接错误信息并报告
	if (!success) {
		GLchar infoLog;
		glGetProgramInfoLog(mShader_pro, 512, NULL, &infoLog);
		qDebug() << "Shader Program Link Erro : " << infoLog;
	}
	success = mQShader_for_Icosahedron.link();
	if (!success) {
		GLchar infoLog;
		glGetProgramInfoLog(mShader_pro, 512, NULL, &infoLog);
		qDebug() << "Shader Program Link Erro : " << infoLog;
	}
	//设置绘制模式
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//填充模式绘制
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//线框模式绘制
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);//点模式绘制
	//调用GPU指令设置OpenGL状态值
	glClearColor(0.25f, 0.2f, 0.4f, 1.0f);

	previewTime = QDateTime::currentDateTime().toMSecsSinceEpoch();//GL初始化结束后开始计时
}

void ZjhOpenGLWidget::resizeGL(int w, int h)
{
}

//updata会调用paintGL函数
void ZjhOpenGLWidget::paintGL()
{
	//调用GPU指令设置OpenGL状态值
	glClear(GL_COLOR_BUFFER_BIT);//绘制之前先清屏

	//设定纹理标识
	mQShader_pro.setUniformValue("tempTex", 0);
	mQShader_pro.setUniformValue("tempTex2", 1);
	//glUseProgram(mShader_pro);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	switch (mShape)
	{
	case ZjhOpenGLWidget::none:
		break;
	case ZjhOpenGLWidget::rectangle:

		scaleMatrix.setToIdentity();//把矩阵归一化(清零所有变换)
		scaleMatrix.translate(-0.5, 0.5, 0);//矩阵运算的顺序是反的,先缩放后位移
		scaleMatrix.scale(fabs(sin(QTime::currentTime().msec())));

		rotMatrix.setToIdentity();
		rotMatrix.translate(0.5, -0.5, 0);
		rotMatrix.rotate(QTime::currentTime().msec(), 0, 0, 1);//绕Z轴不断旋转(在原值的基础上继续增加旋转量)
		mQShader_pro.setUniformValue("zjhMatrix", rotMatrix);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		mQShader_pro.setUniformValue("zjhMatrix", scaleMatrix);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		break;
	case ZjhOpenGLWidget::circle:
		break;
	case ZjhOpenGLWidget::triangle:
		break;
	case ZjhOpenGLWidget::Icosahedron:
		mQShader_for_Icosahedron.setUniformValue("VMatrix", defaultCamera->GetViewMatrix());
		mQShader_for_Icosahedron.setUniformValue("PMatrix", defaultCamera->GetPerspectiveMatrix());
		glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);
		break;
	default:
		break;
	}

	//****计算每帧之间的时间间隔deltaTime****//
	currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();//获取绝对时间戳
	deltaTime = currentTime - previewTime;
	previewTime = currentTime;
	//qDebug() << "deltaTime : " << deltaTime;
}

void ZjhOpenGLWidget::SendGPUPointsData()
{
	QMatrix4x4  mMatrix;
	mMatrix.setToIdentity();
	switch (mShape)
	{
	case ZjhOpenGLWidget::none:
		break;
	case ZjhOpenGLWidget::rectangle:
		makeCurrent();
		//绑定VAO和VBO EBO
		glBindVertexArray(mVAO);//一定要最先绑定VAO再向VBO和EBO里面传送数据,这样VAO才能记录VBO和EBO
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		//向GPU发送数据
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*32, vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*6, indexs, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		//开启VAO的属性
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//使用Qt封装的QOpenGLTexture加载纹理
		tempTex = new QOpenGLTexture(QImage(":/QtOpenGL01/assetsFromWeb/textures/boom00_tex.png").mirrored());
		tempTex2 = new QOpenGLTexture(QImage(":/QtOpenGL01/assetsFromWeb/textures/char00_tex.png").mirrored());

		tempTex->bind(0);//绑定纹理
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		tempTex->setBorderColor(QColor(255, 255, 0, 255));
		tempTex2->bind(1);//绑定第二张纹理
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		//填充完数据解绑
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindVertexArray(0);

		//绑定VAO和shader并向GPU发送绘制指令
		//glBindVertexArray(mVAO);
		mQShader_pro.bind();

		doneCurrent();
		break;
	case ZjhOpenGLWidget::circle:
		break;
	case ZjhOpenGLWidget::triangle:
		break;
	case ZjhOpenGLWidget::Icosahedron:
		makeCurrent();
		//绑定VAO和VBO EBO
		glBindVertexArray(mVAO);//一定要最先绑定VAO再向VBO和EBO里面传送数据,这样VAO才能记录VBO和EBO
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		//向GPU发送数据
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36, icosahedron->GetPointDatas(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 60, icosahedron->GetIndices(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//开启VAO的属性
		glEnableVertexAttribArray(0);
		//填充完数据解绑
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//绑定VAO和shader并向GPU发送绘制指令
		//glBindVertexArray(mVAO);
		mQShader_for_Icosahedron.bind();
		mQShader_for_Icosahedron.setUniformValue("MMatrix", mMatrix);
		mQShader_for_Icosahedron.setUniformValue("VMatrix", defaultCamera->GetViewMatrix());
		mQShader_for_Icosahedron.setUniformValue("PMatrix", defaultCamera->GetPerspectiveMatrix());
		doneCurrent();
		break;
	default:
		break;
	}
}

void ZjhOpenGLWidget::mousePressEvent(QMouseEvent* e)
{
	mousePrevX =e->x();
	mousePrevY =e->y();
}

void ZjhOpenGLWidget::mouseMoveEvent(QMouseEvent* e)
{
	if (e->buttons() & Qt::MiddleButton) {
		int dx = e->x() - mousePrevX;
		int dy = e->y() - mousePrevY;
		defaultCamera->CameraMove(dx, dy);
		mousePrevX = e->x();
		mousePrevY = e->y();
	}
	//update();

}

void ZjhOpenGLWidget::wheelEvent(QWheelEvent* e)
{

}

void ZjhOpenGLWidget::Change()
{
	makeCurrent();
	float greenV = sin(QTime::currentTime().msec()) * 0.5 + 0.5;
	mQShader_pro.setUniformValue("capColor", QVector4D(0.1, greenV, 0.2, 1.0));
	
	doneCurrent();
	update();
}

