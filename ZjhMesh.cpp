#include "ZjhMesh.h"

ZjhMesh::ZjhMesh(FileType fileType, QString fullPath)
{
	modleMatrix.setToIdentity();
	file.setFileName(fullPath);
	file.open(QIODevice::ReadOnly);
	QTextStream textStr(&file);
	QString tempLine;
	QVector<QVector3D> t_posDatas;//用来暂时存放所有点的位置数据
	QVector<float> t_allDatas;//用来暂时存放所有顶点的位置数据和法线数据和UV数据和颜色数据等
	QVector<int> t_indexDatas;//用来暂时存放所有序号数据
	switch (fileType)
	{
	case Z_OBJ:
		while (textStr.readLineInto(&tempLine))
		{
			QStringList tempStrList = tempLine.split(" ");
			if (tempStrList.length() > 0) {
				if (tempStrList[0] == "v" && tempStrList.length() == 4) {
					t_posDatas.append(QVector3D(tempStrList[1].toFloat(), tempStrList[2].toFloat(), tempStrList[3].toFloat()));
				}
				else if (tempStrList[0] == "f" && tempStrList.length() > 2) {
					for (int ti = 0; ti < tempStrList.length() - 1 - 2; ti++) {//欧拉定理,面数比顶点数少2
						int pt0_i = tempStrList[1].toInt() - 1;
						int pt1_i = tempStrList[1 + ti + 1].toInt() - 1;
						int pt2_i = tempStrList[1 + ti + 2].toInt() - 1;
						QVector3D tempNr = QVector3D::crossProduct(t_posDatas[pt0_i] - t_posDatas[pt1_i], t_posDatas[pt0_i] - t_posDatas[pt2_i]).normalized();//面法线
						//填充第一个点
						t_allDatas.append(t_posDatas[pt0_i].x());
						t_allDatas.append(t_posDatas[pt0_i].y());
						t_allDatas.append(t_posDatas[pt0_i].z());
						t_allDatas.append(tempNr.x());
						t_allDatas.append(tempNr.y());
						t_allDatas.append(tempNr.z());
						//填充第二个点
						t_allDatas.append(t_posDatas[pt1_i].x());
						t_allDatas.append(t_posDatas[pt1_i].y());
						t_allDatas.append(t_posDatas[pt1_i].z());
						t_allDatas.append(tempNr.x());
						t_allDatas.append(tempNr.y());
						t_allDatas.append(tempNr.z());
						//填充第三个点
						t_allDatas.append(t_posDatas[pt2_i].x());
						t_allDatas.append(t_posDatas[pt2_i].y());
						t_allDatas.append(t_posDatas[pt2_i].z());
						t_allDatas.append(tempNr.x());
						t_allDatas.append(tempNr.y());
						t_allDatas.append(tempNr.z());

						trianglefacenum++;//统计一下三角面数
					}
				}
			}
			pointnum = t_posDatas.length();
			int tempsize = trianglefacenum * 18 * sizeof(float);//每个面有三个点,每个点有6个数据,分别是空间位置和法线方向
			verticesDatas = (float*)(malloc(tempsize));
			memcpy(verticesDatas, t_allDatas.constData(), tempsize);
		}
		break;
	case Z_FBX:
		break;
	case Z_ABC:
		break;
	case Z_STL:
		break;
	default:
		break;
	}
	file.close();
}

ZjhMesh::~ZjhMesh()
{
	free(verticesDatas);
	free(indices);
}

int ZjhMesh::PointNum()
{
	return pointnum;
}

int ZjhMesh::TriangleFaceNum()
{
	return trianglefacenum;
}

float* ZjhMesh::VerticesDatas()
{
	return verticesDatas;
}

int* ZjhMesh::IndicesDatas()
{
	return indices;
}

void ZjhMesh::DrawMesh()
{
	//glDrawElements(GL_TRIANGLES, pointnum, GL_UNSIGNED_INT, 0);
}

void ZjhMesh::DebugPrint()
{

}
