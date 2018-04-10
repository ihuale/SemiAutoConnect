#include "SemiAutoConnect.h"
#include <algorithm>
#include <iostream>

SemiAutoConnect::SemiAutoConnect(std::vector<double> arg1,DVolume &arg2)
{
	if (arg1.empty() || arg1.size() != 6 )
	{
		std::cout << "startPoint and endPoint Parameter ERROR!" << std::endl;
		return;
	}
	if (arg2.IsEmpty())
	{
		std::cout << "source data ERROR!" << std::endl;
		return;
	}
	startPoint.x = arg1[0];
	startPoint.y = arg1[1];
	startPoint.z = arg1[2];
	startPoint.costToStart = 0;
	startPoint.costToEnd = calcCostToEnd(startPoint);

	endPoint.x = arg1[3];
	endPoint.y = arg1[4];
	endPoint.z = arg1[5];

	sToendVec << (arg1[3] - arg1[0]), (arg1[4] - arg1[1]), (arg1[5] - arg1[2]);
	sToendVec.normalize();
	vertexPoint << (startPoint.x - 10 * sToendVec(0)), (startPoint.y - 10 * sToendVec(1)), (startPoint.z - 10 * sToendVec(2));//////////////////////////////////////////////////////

	currentOpenTable.push_back(std::shared_ptr<pointTable>(new pointTable(startPoint)));
	
	sourceData.QuickCopy(arg2);//get source data
	dataFlag.SetSize(sourceData.x(), sourceData.y(), sourceData.z());
	dataFlag.SetZero();
	dataFlag(startPoint.x, startPoint.y, startPoint.z) = 1;
	getGrayRange();

	std::cout << "SemiAutoConnect Initialize Completed!" << std::endl;//for test

}

SemiAutoConnect::~SemiAutoConnect()
{
}

bool compare(std::shared_ptr<pointTable> &arg1, std::shared_ptr<pointTable> &arg2)
{
	return (std::abs(arg1->getTotalCost()) > std::abs(arg2->getTotalCost()));//< descending sort > increment sprt
	//return (arg1.costToEnd > arg2.costToEnd);//descending sort
}

void SemiAutoConnect::getGrayRange()
{
	double *temGray = &sourceData.GetPixel(0, 0, 0);
	grayMin = *temGray;

	//here should be taken getSearchRange() data£¬not sourceData!
	for (size_t coutNum1 = 0; coutNum1 < sourceData.x()*sourceData.y()*sourceData.z(); ++coutNum1)
	{
		if (*temGray > grayMax)
			grayMax = *temGray;
		else if (*temGray < grayMin)
			grayMin = *temGray;
		++temGray;
	}
}

double SemiAutoConnect::normaValue(double arg)
{
	return ((arg - grayMin) / (grayMax - grayMin));
}

double SemiAutoConnect::calcCostToStart(pointTable &arg)
{
	double result = 0;
	//here recursion, need to control the end of the conditions
	if (arg.fatherPoint.get())
		result = arg.fatherPoint->costToStart - arg.positionToPrevious *0 + normaValue(arg.grayValue)*1;//+ or -
	return result;//start point result =0
}

double SemiAutoConnect::calcCostToEnd(pointTable &arg)
{
	return normaValue(arg.grayValue) * 1;
	//if arg==endPoint,result = ?
	//double result = 0, mdistance = 0;
	//result = sqrt(pow(endPoint.x - arg.x, 2) + pow(endPoint.y - arg.y, 2) + pow(endPoint.y - arg.y, 2));
	//Euclid distance
	//mdistance = std::abs(endPoint.x - arg.x) + std::abs(endPoint.y - arg.y) + std::abs(endPoint.z - arg.z);
	//Manhattan distance better
	//result = normaValue(arg.grayValue)*1 - mdistance *0;//+ or -
	//return result;
}

bool SemiAutoConnect::eveluationRange(pointTable &arg)
{
	if (arg.x >= sourceData.x() || arg.y >= sourceData.y() || arg.z >= sourceData.z() || arg.x < 0 || arg.y < 0 || arg.z < 0)
		return false;
	if (dataFlag(arg.x, arg.y, arg.z) != 0)///////////////////
		return false;
	if (arg == endPoint)
		return true;
	arg.grayValue = sourceData(arg.x, arg.y, arg.z);
	//if (testFlag == true && calcCostToEnd(arg) <= 0.08)
	if (calcCostToEnd(arg) <= normRange)
		return false;

	Vec3d verToargVec((arg.x - vertexPoint(0)), (arg.y - vertexPoint(1)), (arg.z - vertexPoint(2)));
	verToargVec.normalize();
	if (verToargVec.dot(sToendVec) >= 0.7)
		return true;
	else return false;
}

void SemiAutoConnect::getNeighborToOpenTable()//get currentOpenTable every point's neighbor to allNeighborTable
{ 
	allNeighborTable.clear();
	for (auto &arg : currentOpenTable)
	{
		for (int i = -1; i < 2; ++i){
			for (int j = -1; j < 2; ++j){
				for (int ij = -1; ij < 2; ++ij){
					//check boundary
					if (eveluationRange(pointTable((*arg).x + i, (*arg).y + j, (*arg).z + ij)))
					{
						allNeighborTable.push_back(std::make_shared<pointTable>(*arg));
						allNeighborTable.back()->x += i;
						allNeighborTable.back()->y += j;
						allNeighborTable.back()->z += ij;
						allNeighborTable.back()->fatherPoint = arg;
						dataFlag((*arg).x + i, (*arg).y + j, (*arg).z + ij) = 1;
						allNeighborTable.back()->grayValue = sourceData(allNeighborTable.back()->x, allNeighborTable.back()->y, allNeighborTable.back()->z);
						allNeighborTable.back()->costToStart = calcCostToStart(*allNeighborTable.back());
						allNeighborTable.back()->costToEnd = normaValue(allNeighborTable.back()->grayValue);
					}
				}
			}
		}
	}
	std::sort(allNeighborTable.begin(), allNeighborTable.end(), compare);
}

void SemiAutoConnect::searchOpenTable()
{
	getNeighborToOpenTable();

	//search all of the point on currentOpenTable
	endPointFinder = std::find_if(allNeighborTable.begin(), allNeighborTable.end(),
		[&](std::shared_ptr<pointTable>& arg){return(*arg == endPoint); });

	if (endPointFinder != allNeighborTable.end())//found it
	{
		currentOpenTable = allNeighborTable;
		allOpenTable.push_back(currentOpenTable);
		searchFlag = true;
		return;
	}
	else
	{
		if (allNeighborTable.size() == 0)
		{
			errorFlag = true;
			std::cout << "Threshold value error! Please reset!" << std::endl;
			return;
		}
		allOpenTable.push_back(currentOpenTable);
		currentOpenTable.clear();
		currentOpenTable.swap(allNeighborTable);
		allNeighborTable.clear();
	}

}

void SemiAutoConnect::search()
{
	std::cout << "Start Search!" << std::endl;//for test

	//ready to search
	int countNum2 = 0;//for test
	while (!searchFlag && !errorFlag)
	{
		std::cout << "searchOpenTable()" << currentOpenTable.size() << std::endl;/////////////////
		std::cout << ++countNum2 << std::endl;//for test
		searchOpenTable();
	}

	if (!errorFlag)
	{
		std::cout << "Search ended!" << std::endl
			<< "Search father point!" << std::endl;//for test

		//search final path
		std::vector<pointTable> temLine;
		while ((*endPointFinder)->fatherPoint.get())
		{
			temLine.push_back(**endPointFinder);
			*endPointFinder = (*endPointFinder)->fatherPoint;
		}
		temLine.push_back(**endPointFinder);
		std::reverse(temLine.begin(), temLine.end());
		///////////////////////////////////////////////////////

		for (int ii = 0; ii < temLine.size(); ++ii)
		{
			Vec3d tem(temLine[ii].x, temLine[ii].y, temLine[ii].z);
			pathTable.push_back(tem);
		}
		/*
		MatXd mat(17, 3);
		mat << 50, 12, 32,
		49, 11, 31,
		48, 10, 31,
		47, 10, 31,
		46, 10, 31,
		45, 10, 31,
		44, 10, 31,
		43, 11, 29,
		42, 12, 28,
		41, 13, 28,
		40, 14, 28,
		39, 15, 27,
		38, 16, 26,
		37, 17, 25,
		36, 18, 24,
		35, 19, 23,
		34, 20, 22;
		std::cout << mat << std::endl << std::endl;
		VectorVec3d test;
		for (int iii = 0; iii < mat.rows(); ++iii)
		test.push_back(mat.row(iii));
		*/

		ThreeDimdCurveResample(pathTable, 1, traceLine);
		//ThreeDimdCurveResample(test, 1, traceLine);
		std::for_each(traceLine.begin(), traceLine.end(), [&](Vec3d &arg){std::cout << arg.transpose() << std::endl; });
		////////////////////////////////////////////////////////////////////////////////////
		std::cout << "Completed!" << std::endl;//for test
	}
}

void SemiAutoConnect::ThreeDimdCurveResample(VectorVec3d &curvePoints, double incValue, VectorVec3d &traceLine)
{
	double curvePointsNum = curvePoints.size();
	VectorVec3d resampleCurve;
	resampleCurve.resize(5 * curvePointsNum);
	std::for_each(resampleCurve.begin(), resampleCurve.end(),
		[&](Vec3d &arg){arg.setZero(); });

	resampleCurve[0] = curvePoints[0];
	std::vector<int> resampleIndex(5 * curvePointsNum);
	resampleIndex[0] = 1;

	size_t i = 0;
	for (; i < 5 * curvePointsNum - 2; ++i)
	{
		if (resampleIndex[i] < (curvePointsNum))
		{
			VectorVec3d::iterator iteFirst = curvePoints.begin() + (resampleIndex[i]);
			VectorVec3d::iterator iteLast = curvePoints.begin() + std::min(resampleIndex[i] + 3, (int)(curvePointsNum));
			VectorVec3d adjacentPoint(iteFirst, iteLast);
			
			if (adjacentPoint.size() > 1)
			{
				std::vector<double> vectorDist;//whether need initialization?
				for (size_t j = 0; j < adjacentPoint.size(); ++j)
					vectorDist.push_back((adjacentPoint[j] - resampleCurve[i]).norm());
				
				std::vector<int> resampleFlag;
				for (int countNum1 = 0; countNum1 < vectorDist.size(); ++countNum1)
				{
					if (vectorDist[countNum1] > incValue)
						resampleFlag.push_back(countNum1);
				}
				
				
				if (resampleFlag.size() == 0)//not found
				{
					resampleIndex[i + 1] = std::min(resampleIndex[i] + 3, (int)(curvePointsNum));
					resampleCurve[i + 1] = curvePoints[std::min(resampleIndex[i] + 2, (int)(curvePointsNum - 1))];
				}
				else
				{
					Vec3d curDir;
					if (resampleFlag[0] == 0)
					{
						//here not test!!!!
						resampleIndex[i + 1] = resampleIndex[i];////////////////////////////////
						curDir = (curvePoints[resampleIndex[i]] - resampleCurve[i]) / vectorDist[0];/////////////////
						resampleCurve[i + 1] = resampleCurve[i] + 1.5*curDir;
					}
					else
					{
						curDir = adjacentPoint[resampleFlag[0]] - adjacentPoint[resampleFlag[0] - 1];
						curDir.normalize();
						double incStep = pow(incValue, 2) - pow((adjacentPoint[resampleFlag[0] - 1] - resampleCurve[i]).norm(), 2);
						resampleCurve[i + 1] = adjacentPoint[resampleFlag[0] - 1] + sqrt(incStep)*curDir;
						resampleIndex[i + 1] = resampleFlag[0] + resampleIndex[i];
					}
				}
			}

			if (adjacentPoint.size() == 1)
			{
				resampleIndex[i + 1] = curvePointsNum;
				resampleCurve[i + 1] = adjacentPoint[0];
			}
		}
		else break;
	}
	traceLine.assign(resampleCurve.begin(), resampleCurve.begin() + i + 1);
};