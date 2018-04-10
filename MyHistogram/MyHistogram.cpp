#include "MyHistogram.h"
#include <QPainter>

#include <QDebug>

MyHistogram::MyHistogram(SVolume &oriImg,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	if (oriImg.IsEmpty())
	{
		printf("ERROR in MyHistogram arg");
		return;
	}

	unsigned short *temGray1 = &oriImg.GetPixel(0, 0, 0);
	grayMin = *temGray1;
	grayMax = *temGray1;

	for (int i = 0; i < oriImg.x()*oriImg.y()*oriImg.z(); ++i)//get max and min gray value
	{
		if (*temGray1 > grayMax)
			grayMax = *temGray1;
		else if (*temGray1 < grayMin)
			grayMin = *temGray1;

		++temGray1;
	}

	grayRange = grayMax - grayMin + 1;
	grayArry.resize((int)(grayRange));//the first value of grayArry is the count number of min gray
	grayArry.setZero();//last is max gray

	unsigned short *temGray2 = &oriImg.GetPixel(0, 0, 0);

	for (int i = 0; i < oriImg.x()*oriImg.y()*oriImg.z(); ++i)//get the corresponding number of each gray value
	{
		++grayArry[(int)(*temGray2 - grayMin)];//has been sorted //no need -1
		//qDebug() << grayArry[(int)(*temGray2 - grayMin)];
		++temGray2;
	}

	//grayArry /= grayArry.maxCoeff();
	int maxCount = grayArry.maxCoeff();
	qDebug() << "This is maxCount " << maxCount;//for test

	grayArry /= maxCount;
	grayArry *= 100;

	update();//ready to paint!
}

MyHistogram::~MyHistogram()
{

}

unsigned short MyHistogram::Round(unsigned short arg)
{
	return int(arg + 0.5 * (arg > 0 ? 1 : -1));
}

unsigned short linnerExpansion(unsigned short)
{
	unsigned short result=0;
	return result;
}

void MyHistogram::paintEvent(QPaintEvent *)
{
	QPainter _painter(this);
	_painter.setBrush(QBrush(QColor(255, 255, 255)));
	this->resize(40 + grayRange, this->height());
	_painter.drawRect(0, 0, this->width(), this->height());

	QImage* _histimage = new QImage(this->width(), this->height(), QImage::Format_RGB888);
	_histimage->fill(qRgb(255, 255, 255));

	_painter.drawLine(20, 20, 20, 120);//x axis
	_painter.drawLine(20, 20, 19, 22);
	_painter.drawLine(20, 20, 21, 22);

	_painter.drawLine(20, 120, 20 + grayRange, 120);//y axis
	_painter.drawLine(245, 120, 20 + grayRange, 119);
	_painter.drawLine(245, 120, 20 + grayRange, 121);

	_painter.drawText(QPointF(0, 20), QString::number(grayArry.maxCoeff()));//draw text
	_painter.drawText(QPointF(18, 130), QString::number(grayMin));
	_painter.drawText(QPointF(18 + grayRange, 130), QString::number(grayMax));



	int xStep = 21;
	for (int i = 1; i < grayArry.size(); ++i)//i=1,no min
	{
		_painter.drawLine(xStep, 120, xStep, 120 - (int)(grayArry[i]));
		++xStep;
	}

}