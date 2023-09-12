#pragma once
#include "AllIncludeFiles.h"
#include "ZjhIcosahedron.h"
#include "ZjhCamera.h"
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
	//顶点数据							position			color				uv
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
		Icosahedron
	};
	//对外接口
	void DrawShape(ZjhShape inshape);
	void SetWireFrame(bool wireframe);

protected:
	//设置OpenGL的资源和状态,会在第一次调用resizeGL和paintGL之前调用一次
	virtual void initializeGL();
	virtual void resizeGL(int w, int h);
	//渲染OpenGL场景.widget需要更新时调用
	virtual void paintGL();

	//shader program
	GLuint mShader_pro;
private:
	Ui::ZjhOpenGLWidgetClass ui;

	ZjhShape mShape = none;
	QOpenGLShaderProgram mQShader_pro;//Qt封装好的shader类
	QOpenGLShaderProgram mQShader_for_Icosahedron;
	QTimer tempTimer;
	QOpenGLTexture* tempTex = nullptr;
	QOpenGLTexture* tempTex2 = nullptr;
	QMatrix4x4 rotMatrix;//旋转矩阵
	QMatrix4x4 scaleMatrix;//缩放矩阵

	ZjhIcosahedron* icosahedron = nullptr;

	void SendGPUPointsData();

	ZjhCamera* defaultCamera = nullptr;//默认相机

	float mousePrevX = 0;
	float mousePrevY = 0;
	INT64 previewTime = 0;
	INT64 currentTime = 0;
	INT64 deltaTime = 0;

public slots:
	void Change();

protected:
	void mousePressEvent(QMouseEvent* e);
	void mouseMoveEvent(QMouseEvent* e);
	void wheelEvent(QWheelEvent* e);
};
