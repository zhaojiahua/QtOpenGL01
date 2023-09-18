#pragma once
#include "AllIncludeFiles.h"//����Qt�������ļ�

enum FileType
{
	Z_OBJ,
	Z_FBX,
	Z_ABC,
	Z_STL,
};

class ZjhMesh
{

	int pointnum = 0;//���е����Ŀ
	int trianglefacenum = 0;//���������Ŀ
	float* verticesDatas;//���е������
	int* indices;//���е���������

	QFile file;
	QDir directery;//��������ϵͳ�ļ���Ŀ¼

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

