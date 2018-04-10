/********************************************************************************
** Form generated from reading UI file 'MyHistogram.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYHISTOGRAM_H
#define UI_MYHISTOGRAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_MyHistogramClass
{
public:
    QLabel *label;

    void setupUi(QDialog *MyHistogramClass)
    {
        if (MyHistogramClass->objectName().isEmpty())
            MyHistogramClass->setObjectName(QStringLiteral("MyHistogramClass"));
        MyHistogramClass->resize(250, 140);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MyHistogramClass->sizePolicy().hasHeightForWidth());
        MyHistogramClass->setSizePolicy(sizePolicy);
        MyHistogramClass->setMinimumSize(QSize(250, 140));
        MyHistogramClass->setMaximumSize(QSize(1000, 140));
        label = new QLabel(MyHistogramClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(0, 0, 250, 140));

        retranslateUi(MyHistogramClass);

        QMetaObject::connectSlotsByName(MyHistogramClass);
    } // setupUi

    void retranslateUi(QDialog *MyHistogramClass)
    {
        MyHistogramClass->setWindowTitle(QApplication::translate("MyHistogramClass", "MyHistogram", 0));
        label->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MyHistogramClass: public Ui_MyHistogramClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYHISTOGRAM_H
