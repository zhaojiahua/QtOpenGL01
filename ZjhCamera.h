#pragma once
#include "AllIncludeFiles.h"

class ZjhCamera
{
public:
	//Ĭ�Ϲ��������(0,0,1)��λ��,����(0,0,-1),������ӽ�60��,��߱�1.77778,���ü���0.1,Զ�ü���500
	ZjhCamera();

	//���������
	float mSensitivity = 1.0;
	
private:
	QVector3D mPosition;//�����ǰλ��
	QVector3D mForward;//�����ǰ����
	QVector3D mUpward;//���ͷ������

	float mFov;
	float mRatio;
	float mNear;
	float mFar;

	QMatrix4x4 mPMatrix;//���͸�Ӿ���
	QMatrix4x4 mVMatrix;//�����ͼ����

	void UpdataPMatrix();//�ӽ�,��߱�,���ü���,Զ�ü������ĸ�������һ�������ı䶼Ҫ����͸�Ӿ���
	void UpdataVMatrix();//λ��,����,����������������һ�������仯��Ҫ������ͼ����

public:
	//��ȡ�����ǰ�ӽǿ��
	float Fov();
	//���������ǰ�ӽǿ��
	void Fov(float infov);
	//��ȡ�����ǰ��߱���
	float Ratio();
	//���������ǰ��߱���
	void Ratio(float inratio);
	//��ȡ�����ǰ������
	float Near();
	//���������ǰ������
	void Near(float innear);
	//��ȡ�����ǰԶ����
	float Far();
	//���������ǰԶ����
	void Far(float infar);
	
	//��ȡ�����ǰλ��
	QVector3D GetCurrentPosition();
	//���������ǰλ��
	void SetCurrentPosition(QVector3D inposition);
	//��ȡ�����ǰ����
	QVector3D GetCurrentForward();
	//���������ǰ����
	void SetCurrentForward(QVector3D inforward);
	//��ȡ�����ǰ����
	QVector3D GetCurrentUpward();
	//���������ǰ����
	void SetCurrentUpward(QVector3D inupward);
	//��ȡ�����ǰ��͸�Ӿ���
	QMatrix4x4 GetPerspectiveMatrix();
	//��ȡ�����ǰ����ͼ����
	QMatrix4x4 GetViewMatrix();

	void CameraMove(int inx, int iny);
};

