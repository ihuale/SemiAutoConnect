#include "MyHistogram.h"
#include <QtWidgets/QApplication>

#include "volume.h"

//int main(int argc, char *argv[])
//{
//	QApplication a(argc, argv);
//	MyHistogram w;
//	w.show();
//	return a.exec();
//}
#include <string>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <vector>
#include <iostream>
#include <Eigen/Dense>

#include <QDebug>

using namespace std;

SVolume data_Matrix;

void In3dMatFileName(std::string);
void In3dMatData(const vector<std::string> &, int);

int main(int argc, char *argv[])
{

	/////////////////////////////////////Get Data////////////////////////////////////////
	time_t star1, end1;

	star1 = clock();
	data_Matrix.SetSize(62, 44, 54);
	//data_Matrix.SetSize(204, 131, 1);
	//data_Matrix.SetSize(804, 428, 196);
	data_Matrix.SetZero();

	In3dMatFileName("origImg.txt");
	//In3dMatFileName("test222.txt");
	//In3dMatFileName("test11.txt");

	std::cout << "This is Mat(0,0,0):\n" << data_Matrix.operator()(0, 0, 0) << std::endl;
	std::cout << "This is Mat(0,1,0):\n" << data_Matrix.operator()(0, 1, 0) << std::endl;
	//std::cout << "This is Mat(61,42,53)\n" << data_Matrix.operator()(61, 42, 53) << std::endl;
	//std::cout << "This is Mat(61,43,53)\n" << data_Matrix.operator()(61, 43, 53) << std::endl;
	end1 = clock();
	std::cout << "Read data completed! " << difftime(end1, star1) << std::endl;

	QApplication a(argc, argv);
	MyHistogram w(data_Matrix);
	w.show();
	end1 = clock();
	std::cout << "Show window completed! " << difftime(end1, star1) << std::endl;
	return a.exec();

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
			//data_Matrix(m, j, n) = atof(temp.c_str());
			data_Matrix(j, m, n) = atof(temp.c_str());
			if (m < 43)
			//if (m < 803)
				m++;
			else
				m = 0;
		}
	}
}

void In3dMatFileName(std::string str)
{
	cout << "Input 3d-Mat File name:\n";
	string filename = str, temp;
	//cin >> filename;
	ifstream infile(filename.c_str());
	cout << str << endl;
	cout << "Please wait......" << endl;

	vector<string>vec;
	int n = 0;

	while (getline(infile, temp))
	{
		if (temp == "Next")
		{
			In3dMatData(vec, n);
			vec.clear();
			qDebug() << n;
			n++;
		}
		else
			vec.push_back(temp);
	}
}

