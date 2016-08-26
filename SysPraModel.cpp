#include "SysPraModel.h"
#include "ui_SysPraModel.h"
#include <QStandardItemModel>
#include "DataProcess.h"
#include "Header.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>

SysPraModel::SysPraModel(DataProcess *datpress,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SysPraModel),
    m_dataProcess(datpress)
{
    ui->setupUi(this);
    m_timer = new QTimer(this);
    m_timer->setInterval(1000);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(onTime()));
    m_timer->start();

    m_modelSysPra = new QStandardItemModel();
    m_can = CanBus::getInstance();

    ui->pushButtonWritePra->setEnabled(false);
    InitUI();
}

SysPraModel::~SysPraModel()
{
    delete ui;
}

void SysPraModel::InitUI(void)
{
    //ϵͳ����tableView������
    m_modelSysPra->setColumnCount(3);
    m_modelSysPra->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("����"));
    m_modelSysPra->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("ֵ"));
    m_modelSysPra->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("��λ"));
    ui->tableViewSysPra->setModel(m_modelSysPra);
    ui->tableViewSysPra->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    //����������Ӧ���
    ui->tableViewSysPra->horizontalHeader()->setResizeMode(QHeaderView::ResizeToContents);
    //����ѡ��ʱΪ����ѡ��
    ui->tableViewSysPra->setSelectionBehavior(QAbstractItemView::SelectRows);
    //���ñ��ĵ�ԪΪֻ�����ԣ������ܱ༭
    ui->tableViewSysPra->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableViewSysPra->horizontalHeader()->setStretchLastSection(true);

    m_modelSysPra->setItem(0,0,new QStandardItem("�����ѹһ�������"));
    m_modelSysPra->item(0,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(0,2,new QStandardItem("mV"));
    m_modelSysPra->item(0,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(1,0,new QStandardItem("�����ѹ���������"));
    m_modelSysPra->item(1,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(1,2,new QStandardItem("mV"));
    m_modelSysPra->item(1,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(2,0,new QStandardItem("����Ƿѹһ�������"));
    m_modelSysPra->item(2,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(2,2,new QStandardItem("mV"));
    m_modelSysPra->item(2,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(3,0,new QStandardItem("����Ƿѹ���������"));
    m_modelSysPra->item(3,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(3,2,new QStandardItem("mV"));
    m_modelSysPra->item(3,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(4,0,new QStandardItem("��ذ�������һ�������"));
    m_modelSysPra->item(4,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(4,2,new QStandardItem("��"));
    m_modelSysPra->item(4,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(5,0,new QStandardItem("��ذ������¶��������"));
    m_modelSysPra->item(5,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(5,2,new QStandardItem("��"));
    m_modelSysPra->item(5,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(6,0,new QStandardItem("��ذ�������һ�������"));
    m_modelSysPra->item(6,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(6,2,new QStandardItem("��"));
    m_modelSysPra->item(6,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(7,0,new QStandardItem("��ذ������¶��������"));
    m_modelSysPra->item(7,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(7,2,new QStandardItem("��"));
    m_modelSysPra->item(7,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(8,0,new QStandardItem("��ذ��ŵ����һ�������"));
    m_modelSysPra->item(8,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(8,2,new QStandardItem("��"));
    m_modelSysPra->item(8,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(9,0,new QStandardItem("��ذ��ŵ���¶��������"));
    m_modelSysPra->item(9,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(9,2,new QStandardItem("��"));
    m_modelSysPra->item(9,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(10,0,new QStandardItem("��ذ��ŵ����һ�������"));
    m_modelSysPra->item(10,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(10,2,new QStandardItem("��"));
    m_modelSysPra->item(10,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(11,0,new QStandardItem("��ذ��ŵ���¶��������"));
    m_modelSysPra->item(11,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(11,2,new QStandardItem("��"));
    m_modelSysPra->item(11,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(12,0,new QStandardItem("��ذ�������һ�������"));
    m_modelSysPra->item(12,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(12,2,new QStandardItem("A"));
    m_modelSysPra->item(12,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(13,0,new QStandardItem("��ذ����������������"));
    m_modelSysPra->item(13,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(13,2,new QStandardItem("A"));
    m_modelSysPra->item(13,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(14,0,new QStandardItem("��ذ��ŵ����һ�������"));
    m_modelSysPra->item(14,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(14,2,new QStandardItem("A"));
    m_modelSysPra->item(14,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(15,0,new QStandardItem("��ذ��ŵ�������������"));
    m_modelSysPra->item(15,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(15,2,new QStandardItem("A"));
    m_modelSysPra->item(15,2)->setTextAlignment(Qt::AlignCenter);


    m_modelSysPra->setItem(16,0,new QStandardItem("��ذ��²�һ�������"));
    m_modelSysPra->item(16,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(16,2,new QStandardItem("��"));
    m_modelSysPra->item(16,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(17,0,new QStandardItem("��ذ��²���������"));
    m_modelSysPra->item(17,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(17,2,new QStandardItem("��"));
    m_modelSysPra->item(17,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(18,0,new QStandardItem("��ذ���ѹһ�������"));
    m_modelSysPra->item(18,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(18,2,new QStandardItem("V"));
    m_modelSysPra->item(18,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(19,0,new QStandardItem("��ذ���ѹ���������"));
    m_modelSysPra->item(19,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(19,2,new QStandardItem("V"));
    m_modelSysPra->item(19,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(20,0,new QStandardItem("��ذ�Ƿѹһ�������"));
    m_modelSysPra->item(20,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(20,2,new QStandardItem("V"));
    m_modelSysPra->item(20,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(21,0,new QStandardItem("��ذ�Ƿѹ���������"));
    m_modelSysPra->item(21,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(21,2,new QStandardItem("V"));
    m_modelSysPra->item(21,2)->setTextAlignment(Qt::AlignCenter);

    m_modelSysPra->setItem(22,0,new QStandardItem("�����ѹһ����һ�������"));
    m_modelSysPra->item(22,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(22,2,new QStandardItem("mV"));
    m_modelSysPra->item(22,2)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(23,0,new QStandardItem("�����ѹ������һ�������"));
    m_modelSysPra->item(23,0)->setTextAlignment(Qt::AlignCenter);
    m_modelSysPra->setItem(23,2,new QStandardItem("mV"));
    m_modelSysPra->item(23,2)->setTextAlignment(Qt::AlignCenter);
}

void SysPraModel::onTime(void)
{
    unsigned char readData[8] = {0};
    short tempData = 0;

    //PACKPRA
    m_dataProcess->getPackPra(readData,8);
    ui->labelSeriesNum->setText(QString::number(readData[0])+"��");
    ui->labelParallelNum->setText(QString::number(readData[1])+"��");
    unsigned short capacity = (readData[3]<<8)|readData[2];
    ui->labelCapacity->setText(QString::number(capacity)+"Ah");

    // OUC ��о�ߵ�ѹ
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getCellOUC(readData,8);
    tempData = ((readData[1]<<8)+readData[0]);
    m_modelSysPra->setItem(0,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[3]<<8)+readData[2]);
    m_modelSysPra->setItem(1,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[5]<<8)+readData[4]);
    m_modelSysPra->setItem(2,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[7]<<8)+readData[6]);
    m_modelSysPra->setItem(3,1,new QStandardItem(QString::number(tempData)));

    //COT
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackCOT(readData,8);
    tempData = ((readData[1]<<8)+readData[0]);
    m_modelSysPra->setItem(4,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[3]<<8)+readData[2]);
    m_modelSysPra->setItem(5,1,new QStandardItem(QString::number(tempData)));

    // CUT
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackCUT(readData,8);
    tempData = ((readData[3]<<8)+readData[2]);
    m_modelSysPra->setItem(6,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[1]<<8)+readData[0]);
    m_modelSysPra->setItem(7,1,new QStandardItem(QString::number(tempData)));

    // DOT
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackDOT(readData,8);
    tempData = ((readData[3]<<8)+readData[2]);
    m_modelSysPra->setItem(8,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[1]<<8)+readData[0]);
    m_modelSysPra->setItem(9,1,new QStandardItem(QString::number(tempData)));

    // DUT
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackDUT(readData,8);
    tempData = ((readData[3]<<8)+readData[2]);
    m_modelSysPra->setItem(10,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[1]<<8)+readData[0]);
    m_modelSysPra->setItem(11,1,new QStandardItem(QString::number(tempData)));

    // COC
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackCOC(readData,8);
    tempData = ((readData[1]<<8)+readData[0])/10;
    m_modelSysPra->setItem(12,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[4]<<8)+readData[3])/10;
    m_modelSysPra->setItem(13,1,new QStandardItem(QString::number(tempData)));

    // DOC
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackDOC(readData,8);
    tempData = ((readData[1]<<8)+readData[0])/10;
    m_modelSysPra->setItem(14,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[4]<<8)+readData[3])/10;
    m_modelSysPra->setItem(15,1,new QStandardItem(QString::number(tempData)));

    // PDIT ��ذ��²�
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackDIT(readData,8);
    tempData = ((readData[1]<<8)+readData[0])/10;
    m_modelSysPra->setItem(16,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[3]<<8)+readData[2])/10;
    m_modelSysPra->setItem(17,1,new QStandardItem(QString::number(tempData)));

    // ��ذ���ѹ
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackOV(readData,8);
    tempData = ((readData[1]<<8)+readData[0])/10;
    m_modelSysPra->setItem(18,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[3]<<8)+readData[2])/10;
    m_modelSysPra->setItem(19,1,new QStandardItem(QString::number(tempData)));

    // ��ذ�Ƿѹ
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getPackUV(readData,8);
    tempData = ((readData[1]<<8)+readData[0])/10;
    m_modelSysPra->setItem(20,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[3]<<8)+readData[2])/10;
    m_modelSysPra->setItem(21,1,new QStandardItem(QString::number(tempData)));

    // IBM ����һ����
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getCellIBM(readData,8);
    tempData = ((readData[1]<<8)+readData[0]);
    m_modelSysPra->setItem(22,1,new QStandardItem(QString::number(tempData)));
    tempData = ((readData[3]<<8)+readData[2]);
    m_modelSysPra->setItem(23,1,new QStandardItem(QString::number(tempData)));


    if(m_can->getIsStart())
        ui->pushButtonWritePra->setEnabled(true);
    else
        ui->pushButtonWritePra->setEnabled(false);
}

uint32_t CAN_GenerateID(uint8_t msg_fc)
{
    uint32_t temp = 0;

    temp = ((uint32_t)0x18 << 24) + ((uint32_t)msg_fc << 16) +
        (0xF4 << 8) + GUI;
    return temp;
}

// д�����õĲ���
void SysPraModel::on_pushButtonWritePra_clicked()
{
    static int devType = m_can->getDevType();
    static int devIndex = m_can->getDevIndex();
    static int canIndex = m_can->getCanIndex();
    unsigned short data[3] = {0};
    static VCI_CAN_OBJ sendFrame[3];

    data[0] = ui->spinBoxCOV->text().toInt();
    data[1] = ui->spinBoxCUV->text().toInt();
    data[2] = ui->spinBoxCOT->text().toInt();

    for(int i=0;i<3;i++)
    {
        sendFrame[i].SendType = 0;
        sendFrame[i].RemoteFlag = 0;
        sendFrame[i].ExternFlag = 1;
        sendFrame[i].DataLen = 8;
        sendFrame[i].Data[0] = data[i]&0xFF;
        sendFrame[i].Data[1] = (data[i]>>8)&0xFF;
        sendFrame[i].Data[2] = (data[i])&0xFF;
        sendFrame[i].Data[3] = (data[i]>>8)&0xFF;
        sendFrame[i].ID = CAN_GenerateID(CAN_GUI_CONFIG_COV_TH+i);
    }

    for(int i=0;i<3;i++)
    {
        if(m_can->CanTransmit(devType,devIndex,canIndex,&sendFrame[i],1))
            qDebug() << "Write Success";
        else
            qDebug() << "Write failed";
    }
}

void SysPraModel::on_pushButtonImportConfig_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Import config file"),"",tr("Configfile(*.xml);;Allfile(*.*)"));

    if(!fileName.isEmpty())
    {
        xmlconfig = new XmlConfig();

        if(xmlconfig->readFile(fileName))
        {
            ui->lineEditConfigFilePath->setText(QFileInfo(fileName).canonicalFilePath());
            ui->lineEditConfigFilePath->setVisible(true);
            ui->pushButtonWritePra->setEnabled(true);
        }

    }
}
