/********************************************************************************
** Form generated from reading UI file 'calculatejSHMSL.ui'
**
** Created by: Qt User Interface Compiler version 5.14.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef CALCULATEJSHMSL_H
#define CALCULATEJSHMSL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableWidget>

QT_BEGIN_NAMESPACE

class Ui_calculateClass
{
public:
    QGroupBox *groupBox;
    QTableWidget *tableWidget;

    void setupUi(QDialog *calculateClass)
    {
        if (calculateClass->objectName().isEmpty())
            calculateClass->setObjectName(QString::fromUtf8("calculateClass"));
        calculateClass->resize(1100, 1000);
        calculateClass->setMinimumSize(QSize(1100, 1000));
        calculateClass->setMaximumSize(QSize(1100, 1000));
        groupBox = new QGroupBox(calculateClass);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(200, 50, 691, 901));
        QFont font;
        font.setFamily(QString::fromUtf8("\345\276\256\350\275\257\351\233\205\351\273\221"));
        font.setPointSize(11);
        groupBox->setFont(font);
        groupBox->setLayoutDirection(Qt::LeftToRight);
        groupBox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);
        tableWidget = new QTableWidget(groupBox);
        if (tableWidget->columnCount() < 6)
            tableWidget->setColumnCount(6);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(4, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        tableWidget->setHorizontalHeaderItem(5, __qtablewidgetitem5);
        if (tableWidget->rowCount() < 27)
            tableWidget->setRowCount(27);
        QTableWidgetItem *__qtablewidgetitem6 = new QTableWidgetItem();
        __qtablewidgetitem6->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(0, __qtablewidgetitem6);
        QTableWidgetItem *__qtablewidgetitem7 = new QTableWidgetItem();
        __qtablewidgetitem7->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(1, __qtablewidgetitem7);
        QTableWidgetItem *__qtablewidgetitem8 = new QTableWidgetItem();
        __qtablewidgetitem8->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(2, __qtablewidgetitem8);
        QTableWidgetItem *__qtablewidgetitem9 = new QTableWidgetItem();
        __qtablewidgetitem9->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(3, __qtablewidgetitem9);
        QTableWidgetItem *__qtablewidgetitem10 = new QTableWidgetItem();
        __qtablewidgetitem10->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(4, __qtablewidgetitem10);
        QTableWidgetItem *__qtablewidgetitem11 = new QTableWidgetItem();
        __qtablewidgetitem11->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(5, __qtablewidgetitem11);
        QTableWidgetItem *__qtablewidgetitem12 = new QTableWidgetItem();
        __qtablewidgetitem12->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(6, __qtablewidgetitem12);
        QTableWidgetItem *__qtablewidgetitem13 = new QTableWidgetItem();
        __qtablewidgetitem13->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(7, __qtablewidgetitem13);
        QTableWidgetItem *__qtablewidgetitem14 = new QTableWidgetItem();
        __qtablewidgetitem14->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(8, __qtablewidgetitem14);
        QTableWidgetItem *__qtablewidgetitem15 = new QTableWidgetItem();
        __qtablewidgetitem15->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(9, __qtablewidgetitem15);
        QTableWidgetItem *__qtablewidgetitem16 = new QTableWidgetItem();
        __qtablewidgetitem16->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(10, __qtablewidgetitem16);
        QTableWidgetItem *__qtablewidgetitem17 = new QTableWidgetItem();
        __qtablewidgetitem17->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(11, __qtablewidgetitem17);
        QTableWidgetItem *__qtablewidgetitem18 = new QTableWidgetItem();
        __qtablewidgetitem18->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(12, __qtablewidgetitem18);
        QTableWidgetItem *__qtablewidgetitem19 = new QTableWidgetItem();
        __qtablewidgetitem19->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(13, __qtablewidgetitem19);
        QTableWidgetItem *__qtablewidgetitem20 = new QTableWidgetItem();
        __qtablewidgetitem20->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(14, __qtablewidgetitem20);
        QTableWidgetItem *__qtablewidgetitem21 = new QTableWidgetItem();
        __qtablewidgetitem21->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(15, __qtablewidgetitem21);
        QTableWidgetItem *__qtablewidgetitem22 = new QTableWidgetItem();
        __qtablewidgetitem22->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(16, __qtablewidgetitem22);
        QTableWidgetItem *__qtablewidgetitem23 = new QTableWidgetItem();
        __qtablewidgetitem23->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(17, __qtablewidgetitem23);
        QTableWidgetItem *__qtablewidgetitem24 = new QTableWidgetItem();
        __qtablewidgetitem24->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(18, __qtablewidgetitem24);
        QTableWidgetItem *__qtablewidgetitem25 = new QTableWidgetItem();
        __qtablewidgetitem25->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(19, __qtablewidgetitem25);
        QTableWidgetItem *__qtablewidgetitem26 = new QTableWidgetItem();
        __qtablewidgetitem26->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(20, __qtablewidgetitem26);
        QTableWidgetItem *__qtablewidgetitem27 = new QTableWidgetItem();
        __qtablewidgetitem27->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(21, __qtablewidgetitem27);
        QTableWidgetItem *__qtablewidgetitem28 = new QTableWidgetItem();
        __qtablewidgetitem28->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(22, __qtablewidgetitem28);
        QTableWidgetItem *__qtablewidgetitem29 = new QTableWidgetItem();
        __qtablewidgetitem29->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(23, __qtablewidgetitem29);
        QTableWidgetItem *__qtablewidgetitem30 = new QTableWidgetItem();
        __qtablewidgetitem30->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(24, __qtablewidgetitem30);
        QTableWidgetItem *__qtablewidgetitem31 = new QTableWidgetItem();
        __qtablewidgetitem31->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(25, __qtablewidgetitem31);
        QTableWidgetItem *__qtablewidgetitem32 = new QTableWidgetItem();
        __qtablewidgetitem32->setTextAlignment(Qt::AlignLeading|Qt::AlignVCenter);
        tableWidget->setVerticalHeaderItem(26, __qtablewidgetitem32);
        tableWidget->setObjectName(QString::fromUtf8("tableWidget"));
        tableWidget->setGeometry(QRect(20, 40, 649, 851));
        tableWidget->setRowCount(27);
        tableWidget->setColumnCount(6);

        retranslateUi(calculateClass);

        QMetaObject::connectSlotsByName(calculateClass);
    } // setupUi

    void retranslateUi(QDialog *calculateClass)
    {
        calculateClass->setWindowTitle(QCoreApplication::translate("calculateClass", "calculate", nullptr));
        groupBox->setTitle(QCoreApplication::translate("calculateClass", "\350\276\223\345\207\272\347\237\251\351\230\265", nullptr));
        QTableWidgetItem *___qtablewidgetitem = tableWidget->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("calculateClass", "Y", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = tableWidget->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("calculateClass", "Mz", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = tableWidget->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("calculateClass", "Mx", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = tableWidget->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("calculateClass", "X", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = tableWidget->horizontalHeaderItem(4);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("calculateClass", "Z", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = tableWidget->horizontalHeaderItem(5);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("calculateClass", "My", nullptr));
        QTableWidgetItem *___qtablewidgetitem6 = tableWidget->verticalHeaderItem(0);
        ___qtablewidgetitem6->setText(QCoreApplication::translate("calculateClass", "Y", nullptr));
        QTableWidgetItem *___qtablewidgetitem7 = tableWidget->verticalHeaderItem(1);
        ___qtablewidgetitem7->setText(QCoreApplication::translate("calculateClass", "Mz", nullptr));
        QTableWidgetItem *___qtablewidgetitem8 = tableWidget->verticalHeaderItem(2);
        ___qtablewidgetitem8->setText(QCoreApplication::translate("calculateClass", "Mx", nullptr));
        QTableWidgetItem *___qtablewidgetitem9 = tableWidget->verticalHeaderItem(3);
        ___qtablewidgetitem9->setText(QCoreApplication::translate("calculateClass", "X", nullptr));
        QTableWidgetItem *___qtablewidgetitem10 = tableWidget->verticalHeaderItem(4);
        ___qtablewidgetitem10->setText(QCoreApplication::translate("calculateClass", "Z", nullptr));
        QTableWidgetItem *___qtablewidgetitem11 = tableWidget->verticalHeaderItem(5);
        ___qtablewidgetitem11->setText(QCoreApplication::translate("calculateClass", "My", nullptr));
        QTableWidgetItem *___qtablewidgetitem12 = tableWidget->verticalHeaderItem(6);
        ___qtablewidgetitem12->setText(QCoreApplication::translate("calculateClass", "Y.Y", nullptr));
        QTableWidgetItem *___qtablewidgetitem13 = tableWidget->verticalHeaderItem(7);
        ___qtablewidgetitem13->setText(QCoreApplication::translate("calculateClass", "Mz.Mz", nullptr));
        QTableWidgetItem *___qtablewidgetitem14 = tableWidget->verticalHeaderItem(8);
        ___qtablewidgetitem14->setText(QCoreApplication::translate("calculateClass", "Mx.Mx", nullptr));
        QTableWidgetItem *___qtablewidgetitem15 = tableWidget->verticalHeaderItem(9);
        ___qtablewidgetitem15->setText(QCoreApplication::translate("calculateClass", "X.X", nullptr));
        QTableWidgetItem *___qtablewidgetitem16 = tableWidget->verticalHeaderItem(10);
        ___qtablewidgetitem16->setText(QCoreApplication::translate("calculateClass", "Z.Z", nullptr));
        QTableWidgetItem *___qtablewidgetitem17 = tableWidget->verticalHeaderItem(11);
        ___qtablewidgetitem17->setText(QCoreApplication::translate("calculateClass", "My.My", nullptr));
        QTableWidgetItem *___qtablewidgetitem18 = tableWidget->verticalHeaderItem(12);
        ___qtablewidgetitem18->setText(QCoreApplication::translate("calculateClass", "Y.Mz", nullptr));
        QTableWidgetItem *___qtablewidgetitem19 = tableWidget->verticalHeaderItem(13);
        ___qtablewidgetitem19->setText(QCoreApplication::translate("calculateClass", "Y.Mx", nullptr));
        QTableWidgetItem *___qtablewidgetitem20 = tableWidget->verticalHeaderItem(14);
        ___qtablewidgetitem20->setText(QCoreApplication::translate("calculateClass", "Y.X", nullptr));
        QTableWidgetItem *___qtablewidgetitem21 = tableWidget->verticalHeaderItem(15);
        ___qtablewidgetitem21->setText(QCoreApplication::translate("calculateClass", "Y.Z", nullptr));
        QTableWidgetItem *___qtablewidgetitem22 = tableWidget->verticalHeaderItem(16);
        ___qtablewidgetitem22->setText(QCoreApplication::translate("calculateClass", "Y.My", nullptr));
        QTableWidgetItem *___qtablewidgetitem23 = tableWidget->verticalHeaderItem(17);
        ___qtablewidgetitem23->setText(QCoreApplication::translate("calculateClass", "Mz.Mx", nullptr));
        QTableWidgetItem *___qtablewidgetitem24 = tableWidget->verticalHeaderItem(18);
        ___qtablewidgetitem24->setText(QCoreApplication::translate("calculateClass", "Mz.X", nullptr));
        QTableWidgetItem *___qtablewidgetitem25 = tableWidget->verticalHeaderItem(19);
        ___qtablewidgetitem25->setText(QCoreApplication::translate("calculateClass", "Mz.Z", nullptr));
        QTableWidgetItem *___qtablewidgetitem26 = tableWidget->verticalHeaderItem(20);
        ___qtablewidgetitem26->setText(QCoreApplication::translate("calculateClass", "Mz.My", nullptr));
        QTableWidgetItem *___qtablewidgetitem27 = tableWidget->verticalHeaderItem(21);
        ___qtablewidgetitem27->setText(QCoreApplication::translate("calculateClass", "Mx.X", nullptr));
        QTableWidgetItem *___qtablewidgetitem28 = tableWidget->verticalHeaderItem(22);
        ___qtablewidgetitem28->setText(QCoreApplication::translate("calculateClass", "Mx.Z", nullptr));
        QTableWidgetItem *___qtablewidgetitem29 = tableWidget->verticalHeaderItem(23);
        ___qtablewidgetitem29->setText(QCoreApplication::translate("calculateClass", "Mx.My", nullptr));
        QTableWidgetItem *___qtablewidgetitem30 = tableWidget->verticalHeaderItem(24);
        ___qtablewidgetitem30->setText(QCoreApplication::translate("calculateClass", "X.Z", nullptr));
        QTableWidgetItem *___qtablewidgetitem31 = tableWidget->verticalHeaderItem(25);
        ___qtablewidgetitem31->setText(QCoreApplication::translate("calculateClass", "X.My", nullptr));
        QTableWidgetItem *___qtablewidgetitem32 = tableWidget->verticalHeaderItem(26);
        ___qtablewidgetitem32->setText(QCoreApplication::translate("calculateClass", "Z.My", nullptr));
    } // retranslateUi

};

namespace Ui {
    class calculateClass: public Ui_calculateClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // CALCULATEJSHMSL_H
