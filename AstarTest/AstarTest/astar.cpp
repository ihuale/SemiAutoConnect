/////////////////////////////////////////////////////////////
//Search method:
//First, check whether the point belongs to the way forward
//if it is,then add to the openTable
//Second, find out from openTable least costly
///////////////////////////////////////////////////////////// 

#include "astar.h"
#include <cmath>
#include <algorithm>

astar::astar(std::vector<double> arg1,DVolume &arg2)
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

	currentOpenTable.push_back(std::shared_ptr<pointTable>(new pointTable(startPoint)));

	sourceData.QuickCopy(arg2);//get source data
	dataFlag.SetSize(sourceData.x(), sourceData.y(), sourceData.z());
	dataFlag.SetZero();
	dataFlag(startPoint.x, startPoint.y, startPoint.z) = 1;
	getGrayRange();

	std::cout << "astar Initialize Completed!" << std::endl;//for test

}

astar::~astar()
{
}

bool compare(std::shared_ptr<pointTable> &arg1, std::shared_ptr<pointTable> &arg2)
{
	return (std::abs(arg1->getTotalCost()) > std::abs(arg2->getTotalCost()));//< descending sort   //> increment sprt
	//return (arg1.costToEnd > arg2.costToEnd);//descending sort
}

void astar::getGrayRange()
{
	double *temGray = &sourceData.GetPixel(0, 0, 0);
	grayMin = *temGray;
	for (size_t coutNum1 = 0; coutNum1 < sourceData.x()*sourceData.y()*sourceData.z(); ++coutNum1)
	{
		if (*temGray > grayMax)
			grayMax = *temGray;
		else if (*temGray < grayMin)
			grayMin = *temGray;
		++temGray;
	}
}

double astar::normaValue(double arg)
{
	return ((arg - grayMin) / (grayMax - grayMin));
}

double astar::calcCostToStart(pointTable &arg)
{
	double result = 0;
	//here recursion, need to control the end of the conditions
	if (arg.fatherPoint.get())
		result = arg.fatherPoint->costToStart - arg.positionToPrevious *0 + normaValue(arg.grayValue)*1;//+ or -
	return result;//start point result =0
}

double astar::calcCostToEnd(pointTable &arg)//useless!!!
{
	//if arg==endPoint,result = ?
	double result = 0, mdistance = 0;
	//result = sqrt(pow(endPoint.x - arg.x, 2) + pow(endPoint.y - arg.y, 2) + pow(endPoint.y - arg.y, 2));
	//Euclid distance
	mdistance = std::abs(endPoint.x - arg.x) + std::abs(endPoint.y - arg.y) + std::abs(endPoint.z - arg.z);
	//Manhattan distance better
	result = normaValue(arg.grayValue)*1 - mdistance *0;//+ or -
	return result;
}

bool astar::eveluationRange(pointTable &arg)
{
	if (arg.x >= sourceData.x() || arg.y >= sourceData.y() || arg.z >= sourceData.z() || arg.x < 0 || arg.y < 0 || arg.z < 0)
		return false;
	else return true;
}

void astar::getNeighborToOpenTable()
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
						if (dataFlag((*arg).x + i, (*arg).y + j, (*arg).z + ij) == 0)
						{
							allNeighborTable.push_back(std::make_shared<pointTable>(*arg));
							allNeighborTable.back()->x += i;
							allNeighborTable.back()->y += j;
							allNeighborTable.back()->z += ij;
							allNeighborTable.back()->fatherPoint = arg;
							dataFlag((*arg).x + i, (*arg).y + j, (*arg).z + ij) = 1;
							allNeighborTable.back()->grayValue = sourceData(allNeighborTable.back()->x, allNeighborTable.back()->y, allNeighborTable.back()->z);
							allNeighborTable.back()->costToStart = calcCostToStart(*allNeighborTable.back());
							//allNeighborTable.back()->costToEnd = calcCostToEnd(*allNeighborTable.back());
							allNeighborTable.back()->costToEnd = normaValue(allNeighborTable.back()->grayValue);
						}
					}
				}
			}
		}
	}
	std::sort(allNeighborTable.begin(), allNeighborTable.end(), compare);
}

void astar::searchOpenTable()
{
	static int countNum2 = 0;//for test
	std::cout << "searchOpenTable()" << std::endl;/////////////////////////////////////////////////

	getNeighborToOpenTable();//get currentOpenTable every point neighbor to allNeighborTable

	//search all of the point on currentOpenTable
	endPointFinder = std::find_if(allNeighborTable.begin(), allNeighborTable.end(),
		[&](std::shared_ptr<pointTable>& arg){return(*arg == endPoint); });

	if (endPointFinder != allNeighborTable.end())//found it
	{
		currentOpenTable = allNeighborTable;////////////////////////////////
		allOpenTable.push_back(currentOpenTable);
		searchFlag = true;
		return;
	}
	else
	{
		allOpenTable.push_back(currentOpenTable);
		currentOpenTable.clear();
		
		if (allOpenTable.size() > 4)
		{
			currentOpenTable.swap(allNeighborTable);
			//currentOpenTable.assign(allNeighborTable.begin(), allNeighborTable.begin() + (allNeighborTable.size()*0.15));
			//currentOpenTable.assign(allNeighborTable.begin(), allNeighborTable.begin() + 400);
		}
		else currentOpenTable.swap(allNeighborTable);////////////////////////////////////////////////////////
		
		//currentOpenTable.swap(allNeighborTable);
		allNeighborTable.clear();
	}


	++countNum2;///////////for test
	std::cout << countNum2 << std::endl;//for test
}

void astar::search()
{
	std::cout << "Start Search!" << std::endl;//for test

	//ready to search
	while (!searchFlag)
		searchOpenTable();

	std::cout << "Search ended!" << std::endl 
		      << "Search father point!" << std::endl;//for test

	
	//search final path
	while ((*endPointFinder)->fatherPoint.get())
	{
		pathTable.push_back(**endPointFinder);
		*endPointFinder = (*endPointFinder)->fatherPoint;
	}
	pathTable.push_back(**endPointFinder);
	
	std::cout << "Completed!" << std::endl;//for test
}
