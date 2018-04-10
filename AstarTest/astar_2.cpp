///////////////////////////////////////////////////////////////
////Search method:
////First, check whether the point belongs to the way forward
////if it is,then add to the openTable
////Second, find out from openTable least costly
/////////////////////////////////////////////////////////////// 
//
//#include "astar.h"
//#include <cmath>
//#include <algorithm>
//
//astar::astar(std::vector<double> arg1, DVolume &arg2)
//{
//	if (arg1.empty() || arg1.size() != 6)
//	{
//		std::cout << "startPoint and endPoint Parameter ERROR!" << std::endl;
//		return;
//	}
//	if (arg2.IsEmpty())
//	{
//		std::cout << "source data ERROR!" << std::endl;
//		return;
//	}
//	startPoint.x = arg1[0];
//	startPoint.y = arg1[1];
//	startPoint.z = arg1[2];
//	startPoint.costToStart = 0;
//	startPoint.costToEnd = calcCostToEnd(startPoint);
//
//	endPoint.x = arg1[3];
//	endPoint.y = arg1[4];
//	endPoint.z = arg1[5];
//
//	currentOpenTable.push_back(&startPoint);
//
//	sourceData.QuickCopy(arg2);//get source data
//	dataFlag.SetSize(sourceData.x(), sourceData.y(), sourceData.z());
//	dataFlag.SetZero();
//	getGrayRange();
//
//	//generate orientation vector
//	int iFlag = 0, jFlag = 0, kFlag = 0;
//	for (double i = 0; i < 3; ++i)
//	{
//		if (i == 0 || i == 2)
//			iFlag = 1;
//		else iFlag = 0;
//		for (double j = 0; j < 3; ++j)
//		{
//			if (j == 0 || j == 2)
//				jFlag = 1;
//			else jFlag = 0;
//			for (double k = 0; k < 3; ++k)
//			{
//				if (k == 0 || k == 2)
//					kFlag = 1;
//				else kFlag = 0;
//				if (!(i == 1 && j == 1 && k == 1))
//				{
//					pointTable tem(i - 1, j - 1, k - 1);
//					//tem.positionToPrevious = (i * j * k)*sqrt(iFlag + jFlag + kFlag);//(i=0||j=0||k=0)//
//					tem.positionToPrevious = sqrt(iFlag + jFlag + kFlag);
//					tem.positionVector << (i - 1), (j - 1), (k - 1);
//					orientationVector.push_back(tem);
//				}
//			}
//		}
//	}
//
//	std::cout << "astar Initialize Completed!" << std::endl;//for test
//
//}
//
//astar::~astar()
//{
//}
//
//bool compare(pointTable *arg1, pointTable *arg2)
//{
//	return (std::abs(arg1->getTotalCost()) < std::abs(arg2->getTotalCost()));//< descending sort   //> increment sprt
//	//return (arg1.costToEnd > arg2.costToEnd);//descending sort
//}
//
//void astar::getGrayRange()
//{
//	double *temGray = &sourceData.GetPixel(0, 0, 0);
//	grayMin = *temGray;
//	for (size_t coutNum1 = 0; coutNum1 < sourceData.x()*sourceData.y()*sourceData.z(); ++coutNum1)
//	{
//		if (*temGray > grayMax)
//			grayMax = *temGray;
//		else if (*temGray < grayMin)
//			grayMin = *temGray;
//		++temGray;
//	}
//}
//
//double astar::normaValue(double arg)
//{
//	return ((arg - grayMin) / (grayMax - grayMin));
//}
//
//double astar::calcCostToStart(pointTable &arg)
//{
//	double result = 0;
//	//here recursion, need to control the end of the conditions
//	if (arg.fatherPoint != NULL)
//		result = arg.fatherPoint->costToStart - arg.positionToPrevious *0.1 + normaValue(arg.grayValue)*0.9;//+ or -
//	return result;//start point result =0
//}
//
//double astar::calcCostToEnd(pointTable &arg)
//{
//	//if arg==endPoint,result = ?
//	double result = 0, mdistance = 0;
//	//result = sqrt(pow(endPoint.x - arg.x, 2) + pow(endPoint.y - arg.y, 2) + pow(endPoint.y - arg.y, 2));
//	//Euclid distance
//	mdistance = std::abs(endPoint.x - arg.x) + std::abs(endPoint.y - arg.y) + std::abs(endPoint.z - arg.z);
//	//Manhattan distance better
//	result = normaValue(arg.grayValue)*0.9 - mdistance *0.1;//+ or -
//	return result;
//}
//
//bool astar::eveluationRange(pointTable &arg)
//{
//	if (arg.x >= sourceData.x() || arg.y >= sourceData.y() || arg.z >= sourceData.z() || arg.x < 0 || arg.y < 0 || arg.z < 0)
//		return false;
//	else return true;
//}
//
//bool astar::checkRepeat(pointTable &arg)
//{
//	if (dataFlag(arg.x, arg.y, arg.z) != 0)
//		return false;
//	else return true;
//}
//
//void astar::getNeighborToOpenTable()
//{
//	currPointIte = currentOpenTable.begin();
//	while (currPointIte != currentOpenTable.end())
//	{
//		neighborTable.clear();
//
//		//here need to be optimize
//		std::for_each(orientationVector.begin(), orientationVector.end(),
//			[&](pointTable &arg1)
//		{
//			//pointTable tem111 = arg1 + **currPointIte;
//			neighborTable.push_back(&(arg1 + **currPointIte));/////////////////////////////////
//			//neighborTable.push_back(&tem111);
//			if (eveluationRange(*neighborTable.back()) && checkRepeat(*neighborTable.back()))
//			{
//				//no repeat
//				dataFlag(neighborTable.back()->x, neighborTable.back()->y, neighborTable.back()->z) = 1;
//				neighborTable.back()->grayValue = sourceData(neighborTable.back()->x, neighborTable.back()->y, neighborTable.back()->z);
//				neighborTable.back()->costToStart = calcCostToStart(*neighborTable.back());
//				neighborTable.back()->costToEnd = calcCostToEnd(*neighborTable.back());
//			}
//			else neighborTable.erase(neighborTable.end() - 1);//out of range or repeat
//		});
//		allNeighborTable.insert(allNeighborTable.end(), neighborTable.begin(), neighborTable.end());
//		++currPointIte;
//	}
//	std::sort(allNeighborTable.begin(), allNeighborTable.end(), compare);
//}
//
//void astar::searchOpenTable()
//{
//	static int countNum2 = 0;//for test
//	std::cout << "searchOpenTable()" << std::endl;/////////////////////////////////////////////////
//
//	getNeighborToOpenTable();//get currentOpenTable every point neighbor to allNeighborTable
//
//	//search all of the point on currentOpenTable
//	endPointFinder = std::find_if(allNeighborTable.begin(), allNeighborTable.end(),
//		[&](pointTable* arg){return(*arg == endPoint); });////////////////////////
//
//	if (endPointFinder != allNeighborTable.end())//found it
//	{
//		currentOpenTable = allNeighborTable;
//		allOpenTable.push_back(currentOpenTable);
//		searchFlag = true;
//		return;
//	}
//	else
//	{
//		allOpenTable.push_back(currentOpenTable);
//		currentOpenTable.clear();
//		//
//		//if (allOpenTable.size() > 4)
//		//{
//		//	//currentOpenTable.clear();
//		//	//currentOpenTable = allNeighborTable;
//		//	//currentOpenTable.assign(allNeighborTable.begin(), allNeighborTable.begin() + (allNeighborTable.size()*0.15));
//		//	currentOpenTable.assign(allNeighborTable.begin(), allNeighborTable.begin() + 600);
//		//}
//		//else currentOpenTable = allNeighborTable;///////////////////////!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//		//currentOpenTable = allNeighborTable;
//		currentOpenTable.assign(allNeighborTable.begin(), allNeighborTable.end());
//		allNeighborTable.clear();
//	}
//
//
//	++countNum2;///////////for test
//	std::cout << countNum2 << std::endl;//for test
//}
//
//void astar::search()
//{
//	if (sourceData.IsEmpty())
//	{
//		std::cout << "source data ERROR!" << std::endl;
//		return;
//	}
//
//	std::cout << "Start Search!" << std::endl;//for test
//
//	//ready to search
//	while (!searchFlag)
//		searchOpenTable();
//
//	std::cout << "Search ended!" << std::endl
//		<< "Search father point!" << std::endl;//for test
//
//
//	//search final path
//	/*
//	while (endPointFinder->fatherPoint != NULL)
//	{
//	pathTable.push_back(*endPointFinder->fatherPoint);
//	*endPointFinder = *endPointFinder->fatherPoint;
//	}
//	pathTable.push_back(*endPointFinder);
//	*/
//
//
//	/*
//	for (size_t countNum5 = 0; countNum5 < allOpenTable.size(); ++countNum5)
//	pathTable.push_back(allOpenTable[countNum5].front());
//	*/
//	std::cout << "Completed!" << std::endl;//for test
//}
