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
		Icosahedron,
		zjhMesh
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
	QOpenGLShaderProgram mShader_arrow;
	QTimer tempTimer;
	QOpenGLTexture* tempTex = nullptr;
	QOpenGLTexture* tempTex2 = nullptr;
	QMatrix4x4 rotMatrix;//旋转矩阵
	QMatrix4x4 scaleMatrix;//缩放矩阵
	QMatrix4x4 rotmatrix_forArrow;
	QMatrix4x4 rotmatrix_forArrow2;

	void SendGPUPointsData();
	//绑定shader并发送给Mesh的data和矩阵等数据
	void SendGPUMeshData(ZjhMesh* inmesh);

	ZjhIcosahedron* icosahedron = nullptr;
	ZjhMesh* defaultMesh = nullptr;
	ZjhCamera* defaultCamera = nullptr;//默认相机
	ZjhLight* light = nullptr;//默认灯光

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
