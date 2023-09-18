#include "ZjhCamera.h"
#include <cmath>

ZjhCamera::ZjhCamera():
	mPosition(0,0,5),mForward(0,0,-1),mUpward(0,1,0), mFov(60), mRatio(1.7778), mNear(0.1),mFar(1000)
{
	mPMatrix.setToIdentity();
	mVMatrix.setToIdentity();
	mPMatrix.perspective(mFov, mRatio, mNear, mFar);
	mVMatrix.lookAt(mPosition, mPosition + mForward * 100, mUpward);
}

void ZjhCamera::UpdataPMatrix()
{
	mPMatrix.setToIdentity();
	mPMatrix.perspective(mFov, mRatio, mNear, mFar);
}

void ZjhCamera::UpdataVMatrix()
{
	mVMatrix.setToIdentity();
	mVMatrix.lookAt(mPosition, mPosition + mForward * 100, mUpward);
}

void ZjhCamera::UpdataForward()
{
	//这里确保θ取值在正负180之间
	if (theat > PI)theat -= PI_2;
	if (theat < -PI)theat += PI_2;

	mForward.setZ(cos(phi) * cos(theat));//θ和φ都为0的时候指向z轴正方向,所以默认θ=180
	mForward.setX(cos(phi) * sin(theat));
	mForward.setY(sin(phi));
}

void ZjhCamera::UpdataPosition()
{
	mPosition = springForce - mForward * springLength;
}

float ZjhCamera::Fov()
{
	return mFov;
}

void ZjhCamera::Fov(float infov)
{
	mFov = infov;
	UpdataPMatrix();
}

float ZjhCamera::Ratio()
{
	return mRatio;
}

void ZjhCamera::Ratio(float inratio)
{
	mRatio = inratio;
	UpdataPMatrix();
}

float ZjhCamera::Near()
{
	return mNear;
}

void ZjhCamera::Near(float innear)
{
	mNear = innear;
	UpdataPMatrix();
}

float ZjhCamera::Far()
{
	return mFar;
}

void ZjhCamera::Far(float infar)
{
	mFar = infar;
	UpdataPMatrix();
}

QVector3D ZjhCamera::GetCurrentPosition()
{
	return mPosition;
}

void ZjhCamera::SetCurrentPosition(QVector3D inposition)
{
	mPosition = inposition;
	UpdataVMatrix();
}

QVector3D ZjhCamera::GetCurrentForward()
{
	return mForward;
}

void ZjhCamera::SetCurrentForward(QVector3D inforward)
{
	mForward = inforward;
	UpdataVMatrix();
}

QVector3D ZjhCamera::GetCurrentUpward()
{
	return mUpward;
}

void ZjhCamera::SetCurrentUpward(QVector3D inupward)
{
	mUpward = inupward;
	UpdataVMatrix();
}

QMatrix4x4 ZjhCamera::GetPerspectiveMatrix()
{
	return mPMatrix;
}

QMatrix4x4 ZjhCamera::GetViewMatrix()
{
	return mVMatrix;
}

void ZjhCamera::CameraMove(int inx, int iny)
{
	QVector3D leftward = QVector3D::crossProduct(mUpward, mForward);
	mSensitivity = 0.002;
	mPosition += inx * mSensitivity * leftward;
	//qDebug() << " mPosition " << mPosition.x() << " " << mPosition.y() << " " << mPosition.z();
	mPosition = mPosition + iny * mSensitivity * mUpward;
	UpdataVMatrix();
}

void ZjhCamera::TrackingShot(int iny)
{
	mSensitivity = 0.002;
	springLength -= mSensitivity * iny;
	UpdataPosition();
	UpdataVMatrix();
}

void ZjhCamera::CameraRotate(int inx, int iny)
{
	mSensitivity = 0.002;
	theat -= inx * mSensitivity;
	phi -= iny * mSensitivity;
	//这里φ要限制在正负90度之间
	if (phi > PI_0_5) phi = PI_0_5;
	if (phi < -PI_0_5) phi = -PI_0_5;
	UpdataForward();
	UpdataPosition();
	UpdataVMatrix();
}

void ZjhCamera::CameraForceOn(int inx, int iny)
{
	mSensitivity = 0.01;
	
	mFov += (mSensitivity * inx + mSensitivity * iny);

	UpdataPMatrix();
}
