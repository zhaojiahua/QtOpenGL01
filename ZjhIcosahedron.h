#pragma once
#include "AllIncludeFiles.h"
class ZjhIcosahedron
{
public:
	//默认构造(第0个点位置在(0,0,-1))
	ZjhIcosahedron();
	//默认构造的基础上做transform变换
	ZjhIcosahedron(QTransform intransfrom);

	void DebugPrint();

private:
	float pointDatas[36]{};//将数组元素初始化成0(12个顶点)
	float verticesDatas[360]{};//60个顶点信息和60个法线信息
	float normalDatas[180]{};//每个面的三个顶点各有法线方向,一共60个法线
	int indexs[60]{
							0,1,2,	//0
							0,2,3,	//1
							0,3,4,	//2	
							0,4,5,	//3
							0,5,1,	//4
							1,5,6,	//5
							5,4,7,	//6
							4,3,8,	//7
							3,2,9,	//8
							2,1,10,	//9
							1,6,10,	//10
							6,5,7,	//11
							7,4,8,	//12
							8,3,9,	//13
							9,2,10,	//14
							6,11,10,	//15
							9,10,11,	//16
							8,9,11,		//17
							7,8,11,		//18
							6,7,11		//19
						};//20个面
	float face0Datas[18]{};
	float face1Datas[18]{};
	float face2Datas[18]{};
	float face3Datas[18]{};
	float face4Datas[18]{};
	float face5Datas[18]{};
	float face6Datas[18]{};
	float face7Datas[18]{};
	float face8Datas[18]{};
	float face9Datas[18]{};
	float face10Datas[18]{};
	float face11Datas[18]{};
	float face12Datas[18]{};
	float face13Datas[18]{};
	float face14Datas[18]{};
	float face15Datas[18]{};
	float face16Datas[18]{};
	float face17Datas[18]{};
	float face18Datas[18]{};
	float face19Datas[18]{};
	std::vector<float*> faces;

	float zjhTriangle[9]{ 10,10,-1, 0,1,1,  1,0,1 };

public:
	float* GetPointDatas();
	float* GetAllVerticesDatas();
	float* GetNormalDatas();
	float* GetFacesDatas(int faceinidex);
	float* GetZjhTriangle();
	int* GetIndices();

	void PrintNormal();

	//输入旋转轴向和旋转角度,获得相应的旋转矩阵
	static QMatrix4x4 GetRotateMatrix(double inradi, QVector3D indirection);

private:
	//依次输入三个点,复制出与最后一个点关于前两个点与原点组成的面对称的点
	QVector3D GetNewPoint(QVector3D pt0, QVector3D pt1, QVector3D pt2);
};

