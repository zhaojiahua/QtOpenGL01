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
	if (mShape != inshape) {//����������ݲ�һ����Ҫ������GPU���Ͷ�������,������Ҫ
		mShape = inshape;
		SendGPUPointsData();
		update();//�����ػ��ƺ��� 
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
	//��ʼ��OpenGL����,��Qt����ĺ���ָ��ָ����Ӧ���Կ��ṩ�ĺ���
	initializeOpenGLFunctions();
	//�������ݻ�������ݱ���,����GPU��������
	// ����VAO VBO EBO
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);
	//�󶨺����ݻ�����֮����GPU��������
	SendGPUPointsData();

	{
	//�ֶ����� ���������shader
	//QFile vshaderfile("./shader_sources/vShader_test.txt", this);
	//QFile fshaderfile("./shader_sources/fShader_test.txt", this);
	//vshaderfile.open(QFile::ReadOnly);
	//fshaderfile.open(QFile::ReadOnly);
	//const GLchar* vshaderSource = vshaderfile.readAll();
	//const GLchar* fshaderSource = fshaderfile.readAll();

	//GLuint vshader = glCreateShader(GL_VERTEX_SHADER);//����vertex shader
	//glShaderSource(vshader, 1, &vshaderSource, NULL);//����vertex shaderԴ��
	//glCompileShader(vshader);//����shader
	//GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);//����fragment shader
	//glShaderSource(fshader, 1, &fshaderSource, NULL);//����fragment shaderԴ��
	//glCompileShader(fshader);//����shader

	//mShader_pro = glCreateProgram();//����shader����
	////���shader�����vertex shader��fragment shader
	//glAttachShader(mShader_pro, vshader);//���vshader
	//glAttachShader(mShader_pro, fshader);//���fshader
	////����shader(��shader program���ӵ���Ⱦ����)
	//glLinkProgram(mShader_pro);
	////������ɺ�Ϳ�ɾ����ʱ��vertex shader ��fragment shader
	//glDeleteShader(vshader);
	//glDeleteShader(fshader);
	//glGetProgramiv(mShader_pro, GL_LINK_STATUS, &success);
	}

	//Qt��װ�õ�shaderprogram��
	GLint success;
	mQShader_pro.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/QtOpenGL01/shader_sources/vShader_test.txt");
	mQShader_for_Icosahedron.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/QtOpenGL01/shader_sources/vShader_Icosahedron.txt");
	mQShader_pro.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/QtOpenGL01/shader_sources/fShader_test.txt");
	mQShader_for_Icosahedron.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/QtOpenGL01/shader_sources/fShader_Icosahedron.txt");
	success=mQShader_pro.link();
	//��׽���Ӵ�����Ϣ������
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
	//���û���ģʽ
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//���ģʽ����
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);//�߿�ģʽ����
	//glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);//��ģʽ����
	//����GPUָ������OpenGL״ֵ̬
	glClearColor(0.25f, 0.2f, 0.4f, 1.0f);

	previewTime = QDateTime::currentDateTime().toMSecsSinceEpoch();//GL��ʼ��������ʼ��ʱ
}

void ZjhOpenGLWidget::resizeGL(int w, int h)
{
}

//updata�����paintGL����
void ZjhOpenGLWidget::paintGL()
{
	//����GPUָ������OpenGL״ֵ̬
	glClear(GL_COLOR_BUFFER_BIT);//����֮ǰ������

	//�趨�����ʶ
	mQShader_pro.setUniformValue("tempTex", 0);
	mQShader_pro.setUniformValue("tempTex2", 1);
	//glUseProgram(mShader_pro);
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	switch (mShape)
	{
	case ZjhOpenGLWidget::none:
		break;
	case ZjhOpenGLWidget::rectangle:

		scaleMatrix.setToIdentity();//�Ѿ����һ��(�������б任)
		scaleMatrix.translate(-0.5, 0.5, 0);//���������˳���Ƿ���,�����ź�λ��
		scaleMatrix.scale(fabs(sin(QTime::currentTime().msec())));

		rotMatrix.setToIdentity();
		rotMatrix.translate(0.5, -0.5, 0);
		rotMatrix.rotate(QTime::currentTime().msec(), 0, 0, 1);//��Z�᲻����ת(��ԭֵ�Ļ����ϼ���������ת��)
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

	//****����ÿ֮֡���ʱ����deltaTime****//
	currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();//��ȡ����ʱ���
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
		//��VAO��VBO EBO
		glBindVertexArray(mVAO);//һ��Ҫ���Ȱ�VAO����VBO��EBO���洫������,����VAO���ܼ�¼VBO��EBO
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		//��GPU��������
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*32, vertices, GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*6, indexs, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		//����VAO������
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//ʹ��Qt��װ��QOpenGLTexture��������
		tempTex = new QOpenGLTexture(QImage(":/QtOpenGL01/assetsFromWeb/textures/boom00_tex.png").mirrored());
		tempTex2 = new QOpenGLTexture(QImage(":/QtOpenGL01/assetsFromWeb/textures/char00_tex.png").mirrored());

		tempTex->bind(0);//������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		tempTex->setBorderColor(QColor(255, 255, 0, 255));
		tempTex2->bind(1);//�󶨵ڶ�������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

		//��������ݽ��
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindVertexArray(0);

		//��VAO��shader����GPU���ͻ���ָ��
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
		//��VAO��VBO EBO
		glBindVertexArray(mVAO);//һ��Ҫ���Ȱ�VAO����VBO��EBO���洫������,����VAO���ܼ�¼VBO��EBO
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
		//��GPU��������
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 36, icosahedron->GetPointDatas(), GL_STATIC_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * 60, icosahedron->GetIndices(), GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		//����VAO������
		glEnableVertexAttribArray(0);
		//��������ݽ��
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//��VAO��shader����GPU���ͻ���ָ��
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

