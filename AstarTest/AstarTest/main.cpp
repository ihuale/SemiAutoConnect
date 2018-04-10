#include "astar.h"
#include "volume.h"
#include <string>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <vector>

#include <iostream>
#include <Eigen/Dense>
using namespace std;

DVolume data_Matrix;

void In3dMatFileName(std::string);
void In3dMatData(const vector<std::string> &, int);

int main()
{

	/////////////////////////////////////数据获取////////////////////////////////////////

	data_Matrix.SetSize(62, 44, 54);
	data_Matrix.SetZero();
	//data_Matrix.resize(54);

	In3dMatFileName("origImg.txt");
	std::cout << "This is Mat(0,0,0):\n" << data_Matrix.operator()(0, 0, 0) << std::endl;
	//std::cout << "This is Mat(0,0,0):\n" << data_Matrix[0](0, 0) << std::endl;
	std::cout << "This is Mat(0,1,0):\n" << data_Matrix.operator()(0, 1, 0) << std::endl;
	//std::cout << "This is Mat(0,1,0):\n" << data_Matrix[0](0, 1) << std::endl;
	std::cout << "This is Mat(61,42,53)\n" << data_Matrix.operator()(61, 42, 53) << std::endl;
	//std::cout << "This is Mat(61,42,53)\n" << data_Matrix[53](61, 42) << std::endl;
	std::cout << "This is Mat(61,43,53)\n" << data_Matrix.operator()(61, 43, 53) << std::endl;
	//std::cout << "This is Mat(61,43,53)\n" << data_Matrix[53](61, 43) << std::endl;

	//////////////////////////////////////Data Test//////////////////////////////////////////
	std::vector<double> arg1 = { 56, 14, 33, 14, 33, 29 };
	//std::vector<double> arg1 = { 56, 14, 33, 47, 10, 31 };
	//std::vector<double> arg1 = { 56, 14, 33, 50, 31, 32 };
	astar w2(arg1, data_Matrix);
	w2.search();

	//////////////////////////////////////Data Write//////////////////////////////////////////
	string str1 = "pathTable.swc";
	int id = 1;
	ofstream Ofile1(str1);
	if (Ofile1.is_open()){

		for (; id < w2.pathTable.size(); ++id)
		{
			Ofile1 << id << " " << 2 << " " << w2.pathTable[id - 1].x << " " << w2.pathTable[id - 1].y << " " << w2.pathTable[id - 1].z << " " << 1 << " " << -1 << "\n";
		}
	}

	string str2 = "allOpenTable.swc";
	id = 0;
	ofstream Ofile2(str2);
	if (Ofile2.is_open())
	{
		std::vector<std::vector<std::shared_ptr<pointTable>>>::iterator iteWriter = w2.allOpenTable.begin();
		for (; id < w2.allOpenTable.size(); ++id)
		{
			for (int j = 0; j < w2.allOpenTable[id].size(); ++j)
				Ofile2 << (id*j + j) << " " << 2 << " " << w2.allOpenTable[id][j]->x << " " << w2.allOpenTable[id][j]->y << " " << w2.allOpenTable[id][j]->z << " " << 1 << " " << -1 << "\n";
		}
	}
	//////////////////////////////////////////////////////////////////////////////////////////
	
	system("pause");
	return 0;
}

void In3dMatData(const vector<std::string> &vec, int n)
{

	for (int j = 0; j < vec.size(); j++)
	{
		string temp;
		stringstream ss(vec[j]);
		int m = 0;
		while (ss >> temp)
		{
			data_Matrix(j, m, n) = atof(temp.c_str());
			//data_Matrix[n](j, m) = atof(temp.c_str());
			if (m < 43)
				m++;
			else
				m = 0;
		}
	}

	//cout << data_Matrix[n] << endl;
}

void In3dMatFileName(std::string str)
{
	cout << "Input 3d-Mat File name:\n";
	string filename = str, temp;
	//cin >> filename;
	ifstream infile(filename.c_str());
	cout << str << endl;
	cout << "Please wait......" << endl;

	vector<string>vec[100];
	int n = 0;

	while (getline(infile, temp))
	{
		if (temp == "Next")
		{
			In3dMatData(vec[n], n);
			n++;
		}
		else
			vec[n].push_back(temp);
	}
}
