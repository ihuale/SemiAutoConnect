#ifndef MYHISTOGRAM_H
#define MYHISTOGRAM_H

#include <QtWidgets/QDialog>
#include "ui_MyHistogram.h"

#include "volume.h"
#include "basetypes.h"

class MyHistogram : public QDialog
{
	Q_OBJECT

public:
	MyHistogram(SVolume&,QWidget *parent = 0);
	~MyHistogram();

private:
	Ui::MyHistogramClass ui;

	VecXd normArry, grayArry;
	unsigned short grayMin, grayMax, grayRange;

	unsigned short Round(unsigned short);

	void paintEvent(QPaintEvent *);
	unsigned short linnerExpansion(unsigned short);

};

#endif // MYHISTOGRAM_H
