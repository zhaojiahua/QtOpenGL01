#include "ZjhCamera.h"

ZjhCamera::ZjhCamera():
	mPosition(0,0,1),mForward(0,0,-1),mUpward(0,1,0), mFov(60), mRatio(1.7778), mNear(0.1),mFar(500)
{
	mPMatrix.setToIdentity();
	mVMatrix.setToIdentity();
	UpdataPMatrix();
	UpdataVMatrix();
}

void ZjhCamera::UpdataPMatrix()
{
	mPMatrix.perspective(mFov, mRatio, mNear, mFar);
}

void ZjhCamera::UpdataVMatrix()
{
	mVMatrix.setToIdentity();
	mVMatrix.lookAt(mPosition, mPosition + mForward * 10, mUpward);
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
	mPMatrix.setToIdentity();
	return mPMatrix;
}

QMatrix4x4 ZjhCamera::GetViewMatrix()
{
	return mVMatrix;
}

void ZjhCamera::CameraMove(int inx, int iny)
{
	QVector3D leftward = QVector3D::crossProduct(mUpward, mForward);
	mSensitivity = 0.001;
	mPosition += inx * mSensitivity * leftward;
	//qDebug() << " mPosition " << mPosition.x() << " " << mPosition.y() << " " << mPosition.z();
	mPosition = mPosition + iny * mSensitivity * mUpward;
	UpdataVMatrix();
}
