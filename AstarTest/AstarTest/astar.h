#pragma once
#include "basetypes.h"
#include "volume.h"
#include <iostream>

struct pointTable 
{
	double x, y, z,grayValue, costToStart, costToEnd;//sumDis+=curDis;sumGray+=curGray; cost=sumDis*a+sumGray*b;
	/* useless!!! */
	double positionToPrevious;//1 is equal level,sqrt(2) is intermediate level,sqrt(3) is vertex
	
	double getTotalCost(){ return (costToStart + costToEnd); };
	Vec3d positionVector;
	std::shared_ptr<pointTable> fatherPoint;

	pointTable()
	{
		x = 0; y = 0; z = 0;
		grayValue = 0;
		costToStart = 0;
		costToEnd = 0;
		positionToPrevious = 0;
	};
	pointTable(double arg1, double arg2, double arg3)
	{
		x = arg1; y = arg2; z = arg3;
		grayValue = 0;
		costToStart = 0;
		costToEnd = 0;
		positionToPrevious = 0;
	};

	pointTable(const pointTable &arg)
	{
		*this = arg;
	};

	pointTable operator+(pointTable &arg)const
	{
		pointTable sum(*this);
		//sum.x = x + arg.x;
		//sum.y = y + arg.y;
		//sum.z = z + arg.z;
		//sum.positionVector = positionVector;
		//sum.positionToPrevious = positionToPrevious;

		////sum.fatherPoint = std::make_shared<pointTable>(arg);//no copy function
		////sum.fatherPoint = arg;
		
		return sum += arg;
	};
	pointTable& operator+=(pointTable &arg)
	{
		x = x + arg.x;
		y = y + arg.y;
		z = z + arg.z;
		positionVector = positionVector;
		positionToPrevious = positionToPrevious;

		//sum.fatherPoint = std::make_shared<pointTable>(new pointTable(arg));//no copy function
		//sum.fatherPoint.get() = &arg;

		return *this;
	};

	pointTable& operator=(const pointTable &arg)
	{
		x = arg.x;
		y = arg.y;
		z = arg.z;
		grayValue = arg.grayValue;
		costToStart = arg.costToStart;
		costToEnd = arg.costToEnd;
		positionToPrevious = arg.positionToPrevious;
		positionVector = arg.positionVector;
		//deep copy
		//here recursion, need to control the end of the conditions
		if (arg.fatherPoint.get())
			fatherPoint = arg.fatherPoint;/////////////////////
			
		return *this;
	};

	bool operator==(const pointTable &arg)
	{
		return (x == arg.x && y == arg.y && z == arg.z);
	};

};

class astar
{
public:
	astar(std::vector<double>, DVolume&);
	~astar();
	std::vector<pointTable> pathTable;
	std::vector<std::vector<std::shared_ptr<pointTable>>> allOpenTable;
	void search();

private:
	double grayMax, grayMin;
	bool searchFlag = false;
	IVolume dataFlag;
	DVolume sourceData;
	pointTable startPoint, endPoint;
	std::shared_ptr<pointTable> currentPoint;
	std::vector<std::shared_ptr<pointTable>>  allNeighborTable, currentOpenTable;
	std::vector<std::shared_ptr<pointTable>>::iterator currPointIte, endPointFinder;

	bool eveluationRange(pointTable&);
	double normaValue(double);
	double calcCostToStart(pointTable&);
	double calcCostToEnd(pointTable&);//useless!!!
	void getGrayRange();
	void getNeighborToOpenTable();
	void searchOpenTable();
	//void search();///////////////////////////////////////
};
