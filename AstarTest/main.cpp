#include "SemiAutoConnect.h"
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

	/////////////////////////////////////Get Data////////////////////////////////////////
	time_t star1, end1;
	
	star1 = clock();
	//data_Matrix.SetSize(62, 44, 54);
	data_Matrix.SetSize(804, 428, 196);
	data_Matrix.SetZero();

	//In3dMatFileName("origImg.txt");
	In3dMatFileName("test11.txt");

	double tttt = data_Matrix.GetPixel(0, 0, 0);

	//std::cout << "This is Mat(0,0,0):\n" << data_Matrix.operator()(0, 0, 0) << std::endl;
	std::cout << "This is Mat(0,0,0):\n" << tttt << std::endl;
	std::cout << "This is Mat(0,1,0):\n" << data_Matrix.operator()(0, 1, 0) << std::endl;
	std::cout << "This is Mat(61,42,53)\n" << data_Matrix.operator()(61, 42, 53) << std::endl;
	std::cout << "This is Mat(61,43,53)\n" << data_Matrix.operator()(61, 43, 53) << std::endl;
	end1 = clock();
	std::cout << "Read data completed! " << difftime(end1, star1) << std::endl;

	//////////////////////////////////////Data Test//////////////////////////////////////////
	//std::vector<double> arg1 = { 56, 14, 33, 14, 33, 29 };
	//std::vector<double> arg1 = { 56, 21, 32, 14, 33, 29 };//wrong start point
	//std::vector<double> arg1 = { 56, 14, 33, 47, 10, 31 };
	//std::vector<double> arg1 = { 56, 14, 33, 50, 31, 32 };

	//std::vector<double> arg1 = { 474, 262, 188, 295, 186, 139 };
	std::vector<double> arg1 = { 559, 220, 186, 354, 130, 153 };

	SemiAutoConnect w2(arg1, data_Matrix);
	w2.normRange = 0.30;
	w2.search();
	end1 = clock();
	std::cout << "search data completed! " << difftime(end1, star1) << std::endl;

	//////////////////////////////////////Data Write//////////////////////////////////////////
	
	string str1 = "pathTable.swc";
	int id = 0;
	ofstream Ofile1(str1);
	if (Ofile1.is_open()){

		for (; id < w2.pathTable.size(); ++id)
		{
			Ofile1 << id << " " << 2 << " " << w2.pathTable[id](0) << " " << w2.pathTable[id](1) << " " << w2.pathTable[id](2) << " " << 1 << " " << -1 << "\n";
		}
	}
	
	int countNUM = 0;
	id = 0;
	for (; id < w2.allOpenTable.size(); ++id)
		countNUM += w2.allOpenTable[id].size();
	std::cout << "This is size :" << countNUM << std::endl;
	
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
	end1 = clock();
	std::cout << "Write data completed! " << difftime(end1, star1) << std::endl;
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
			data_Matrix(m, j, n) = atof(temp.c_str());
			double test1111 = atof(temp.c_str());
			//data_Matrix[n](j, m) = atof(temp.c_str());
			if (m < 803)////////////////////////////////////////////////////
				++m;
			else
				m = 0;
		}
	}
}

void In3dMatFileName(std::string filename)
{
	cout << "Input 3d-Mat File name:\n" << filename << endl;
	ifstream infile(filename.c_str());
	string temp;
	//cin >> filename;
	
	cout << "Please wait......" << endl;

	vector<string>vec;
	int n = 0;

	while (getline(infile, temp))
	{
		if (temp == "Next")
		{
			In3dMatData(vec, n);
			vec.clear();
			std::cout << n << std::endl;
			n++;
		}
		else
			vec.push_back(temp);
	}
}
