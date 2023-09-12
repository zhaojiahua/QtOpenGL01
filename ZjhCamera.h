#pragma once
#include "AllIncludeFiles.h"

class ZjhCamera
{
public:
	//默认构造相机在(0,0,1)的位置,朝向(0,0,-1),相机的视角60度,宽高比1.77778,近裁剪面0.1,远裁剪面500
	ZjhCamera();

	//相机灵敏度
	float mSensitivity = 1.0;
	
private:
	QVector3D mPosition;//相机当前位置
	QVector3D mForward;//相机当前朝向
	QVector3D mUpward;//相机头顶方向

	float mFov;
	float mRatio;
	float mNear;
	float mFar;

	QMatrix4x4 mPMatrix;//相机透视矩阵
	QMatrix4x4 mVMatrix;//相机视图矩阵

	void UpdataPMatrix();//视角,宽高比,近裁剪面,远裁剪面这四个参数任一个发生改变都要更新透视矩阵
	void UpdataVMatrix();//位置,朝向,顶向这三个参数任一个发生变化都要更新视图矩阵

public:
	//获取相机当前视角宽度
	float Fov();
	//设置相机当前视角宽度
	void Fov(float infov);
	//获取相机当前宽高比率
	float Ratio();
	//设置相机当前宽高比率
	void Ratio(float inratio);
	//获取相机当前近裁面
	float Near();
	//设置相机当前近裁面
	void Near(float innear);
	//获取相机当前远裁面
	float Far();
	//设置相机当前远裁面
	void Far(float infar);
	
	//获取相机当前位置
	QVector3D GetCurrentPosition();
	//设置相机当前位置
	void SetCurrentPosition(QVector3D inposition);
	//获取相机当前朝向
	QVector3D GetCurrentForward();
	//设置相机当前朝向
	void SetCurrentForward(QVector3D inforward);
	//获取相机当前顶向
	QVector3D GetCurrentUpward();
	//设置相机当前顶向
	void SetCurrentUpward(QVector3D inupward);
	//获取相机当前的透视矩阵
	QMatrix4x4 GetPerspectiveMatrix();
	//获取相机当前的视图矩阵
	QMatrix4x4 GetViewMatrix();

	void CameraMove(int inx, int iny);
};

