#include "ZjhOpenGLWidget.h"

ZjhOpenGLWidget::ZjhOpenGLWidget(QWidget *parent)	: QOpenGLWidget(parent)
{
	ui.setupUi(this);
	tempTimer.start(20);
	connect(&tempTimer, &QTimer::timeout, this, &ZjhOpenGLWidget::Change);
	icosahedron =new ZjhIcosahedron();
	defaultMesh = new ZjhMesh(FileType::Z_OBJ, "E:/AAA_Working/VisualStudio2022/QtOpenGL01/assetsFromCC/coodArrow.obj");
	defaultCamera = new ZjhCamera();
	light = new ZjhLight(QVector3D(-1, -1, -1), QVector3D(10, 10, 10), QColor(200, 200, 50));
	rotmatrix_forArrow.setToIdentity();
	rotmatrix_forArrow2.setToIdentity();
	rotmatrix_forArrow.rotate(90, 0, 0, -1);
	rotmatrix_forArrow2.rotate(90, 1, 0, 0);

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

	glGenVertexArrays(1, &mVAO_ARROW);
	glGenBuffers(1, &mVBO_ARROW);
	glGenBuffers(1, &mEBO_ARROW);
	SendGPUMeshData(defaultMesh);

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
	//��ʼ��һ��shader������Ⱦ�����ͷ
	mShader_arrow.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/QtOpenGL01/shader_sources/vShader_arrow.txt");
	mShader_arrow.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/QtOpenGL01/shader_sources/fShader_arrow.txt");
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
	success = mShader_arrow.link();
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

	glEnable(GL_DEPTH_TEST);

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

		mQShader_pro.bind();
		mQShader_pro.setUniformValue("zjhMatrix", rotMatrix);

		glBindVertexArray(mVAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		mQShader_pro.setUniformValue("zjhMatrix", scaleMatrix);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		break;
	case ZjhOpenGLWidget::circle:
		break;
	case ZjhOpenGLWidget::triangle:
		break;
	case ZjhOpenGLWidget::Icosahedron:
		mQShader_for_Icosahedron.bind();
		mQShader_for_Icosahedron.setUniformValue("VMatrix", defaultCamera->GetViewMatrix());
		mQShader_for_Icosahedron.setUniformValue("PMatrix", defaultCamera->GetPerspectiveMatrix());
		mQShader_for_Icosahedron.setUniformValue("camPos", defaultCamera->GetCurrentPosition());
		mQShader_for_Icosahedron.setUniformValue("lightDir", light->mDirection);
		glBindVertexArray(mVAO);
		glDrawArrays(GL_TRIANGLES, 0, 60);
		//glDrawElements(GL_TRIANGLES, 60, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		break;
	default:
		break;
	}

	//���Ƽ�ͷ����
	glBindVertexArray(mVAO_ARROW);
	mShader_arrow.bind();
	mShader_arrow.setUniformValue("MMatrix", defaultMesh->modleMatrix);
	mShader_arrow.setUniformValue("VMatrix", defaultCamera->GetViewMatrix());
	mShader_arrow.setUniformValue("PMatrix", defaultCamera->GetPerspectiveMatrix());
	mShader_arrow.setUniformValue("camPos", defaultCamera->GetCurrentPosition());
	mShader_arrow.setUniformValue("axisCr", QVector3D(0.1, 0.5, 0.1));
	glDrawArrays(GL_TRIANGLES, 0, defaultMesh->TriangleFaceNum() * 3);
	mShader_arrow.setUniformValue("MMatrix", rotmatrix_forArrow);
	mShader_arrow.setUniformValue("axisCr", QVector3D(0.5, 0.1, 0.1));
	glDrawArrays(GL_TRIANGLES, 0, defaultMesh->TriangleFaceNum() * 3);
	mShader_arrow.setUniformValue("MMatrix", rotmatrix_forArrow2);
	mShader_arrow.setUniformValue("axisCr", QVector3D(0.1, 0.1, 0.5));
	glDrawArrays(GL_TRIANGLES, 0, defaultMesh->TriangleFaceNum() * 3);
	glBindVertexArray(0);

	//****����ÿ֮֡���ʱ����deltaTime****//
	currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();//��ȡ����ʱ���
	deltaTime = currentTime - previewTime;
	previewTime = currentTime;
	//qDebug() << "deltaTime : " << deltaTime;
}

void ZjhOpenGLWidget::SendGPUPointsData()
{
	QFile t_file;
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
		glBindVertexArray(0);

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
		
		//�󶨶���ռ�λ������
		glBindBuffer(GL_ARRAY_BUFFER, mVBO);
		//��GPU��������
		//glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18, icosahedron->GetFacesDatas(15), GL_STATIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 360, icosahedron->GetAllVerticesDatas(), GL_STATIC_DRAW);
		
		/*
				t_file.setFileName(QString("./out/teshobjs/obj_all.obj"));
		t_file.open(QIODevice::WriteOnly);
		for (int i = 0; i < 12; i++) {
			float* tempdatas = icosahedron->GetPointDatas();
			QString tempStr = QString("v %1 %2 %3\n").arg(tempdatas[3 * i]).arg(tempdatas[3 * i + 1]).arg(tempdatas[3 * i + 2]);
			t_file.write(tempStr.toStdString().c_str());
		}
		t_file.write("\n");
		for (int i = 0; i < 20; i++) {
			int* tempdatas = icosahedron->GetIndices();
			QString tempStr = QString("f %1 %2 %3\n").arg(tempdatas[3 * i] + 1).arg(tempdatas[3 * i + 1] + 1).arg(tempdatas[3 * i + 2] + 1);
			t_file.write(tempStr.toStdString().c_str());
		}
		t_file.close();
		*/

		//debug	�������ƬԪ
		/*for (int i = 0; i < 20; i++) {
			t_file.setFileName(QString("./out/teshobjs/obj_%1.obj").arg(i));
			t_file.open(QIODevice::WriteOnly);
			float* tempdatas = icosahedron->GetFacesDatas(i);
			for (int j = 0; j < 3; j++) {
				QString t_writeContent = QString("v %1 %2 %3\n").arg(tempdatas[6 * j]).arg(tempdatas[6 * j + 1]).arg(tempdatas[6 * j + 2]);
				QString t_writeContent_n = QString("vn %1 %2 %3\n").arg(tempdatas[6 * j+3]).arg(tempdatas[6 * j + 4]).arg(tempdatas[6 * j + 5]);
				t_file.write(t_writeContent.toStdString().c_str());
				t_file.write(t_writeContent_n.toStdString().c_str());
			}
			t_file.write("\nf 1 2 3");
			t_file.close();
		}*/
		
		//VAO��¼һ��
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//��������ݽ��
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//��VAO��shader����GPU���ͻ���ָ��
		//glBindVertexArray(mVAO);
		mQShader_for_Icosahedron.bind();
		mQShader_for_Icosahedron.setUniformValue("MMatrix", mMatrix);
		mQShader_for_Icosahedron.setUniformValue("VMatrix", defaultCamera->GetViewMatrix());
		mQShader_for_Icosahedron.setUniformValue("PMatrix", defaultCamera->GetPerspectiveMatrix());
		mQShader_for_Icosahedron.setUniformValue("camPos", defaultCamera->GetCurrentPosition());
		mQShader_for_Icosahedron.setUniformValue("lightDir", light->mDirection);

		doneCurrent();
		break;
	default:
		break;
	}
}

void ZjhOpenGLWidget::SendGPUMeshData(ZjhMesh* inmesh)
{
	glBindVertexArray(mVAO_ARROW);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO_ARROW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO_ARROW);
	//��GPU��������
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 18 * inmesh->TriangleFaceNum(), inmesh->VerticesDatas(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(sizeof(float) * 3));
	//����VAO������
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);
	glBindVertexArray(0);

}

void ZjhOpenGLWidget::DrawMesh(ZjhMesh* inmesh)
{

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
	else if (e->buttons() & Qt::LeftButton) {
		int dx = e->x() - mousePrevX;
		int dy = e->y() - mousePrevY;
		defaultCamera->CameraRotate(dx, dy);
		mousePrevX = e->x();
		mousePrevY = e->y();
	}
	else if (e->buttons() & Qt::RightButton) {
		int dx = e->x() - mousePrevX;
		int dy = e->y() - mousePrevY;
		defaultCamera->CameraForceOn(dx, dy);
		mousePrevX = e->x();
		mousePrevY = e->y();
	}

}

void ZjhOpenGLWidget::wheelEvent(QWheelEvent* e)
{
	defaultCamera->TrackingShot(e->delta() / 10);
}

void ZjhOpenGLWidget::Change()
{
	makeCurrent();
	float greenV = sin(QTime::currentTime().msec()) * 0.5 + 0.5;
	mQShader_pro.setUniformValue("capColor", QVector4D(0.1, greenV, 0.2, 1.0));
	
	doneCurrent();
	update();
}

