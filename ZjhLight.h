#pragma once
#include "AllIncludeFiles.h"

class ZjhLight
{

public:
	//�����ƹ�ķ���,λ�ú���ɫ(Ĭ����ƽ�й�)
	ZjhLight(QVector3D inDir, QVector3D inPos, QColor inColor);

	QVector3D mDirection;
	QVector3D mPosition;
	QColor mColor;
};

