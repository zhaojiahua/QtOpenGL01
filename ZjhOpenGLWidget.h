#pragma once
#include "AllIncludeFiles.h"
#include "ZjhIcosahedron.h"
#include "ZjhCamera.h"
#include "ZjhLight.h"
#include "ZjhMesh.h"
#include "ui_ZjhOpenGLWidget.h"

class ZjhOpenGLWidget : public QOpenGLWidget,public QOpenGLFunctions_4_4_Core
{
	Q_OBJECT

public:
	ZjhOpenGLWidget(QWidget *parent = nullptr);
	~ZjhOpenGLWidget();

	//VAO VBO EBO
	GLuint mVAO = -1;
	GLuint mVBO = -1;
	GLuint mEBO = -1;
	GLuint mVAO_ARROW = -1;
	GLuint mVBO_ARROW = -1;
	GLuint mEBO_ARROW = -1;
	//��������							position			color				uv
	const float vertices[32] = { -0.5f,0.0f,0.0f,	1.0f,0.0f,0.0f,	-1.0,-1.0,
												-0.5f,1.0f,0.0f,	0.0f,1.0f,0.0f,	-1.0,2.0,
												0.5f,1.0f,0.0f ,	0.0f,0.0f,1.0f,	2.0,2.0,
												0.5f,0.0f,0.0f	,	1.0f,1.0f,1.0f,	2.0,-1.0};
	const int indexs[6] = { 0,2,1,		0,3,2 };

	enum ZjhShape
	{
		none,
		rectangle,
		circle,
		triangle,
		Icosahedron,
		zjhMesh
	};
	//����ӿ�
	void DrawShape(ZjhShape inshape);
	void SetWireFrame(bool wireframe);

protected:
	//����OpenGL����Դ��״̬,���ڵ�һ�ε���resizeGL��paintGL֮ǰ����һ��
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	//��ȾOpenGL����.widget��Ҫ����ʱ����
	virtual void paintGL();

	//shader program
	GLuint mShader_pro;
private:
	Ui::ZjhOpenGLWidgetClass ui;

	ZjhShape mShape = none;
	QOpenGLShaderProgram mQShader_pro;//Qt��װ�õ�shader��
	QOpenGLShaderProgram mQShader_for_Icosahedron;
	QOpenGLShaderProgram mShader_arrow;
	QTimer tempTimer;
	QOpenGLTexture* tempTex = nullptr;
	QOpenGLTexture* tempTex2 = nullptr;
	QMatrix4x4 rotMatrix;//��ת����
	QMatrix4x4 scaleMatrix;//���ž���
	QMatrix4x4 rotmatrix_forArrow;
	QMatrix4x4 rotmatrix_forArrow2;

	void SendGPUPointsData();
	//��shader�����͸�Mesh��data�;��������
	void SendGPUMeshData(ZjhMesh* inmesh);

	ZjhIcosahedron* icosahedron = nullptr;
	ZjhMesh* defaultMesh = nullptr;
	ZjhCamera* defaultCamera = nullptr;//Ĭ�����
	ZjhLight* light = nullptr;//Ĭ�ϵƹ�

	float mousePrevX = 0;
	float mousePrevY = 0;
	INT64 previewTime = 0;
	INT64 currentTime = 0;
	INT64 deltaTime = 0;

	void DrawMesh(ZjhMesh* inmesh);

public slots:
	void Change();

protected:
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void wheelEvent(QWheelEvent* e);
};
