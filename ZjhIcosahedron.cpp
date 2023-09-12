#include "ZjhIcosahedron.h"

ZjhIcosahedron::ZjhIcosahedron() {
	QVector3D Pt00 = { 0.0, 0.0, -1.0 };		//point 0
	pointDatas[0] = Pt00.x(); pointDatas[1] = Pt00.y(); pointDatas[2] = Pt00.z();
	QMatrix4x4 tempMatr = GetRotateMatrix(1.10715, QVector3D(1, 0, 0));//1.10715是正二十面体相邻两定点之间的固定夹角
	QVector3D Pt01 = tempMatr * Pt00;		//point 1
	pointDatas[3] = Pt01.x(); pointDatas[4] = Pt01.y(); pointDatas[5] = Pt01.z();
	QVector3D  midPt = Pt00 + Pt01;
	midPt.normalize();
	midPt = midPt * Pt00.length();
	QVector3D rotAxis = Pt00 - Pt01; rotAxis.normalize();
	tempMatr = GetRotateMatrix(1.01722, rotAxis);//1.01722是正二十面体每三个相邻的点组成的三棱锥的棱面夹角
	QVector3D Pt02 = tempMatr * midPt;		//point 2	//第一个三棱锥已经出现接下来就是不断的复制就可以了
	pointDatas[6] = Pt02.x(); pointDatas[7] = Pt02.y(); pointDatas[8] = Pt02.z();
	QVector3D Pt03 = GetNewPoint(Pt00, Pt02, Pt01);	//point 3
	pointDatas[9] = Pt03.x(); pointDatas[10] = Pt03.y(); pointDatas[11] = Pt03.z();
	QVector3D Pt04 = GetNewPoint(Pt00, Pt03, Pt02);	//point 4
	pointDatas[12] = Pt04.x(); pointDatas[13] = Pt04.y(); pointDatas[14] = Pt04.z();
	QVector3D Pt05 = GetNewPoint(Pt01, Pt00, Pt02);	//point 5
	pointDatas[15] = Pt05.x(); pointDatas[16] = Pt05.y(); pointDatas[17] = Pt05.z();
	QVector3D Pt06 = GetNewPoint(Pt01, Pt05, Pt00);	//point 6
	pointDatas[18] = Pt06.x(); pointDatas[19] = Pt06.y(); pointDatas[20] = Pt06.z();
	QVector3D Pt07 = GetNewPoint(Pt05, Pt04, Pt00);	//point 7
	pointDatas[21] = Pt07.x(); pointDatas[22] = Pt07.y(); pointDatas[23] = Pt07.z();
	QVector3D Pt08 = GetNewPoint(Pt07, Pt04, Pt05);	//point 8
	pointDatas[24] = Pt08.x(); pointDatas[25] = Pt08.y(); pointDatas[26] = Pt08.z();
	QVector3D Pt09 = GetNewPoint(Pt06, Pt03, Pt04);	//point 9
	pointDatas[27] = Pt09.x(); pointDatas[28] = Pt09.y(); pointDatas[29] = Pt09.z();
	QVector3D Pt10 = GetNewPoint(Pt01, Pt06, Pt05);	//point 10
	pointDatas[30] = Pt10.x(); pointDatas[31] = Pt10.y(); pointDatas[32] = Pt10.z();
	QVector3D Pt11 = GetNewPoint(Pt06, Pt07, Pt05);	//point 11
	pointDatas[33] = Pt11.x(); pointDatas[34] = Pt11.y(); pointDatas[35] = Pt11.z();
}
ZjhIcosahedron::ZjhIcosahedron(QTransform intransfrom) {
	ZjhIcosahedron();
	for (int i = 0; i < 12; i++) {
		QVector3D temp3d = QVector3D(pointDatas[i * 3 + 0], pointDatas[i * 3 + 1], pointDatas[i * 3 + 2]);
		temp3d = intransfrom * temp3d;
		pointDatas[i * 3 + 0] = temp3d.x();
		pointDatas[i * 3 + 1] = temp3d.y();
		pointDatas[i * 3 + 2] = temp3d.z();
	}
}

void ZjhIcosahedron::DebugPrint()
{
	for (int i = 0; i < 12; i++) {
		QString tempstr = "point%1 : %2 %3 %4";
		qDebug() << tempstr.arg(i).arg(pointDatas[i * 3 + 0]).arg(pointDatas[i * 3 + 1]).arg(pointDatas[i * 3 + 2]);
	}
}

float* ZjhIcosahedron::GetPointDatas()
{
	return pointDatas;
}

int* ZjhIcosahedron::GetIndices()
{
	return indexs;
}

QMatrix4x4 ZjhIcosahedron::GetRotateMatrix(double inradi, QVector3D indirection)
{
	QMatrix4x4 tempM;
	tempM.setToIdentity();
	
	tempM.rotate(inradi / 3.1415926 * 180, indirection);
	return tempM;
}

QVector3D ZjhIcosahedron::GetNewPoint(QVector3D pt0, QVector3D pt1, QVector3D pt2)
{
	QVector3D rotAxis = pt0 - pt1; rotAxis.normalize();
	QMatrix4x4 tempMatr = GetRotateMatrix(2.03444, rotAxis);
	return tempMatr * pt2;
}

