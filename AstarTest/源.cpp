#include "basetypes.h"
#include "volume.h"

struct pointTable
{
	double x, y, z, costToStart, costToEnd;
	double positionToPrevious;//1 is equal level,sqrt(2) is intermediate level,sqrt(3) is vertex
	double getTotalCost(){ return (calcCostToStart() + costToEnd); };
	Vec3d positionVector;
	pointTable *fatherPoint;

	pointTable()
	{
		x = 0; y = 0; z = 0;
		costToStart = 0;
		costToEnd = 0;
		positionToPrevious = 0;
	};

	pointTable(double arg1, double arg2, double arg3)
	{
		x = arg1; y = arg2; z = arg3;
		costToStart = 0;
		costToEnd = 0;
		positionToPrevious = 0;
	};

	pointTable operator-(const pointTable &arg)const
	{
		pointTable diff;
		diff.x = x - arg.x;
		diff.y = y - arg.y;
		diff.z = z - arg.z;
		return diff;
	};

	pointTable operator+(pointTable &arg)const
	{
		pointTable sum;
		sum.x = x + arg.x;
		sum.y = y + arg.y;
		sum.z = z + arg.z;
		sum.costToEnd = arg.costToEnd + (positionVector[0] + positionVector[1] + positionVector[2]);
		sum.positionToPrevious = this->positionToPrevious;

		/*sum.costToStart = arg.costToStart +
		(positionVector[0] * positionVector[1] * positionVector[2])
		*sqrt(positionVector[0] + positionVector[1] + positionVector[2]);//BUG*/
		sum.costToStart = arg.costToStart + sqrt(positionVector[0] + positionVector[1] + positionVector[2]);/////////////////////////////

		sum.fatherPoint = &arg;/////////////////////////////BUG~!!!!!!!!!!!!//////////////////////////

		return sum;
	};

	pointTable& operator=(const pointTable &arg)
	{
		x = arg.x;//////////////////////BUG~!!!!!!!!!!!!////////////////////////////
		y = arg.y;
		z = arg.z;
		costToStart = arg.costToStart;
		costToEnd = arg.costToEnd;
		positionToPrevious = arg.positionToPrevious;
		positionVector = arg.positionVector;
		//deep copy
		//here recursion, need to control the end of the conditions
		if (arg.fatherPoint != NULL)
			*fatherPoint = *(arg.fatherPoint);

		return *this;
	};

	bool operator==(const pointTable &arg)
	{
		return (x == arg.x && y == arg.y && z == arg.z);
	};

	double calcCostToStart()
	{
		double result = 0;
		result = fatherPoint->costToStart + positionToPrevious;
		return result;
	};
};
