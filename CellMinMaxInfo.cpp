#include "CellMinMaxInfo.h"
#include <QGridLayout>
#include <QGroupBox>

CellMinMaxInfo::CellMinMaxInfo(QWidget *parent) :
    QWidget(parent)
{
    groupBox = new QGroupBox(this);
    groupBox->setTitle(tr("ͳ����Ϣ"));

    gridLayout = new QGridLayout(groupBox);

    labelMaxCellVol = new QLabel("��о����ѹ:",this);
    labelMaxVPos = new QLabel("��о����ѹλ��:",this);
    labelMinCellVol = new QLabel("��о��С��ѹ:",this);
    labelMinVPos = new QLabel("��о��С��ѹλ��:",this);
    labelDltV = new QLabel("��ѹ��:",this);

    labelMaxCellTemp = new QLabel("����¶�:",this);
    labelMaxTPos = new QLabel("����¶�λ��:",this);
    labelMinCellTemp = new QLabel("��С�¶�:",this);
    labelMinTPos = new QLabel("��С�¶�λ��:",this);
    labelDltT = new QLabel("�¶Ȳ�:",this);
    labelCycTimes = new QLabel("ѭ������:",this);
    labelCycTimes->setAlignment(Qt::AlignCenter);

    lineEditMaxCellVol = new QLineEdit(this);
    lineEditMaxVPos = new QLineEdit(this);
    lineEditMinCellVol = new QLineEdit(this);
    lineEditMinVPos = new QLineEdit(this);
    lineEditDltV = new QLineEdit(this);
    lineEditMaxCellTemp = new QLineEdit(this);
    lineEditMaxTPos = new QLineEdit(this);
    lineEditMinCellTemp = new QLineEdit(this);
    lineEditMinTPos = new QLineEdit(this);
    lineEditDltT = new QLineEdit(this);
    lineEditCycTimes = new QLineEdit(this);

    gridLayout->addWidget(labelMaxCellVol,0,0,1,1);
    gridLayout->addWidget(labelMaxVPos,0,1,1,1);
    gridLayout->addWidget(labelMinCellVol,0,2,1,1);
    gridLayout->addWidget(labelMinVPos,0,3,1,1);
    gridLayout->addWidget(labelDltV,0,4,1,1);
    gridLayout->addWidget(labelMaxCellTemp,2,0,1,1);
    gridLayout->addWidget(labelMaxTPos,2,1,1,1);
    gridLayout->addWidget(labelMinCellTemp,2,2,1,1);
    gridLayout->addWidget(labelMinTPos,2,3,1,1);
    gridLayout->addWidget(labelDltT,2,4,1,1);
    gridLayout->addWidget(labelCycTimes,4,0,1,1);

    gridLayout->addWidget(lineEditMaxCellVol,1,0,1,1);
    gridLayout->addWidget(lineEditMaxVPos,1,1,1,1);
    gridLayout->addWidget(lineEditMinCellVol,1,2,1,1);
    gridLayout->addWidget(lineEditMinVPos,1,3,1,1);
    gridLayout->addWidget(lineEditDltV,1,4,1,1);
    gridLayout->addWidget(lineEditMaxCellTemp,3,0,1,1);
    gridLayout->addWidget(lineEditMaxTPos,3,1,1,1);
    gridLayout->addWidget(lineEditMinCellTemp,3,2,1,1);
    gridLayout->addWidget(lineEditMinTPos,3,3,1,1);
    gridLayout->addWidget(lineEditDltT,3,4,1,1);
    gridLayout->addWidget(lineEditCycTimes,4,1,1,2);

    lineEditMaxCellVol->setReadOnly(true);
    lineEditMaxVPos->setReadOnly(true);
    lineEditMinCellVol->setReadOnly(true);
    lineEditMinVPos->setReadOnly(true);
    lineEditDltV->setReadOnly(true);
    lineEditMaxCellTemp->setReadOnly(true);
    lineEditMaxTPos->setReadOnly(true);
    lineEditMinCellTemp->setReadOnly(true);
    lineEditMinTPos->setReadOnly(true);
    lineEditDltT->setReadOnly(true);
    lineEditCycTimes->setReadOnly(true);

    gridLayout->setSpacing(5);
    groupBox->setLayout(gridLayout);
    resize(600,200);
}
