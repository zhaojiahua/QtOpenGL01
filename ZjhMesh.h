#pragma once
#include "AllIncludeFiles.h"//关于Qt的所有文件

enum FileType
{
	Z_OBJ,
	Z_FBX,
	Z_ABC,
	Z_STL,
};

class ZjhMesh
{

	int pointnum = 0;//所有点的数目
	int trianglefacenum = 0;//所有面的数目
	float* verticesDatas;//所有点的数据
	int* indices;//所有点的序号数据

	QFile file;
	QDir directery;//用来处理系统文件夹目录

public:
	ZjhMesh(FileType fileType, QString fullPath);
	~ZjhMesh();
	QMatrix4x4 modleMatrix;
	int PointNum();
	int TriangleFaceNum();
	float* VerticesDatas();
	int* IndicesDatas();

	void DrawMesh();

	void DebugPrint();

};

