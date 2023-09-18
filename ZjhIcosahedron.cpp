#include "ZjhIcosahedron.h"

ZjhIcosahedron::ZjhIcosahedron() {
	QList<QVector3D> vecPositions;
	QVector3D Pt00 = { 0.0, 0.0, -1.0 };		//point 0
	vecPositions.append(Pt00);
	pointDatas[0] = Pt00.x(); pointDatas[1] = Pt00.y(); pointDatas[2] = Pt00.z();
	QMatrix4x4 tempMatr = GetRotateMatrix(1.10715, QVector3D(1, 0, 0));//1.10715是正二十面体相邻两定点之间的固定夹角
	QVector3D Pt01 = tempMatr * Pt00;		//point 1
	vecPositions.append(Pt01);
	pointDatas[3] = Pt01.x(); pointDatas[4] = Pt01.y(); pointDatas[5] = Pt01.z();
	QVector3D  midPt = (Pt00 + Pt01).normalized();
	midPt = midPt * Pt00.length();
	QVector3D rotAxis = (Pt00 - Pt01).normalized();
	tempMatr = GetRotateMatrix(1.01722, rotAxis);//1.01722是正二十面体每三个相邻的点组成的三棱锥的棱面夹角
	QVector3D Pt02 = tempMatr * midPt;		//point 2	//第一个三棱锥已经出现接下来就是不断的复制就可以了
	vecPositions.append(Pt02);
	pointDatas[6] = Pt02.x(); pointDatas[7] = Pt02.y(); pointDatas[8] = Pt02.z();
	QVector3D Pt03 = GetNewPoint(Pt00, Pt02, Pt01);	//point 3
	vecPositions.append(Pt03);
	pointDatas[9] = Pt03.x(); pointDatas[10] = Pt03.y(); pointDatas[11] = Pt03.z();
	QVector3D Pt04 = GetNewPoint(Pt00, Pt03, Pt02);	//point 4
	vecPositions.append(Pt04);
	pointDatas[12] = Pt04.x(); pointDatas[13] = Pt04.y(); pointDatas[14] = Pt04.z();
	QVector3D Pt05 = GetNewPoint(Pt01, Pt00, Pt02);	//point 5
	vecPositions.append(Pt05);
	pointDatas[15] = Pt05.x(); pointDatas[16] = Pt05.y(); pointDatas[17] = Pt05.z();
	QVector3D Pt06 = GetNewPoint(Pt01, Pt05, Pt00);	//point 6
	vecPositions.append(Pt06);
	pointDatas[18] = Pt06.x(); pointDatas[19] = Pt06.y(); pointDatas[20] = Pt06.z();
	QVector3D Pt07 = GetNewPoint(Pt05, Pt04, Pt00);	//point 7
	vecPositions.append(Pt07);
	pointDatas[21] = Pt07.x(); pointDatas[22] = Pt07.y(); pointDatas[23] = Pt07.z();
	QVector3D Pt08 = GetNewPoint(Pt07, Pt04, Pt05);	//point 8
	vecPositions.append(Pt08);
	pointDatas[24] = Pt08.x(); pointDatas[25] = Pt08.y(); pointDatas[26] = Pt08.z();
	QVector3D Pt09 = GetNewPoint(Pt03, Pt02, Pt00);	//point 9
	vecPositions.append(Pt09);
	pointDatas[27] = Pt09.x(); pointDatas[28] = Pt09.y(); pointDatas[29] = Pt09.z();
	QVector3D Pt10 = GetNewPoint(Pt01, Pt06, Pt05);	//point 10
	//QVector3D Pt10 = QVector3D(2, 2, 1);	//point 10
	vecPositions.append(Pt10);
	pointDatas[30] = Pt10.x(); pointDatas[31] = Pt10.y(); pointDatas[32] = Pt10.z();
	QVector3D Pt11 = QVector3D(0.0f, 0.0f, 1.0f);	//point 11
	vecPositions.append(Pt11);
	pointDatas[33] = Pt11.x(); pointDatas[34] = Pt11.y(); pointDatas[35] = Pt11.z();

	faces.push_back(face0Datas);
	faces.push_back(face1Datas);
	faces.push_back(face2Datas);
	faces.push_back(face3Datas);
	faces.push_back(face4Datas);
	faces.push_back(face5Datas);
	faces.push_back(face6Datas);
	faces.push_back(face7Datas);
	faces.push_back(face8Datas);
	faces.push_back(face9Datas);
	faces.push_back(face10Datas);
	faces.push_back(face11Datas);
	faces.push_back(face12Datas);
	faces.push_back(face13Datas);
	faces.push_back(face14Datas);
	faces.push_back(face15Datas);
	faces.push_back(face16Datas);
	faces.push_back(face17Datas);
	faces.push_back(face18Datas);
	faces.push_back(face19Datas);

	//填充法线数据和vertices数据
	for (int i = 0; i < 20; i++) {
		int pt0_index = indexs[3 * i]; int pt1_index = indexs[3 * i + 1]; int pt2_index = indexs[3 * i + 2];
		QVector3D tempN = QVector3D::crossProduct((vecPositions[pt1_index] - vecPositions[pt0_index]).normalized(), (vecPositions[pt2_index] - vecPositions[pt0_index]).normalized());
		
		//第一个点
		faces[i][0]= vecPositions[pt0_index].x();
		faces[i][1] = vecPositions[pt0_index].y();
		faces[i][2] = vecPositions[pt0_index].z();
		faces[i][3] = tempN.x();
		faces[i][4] = tempN.y();
		faces[i][5] = tempN.z();

		verticesDatas[18 * i + 0] = vecPositions[pt0_index].x();
		verticesDatas[18 * i + 1] = vecPositions[pt0_index].y();
		verticesDatas[18 * i + 2] = vecPositions[pt0_index].z();
		verticesDatas[18 * i + 3 ] = tempN.x();
		verticesDatas[18 * i + 4 ] = tempN.y();
		verticesDatas[18 * i + 5 ] = tempN.z();

		//第二个点
		faces[i][6] = vecPositions[pt1_index].x();
		faces[i][7] = vecPositions[pt1_index].y();
		faces[i][8] = vecPositions[pt1_index].z();
		faces[i][9] = tempN.x();
		faces[i][10] = tempN.y();
		faces[i][11] = tempN.z();

		verticesDatas[18 * i + 6] = vecPositions[pt1_index].x();
		verticesDatas[18 * i + 7] = vecPositions[pt1_index].y();
		verticesDatas[18 * i + 8] = vecPositions[pt1_index].z();
		verticesDatas[18 * i + 9] = tempN.x();
		verticesDatas[18 * i + 10] = tempN.y();
		verticesDatas[18 * i + 11] = tempN.z();

		//第三个点
		faces[i][12] = vecPositions[pt2_index].x();
		faces[i][13] = vecPositions[pt2_index].y();
		faces[i][14] = vecPositions[pt2_index].z();
		faces[i][15] = tempN.x();
		faces[i][16] = tempN.y();
		faces[i][17] = tempN.z();

		verticesDatas[18 * i + 12] = vecPositions[pt2_index].x();
		verticesDatas[18 * i + 13] = vecPositions[pt2_index].y();
		verticesDatas[18 * i + 14] = vecPositions[pt2_index].z();
		verticesDatas[18 * i + 15] = tempN.x();
		verticesDatas[18 * i + 16] = tempN.y();
		verticesDatas[18 * i + 17] = tempN.z();

	}
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

float* ZjhIcosahedron::GetAllVerticesDatas()
{
	return verticesDatas;
}

float* ZjhIcosahedron::GetNormalDatas()
{
	return normalDatas;
}

float* ZjhIcosahedron::GetFacesDatas(int faceinidex)
{
	return faces[faceinidex];
}

float* ZjhIcosahedron::GetZjhTriangle()
{
	return zjhTriangle;
}

int* ZjhIcosahedron::GetIndices()
{
	return indexs;
}

void ZjhIcosahedron::PrintNormal()
{
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 3; j++) {
			qDebug() << normalDatas[9 * i + 3 * j] << " " << normalDatas[9 * i + 3 * j + 1] << " " << normalDatas[9 * i + 3 * j + 2];

		}
	}
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
	QVector3D rotAxis = (pt0 - pt1).normalized();
	QMatrix4x4 tempMatr = GetRotateMatrix(2.03444, rotAxis);
	return tempMatr * pt2;
}

