#pragma once
#include "AllIncludeFiles.h"

class ZjhLight
{

public:
	//给定灯光的方向,位置和颜色(默认是平行光)
	ZjhLight(QVector3D inDir, QVector3D inPos, QColor inColor);

	QVector3D mDirection;
	QVector3D mPosition;
	QColor mColor;
};

