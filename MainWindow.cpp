#include <QGridLayout>
#include <QLabel>
#include <QThread>
#include <QTime>
#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "VoltageUI.h"
#include "CellMinMaxInfo.h"
#include "AmpereMeter.h"
#include "BatterySoc.h"
#include "IndictorLight.h"
#include "DevSetDialog.h"
#include "ControlCAN.h"
#include "ExcelEngine.h"
#include "TableView2Excel.h"
#include "DownLoadUi.h"
#include "SysPraModel.h"

#define ROWNUM 10000

QString statusArray[7] = {
    "IDLE","PRECHARGE","DISCHARGE",
    "CHARGE","HEATING","COOLING","PROTECTION"
};

const char* IndictorLabelText[IndictorLightNum] = {
    "������ͨ��","BMS��ͨ��","�¶�ģ��ͨ��",
    "�Ǳ�ͨ��","�û�����ͨ��"
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initUI();

    //�򿪹رհ�ť�ź�
    connect(ui->pushButtonOpen,SIGNAL(clicked()),m_devsetdlg,SLOT(devSetButtonClicked()));
    //
    connect(m_dataProcess,SIGNAL(datRecSignal(VCI_CAN_OBJ&)),
            this,SLOT(dataReceived(VCI_CAN_OBJ&)),Qt::DirectConnection);

    ui->pushButtonOutputData->setEnabled(false);

    //�����Ϣ�õĶ�ʱ��
    m_timer = new QTimer(this);
    m_timer->setInterval(500);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(timeUpdate()));
    m_timer->start();
}

void MainWindow::initUI()
{
    statusLabel = new QLabel(this);
    ui->statusBar->addWidget(statusLabel);
    model = new QStandardItemModel();

    //CAN���ݴ�����
    m_dataProcess = new DataProcess();
    m_dataProcess->setTimerInterval(100); //50ms����һ������
    m_sysPraModel = new SysPraModel(m_dataProcess,ui->tabSet);
    m_sysPraModel->move(0,5);
    m_sysPraModel->show();

    //CAN�豸���ý����ʼ��
    m_devsetdlg = new DevSetDialog(this);
    m_devsetdlg->setWindowTitle(tr("ѡ���CAN�豸"));
    m_devsetdlg->resize(320,280);
    m_devsetdlg->setFixedSize(m_devsetdlg->width(),m_devsetdlg->height());
    m_devsetdlg->getValueFrmUi();//��ȡCAN��Ĭ�ϲ���

    //�����豸CAN��ָʾ��
    m_devCanLight = new IndictorLight(this);
    m_devCanLight->move(20,20);
    m_devCanLight->resize(35,35);
    m_devCanLight->setColor(Qt::red);
    m_devCanLight->show();
    ui->labelDevStatus->move(58,6);

    ui->pushButtonOpen->resize(38,38);
    ui->pushButtonOpen->setFlat(true);
    //����CAn�豸״̬���ô򿪰�ť
    if(m_devsetdlg->getCan()->getIsOpen() && m_devsetdlg->getCan()->getIsStart())
    {
        QIcon  icon(":/src/Close.jpg");
        m_devCanLight->setColor(Qt::green);
        ui->pushButtonOpen->setIcon(icon);
        ui->pushButtonOpen->setIconSize(QSize(36,36));
        ui->pushButtonWritePrograme->setEnabled(true);
    }
    else
    {
        m_devCanLight->setColor(Qt::red);
        QIcon  icon(":/src/Open.jpg");
        ui->pushButtonOpen->setIcon(icon);
        ui->pushButtonOpen->setIconSize(QSize(40,40));
        ui->pushButtonWritePrograme->setEnabled(false);
    }

    //���Ƶ�ѹ��
    m_voltageui = new VoltageUI(ui->groupBoxMianInfo);
    m_voltageui->resize(220,220);
    m_voltageui->setVisible(true);
    m_voltageui->move(20,20);
    m_voltageui->show();

    //���Ƶ�����
    ampereMeter = new AmpereMeter(ui->groupBoxMianInfo);
    ampereMeter->resize(220,220);
    ampereMeter->setVisible(true);
    ampereMeter->move(280,20);
    ampereMeter->show();

    //���Ƶ�����ʾ��
    m_batterySoc = new BatterySoc(ui->groupBoxMianInfo);
    m_batterySoc->resize(80,220);
    m_batterySoc->move(540,20);
    m_batterySoc->show();

    //���Ƶ�о��ֵ��Ϣ
    m_cellMinMaxInfoUi = new CellMinMaxInfo(ui->groupBoxMianInfo);
    m_cellMinMaxInfoUi->move(15,300);
    m_cellMinMaxInfoUi->show();

    //����ͨ��ָʾ��
    for(uchar i=0;i<IndictorLightNum;i++)
    {
        m_indictor_light[i] = new IndictorLight(this);
        m_indictor_light_label[i] = new QLabel(this);

        m_indictor_light[i]->move(20+i*120,600);
        m_indictor_light[i]->resize(30,30);
        if(i%2 == 0)
        {
            m_indictor_light[i]->setColor(Qt::red);
        }
        else
        {
            m_indictor_light[i]->setColor(Qt::green);
        }
        m_indictor_light[i]->show();

        m_indictor_light_label[i]->setText(IndictorLabelText[i]);
        m_indictor_light_label[i]->move(20+i*120+m_indictor_light[i]->width()+5,600);
    }

    //tableView������
    initModel();

    //��ͷ��Ϣ��ʾ����
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);

    //�����п��ɱ�
    for(int i=0;i<6;i++)
    {
        ui->tableView->horizontalHeader()->setResizeMode(i,QHeaderView::Fixed);
        ui->tableView->setColumnWidth(i,130);
    }
    ui->tableView->horizontalHeader()->setResizeMode(6,QHeaderView::Fixed);
    ui->tableView->setColumnWidth(6,200);

    //����ѡ��ʱΪ����ѡ��
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    //���ñ��ĵ�ԪΪֻ�����ԣ������ܱ༭
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //���õ�о��ѹֻ��
    ui->lineEditCell01->setReadOnly(true);
    ui->lineEditCell02->setReadOnly(true);
    ui->lineEditCell03->setReadOnly(true);
    ui->lineEditCell04->setReadOnly(true);
    ui->lineEditCell05->setReadOnly(true);
    ui->lineEditCell06->setReadOnly(true);
    ui->lineEditCell07->setReadOnly(true);
    ui->lineEditCell08->setReadOnly(true);
    ui->lineEditCell09->setReadOnly(true);
    ui->lineEditCell10->setReadOnly(true);
    ui->lineEditCell11->setReadOnly(true);
    ui->lineEditCell12->setReadOnly(true);
    ui->lineEditCell13->setReadOnly(true);
    ui->lineEditCell14->setReadOnly(true);
    ui->lineEditCell15->setReadOnly(true);
    ui->lineEditCell16->setReadOnly(true);
    ui->lineEditCell17->setReadOnly(true);
    ui->lineEditCell18->setReadOnly(true);
    ui->lineEditCell19->setReadOnly(true);
    ui->lineEditCell20->setReadOnly(true);

    ui->lineEditCOC->setReadOnly(true);
    ui->lineEditDOC->setReadOnly(true);
    ui->lineEditCOV->setReadOnly(true);
    ui->lineEditCUV->setReadOnly(true);
    ui->lineEditCOT->setReadOnly(true);
    ui->lineEditCUT->setReadOnly(true);
    ui->lineEditDOT->setReadOnly(true);
    ui->lineEditDUT->setReadOnly(true);
    ui->lineEditLTC_COM->setReadOnly(true);

    // ���õ������ݵ�model
    modelTestData = new QStandardItemModel();
    modelTestData->setColumnCount(15);
    modelTestData->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("ʱ��"));
    modelTestData->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("��ذ�״̬"));
    modelTestData->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("�ܵ�ѹ"));
    modelTestData->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("�ܵ���"));
    modelTestData->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("SOC"));
    modelTestData->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("����ѹ"));
    modelTestData->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("���VMax"));
    modelTestData->setHeaderData(7,Qt::Horizontal,QString::fromLocal8Bit("��С��ѹ"));
    modelTestData->setHeaderData(8,Qt::Horizontal,QString::fromLocal8Bit("���VMin"));
    modelTestData->setHeaderData(9,Qt::Horizontal,QString::fromLocal8Bit("����¶�"));
    modelTestData->setHeaderData(10,Qt::Horizontal,QString::fromLocal8Bit("���TMax"));
    modelTestData->setHeaderData(11,Qt::Horizontal,QString::fromLocal8Bit("��С�¶�"));
    modelTestData->setHeaderData(12,Qt::Horizontal,QString::fromLocal8Bit("���TMin"));
    modelTestData->setHeaderData(13,Qt::Horizontal,QString::fromLocal8Bit("ƽ���¶�"));
    modelTestData->setHeaderData(14,Qt::Horizontal,QString::fromLocal8Bit("��ѹ��"));

    //Ĭ�ϲ���¼����
    ui->radioButtonNo->setChecked(true);
}

void MainWindow::initModel()
{
    if(model != NULL)
        delete model;

    model = new QStandardItemModel();
    //tableView������
    model->setColumnCount(7);
    model->setHeaderData(0,Qt::Horizontal,QString::fromLocal8Bit("���䷽��"));
    model->setHeaderData(1,Qt::Horizontal,QString::fromLocal8Bit("ʱ���ʶ"));
    model->setHeaderData(2,Qt::Horizontal,QString::fromLocal8Bit("֡ID"));
    model->setHeaderData(3,Qt::Horizontal,QString::fromLocal8Bit("֡��ʽ"));
    model->setHeaderData(4,Qt::Horizontal,QString::fromLocal8Bit("֡����"));
    model->setHeaderData(5,Qt::Horizontal,QString::fromLocal8Bit("���ݳ���"));
    model->setHeaderData(6,Qt::Horizontal,QString::fromLocal8Bit("����"));
    ui->tableView->setModel(model);
}

void MainWindow::timeUpdate(void)
{
    if(!m_devsetdlg->getCan()->getIsOpen() ||
            !m_devsetdlg->getCan()->getIsStart())
    {
        return;
    }
    float maxV  = m_dataProcess->getMaxCellVoltage();
    float minV = m_dataProcess->getMinCellVoltage();
    uchar maxVP = m_dataProcess->getMaxCellVolNum();
    uchar minVP = m_dataProcess->getMinCellVolNum();
    float vDlt = maxV - minV;

    float maxT = m_dataProcess->getMaxCellTemp();
    float minT = m_dataProcess->getMinCellTemp();
    uchar maxTP = m_dataProcess->getMaxCellTempNum();
    uchar minTP = m_dataProcess->getMinCellTempNum();
    float tDlt = maxT - minT;

    float totalVoltage = m_dataProcess->getTotalVoltage();
    float current = 0.0;
    ushort currentTemp = m_dataProcess->getCurrent();
    if( currentTemp&0x8000)
    {
        short temp = 0xFFFF - currentTemp;
        current = temp * (-0.1);
    }
    else
    {
        current = currentTemp * 0.1;
    }

    uchar soc = m_dataProcess->getSoc();

    //���̸���
    m_voltageui->setValue(totalVoltage);
    ampereMeter->setValue(current);
    m_batterySoc->setValue(soc);

    m_cellMinMaxInfoUi->lineEditMaxCellVol->setText(QString::number(maxV));
    m_cellMinMaxInfoUi->lineEditMaxVPos->setText(QString::number(maxVP));
    m_cellMinMaxInfoUi->lineEditMinCellVol->setText(QString::number(minV));
    m_cellMinMaxInfoUi->lineEditMinVPos->setText(QString::number(minVP));
    m_cellMinMaxInfoUi->lineEditDltV->setText(QString::number(vDlt));

    m_cellMinMaxInfoUi->lineEditMaxCellTemp->setText(QString::number(maxT));
    m_cellMinMaxInfoUi->lineEditMinCellTemp->setText(QString::number(minT));
    m_cellMinMaxInfoUi->lineEditMaxTPos->setText(QString::number(maxTP));
    m_cellMinMaxInfoUi->lineEditMinTPos->setText(QString::number(minTP));
    m_cellMinMaxInfoUi->lineEditDltT->setText(QString::number(tDlt));

    float cellsValt[20] = {0};
    m_dataProcess->getCellsVoltage(cellsValt,20);
    //����Cell�ĵ�ѹ
    ui->lineEditCell01->setText(QString::number(cellsValt[0]));
    ui->lineEditCell02->setText(QString::number(cellsValt[1]));
    ui->lineEditCell03->setText(QString::number(cellsValt[2]));
    ui->lineEditCell04->setText(QString::number(cellsValt[3]));
    ui->lineEditCell05->setText(QString::number(cellsValt[4]));
    ui->lineEditCell06->setText(QString::number(cellsValt[5]));
    ui->lineEditCell07->setText(QString::number(cellsValt[6]));
    ui->lineEditCell08->setText(QString::number(cellsValt[7]));
    ui->lineEditCell09->setText(QString::number(cellsValt[8]));
    ui->lineEditCell10->setText(QString::number(cellsValt[9]));

    ui->lineEditCell11->setText(QString::number(cellsValt[10]));
    ui->lineEditCell12->setText(QString::number(cellsValt[11]));
    ui->lineEditCell13->setText(QString::number(cellsValt[12]));
    ui->lineEditCell14->setText(QString::number(cellsValt[13]));
    ui->lineEditCell15->setText(QString::number(cellsValt[14]));
    ui->lineEditCell16->setText(QString::number(cellsValt[15]));
    ui->lineEditCell17->setText(QString::number(cellsValt[16]));
    ui->lineEditCell18->setText(QString::number(cellsValt[17]));
    ui->lineEditCell19->setText(QString::number(cellsValt[18]));
    ui->lineEditCell20->setText(QString::number(cellsValt[19]));

    short temp[4] = {0};
    m_dataProcess->getCellTemp(temp,8);
    ui->lineEditTemp01->setText(QString::number(temp[0]));
    ui->lineEditTemp02->setText(QString::number(temp[1]));
    ui->lineEditTemp03->setText(QString::number(temp[2]));
    ui->lineEditTemp04->setText(QString::number(temp[3]));

    //ϵͳ����ˢ��
    unsigned char readData[8] = {0};
    short tempData = 0;

    // NOR_REC
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getNorRec(readData,8);
    tempData = readData[1];
    tempData <<= 8;
    tempData += readData[0];
    m_cellMinMaxInfoUi->lineEditCycTimes->setText(QString::number(tempData));

    // FALT_OC
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getOCRec(readData,8);
    tempData = readData[1];
    tempData <<= 8;
    tempData += readData[0];
    ui->lineEditCOC->setText(QString::number(tempData));
    tempData = readData[5];
    tempData <<= 8;
    tempData += readData[4];
    ui->lineEditDOC->setText(QString::number(tempData));

    //OUV
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getOUVRec(readData,8);
    tempData = readData[1];
    tempData <<= 8;
    tempData += readData[0];
    ui->lineEditCOV->setText(QString::number(tempData));
    tempData = readData[5];
    tempData <<= 8;
    tempData += readData[4];
    ui->lineEditCUV->setText(QString::number(tempData));

    // COUT
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getCOUTRec(readData,8);
    tempData = readData[1];
    tempData <<= 8;
    tempData += readData[0];
    ui->lineEditCOT->setText(QString::number(tempData));
    tempData = readData[5];
    tempData <<= 8;
    tempData += readData[4];
    ui->lineEditCUT->setText(QString::number(tempData));

    // DOUT
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getDOUTRec(readData,8);
    tempData = readData[1];
    tempData <<= 8;
    tempData += readData[0];
    ui->lineEditDOT->setText(QString::number(tempData));
    tempData = readData[5];
    tempData <<= 8;
    tempData += readData[4];
    ui->lineEditDUT->setText(QString::number(tempData));

    // HARD  LTC6803
    memset(readData,0,8);
    tempData = 0;
    m_dataProcess->getLtcRec(readData,8);
    tempData = readData[1];
    tempData <<= 8;
    tempData += readData[0];
    ui->lineEditLTC_COM->setText(QString::number(tempData));

    uchar index = (uchar)m_sysPraModel->getStatus();
    if(index >= 0 && index < 7)
        statusLabel->setText(statusArray[index]);
    else
        statusLabel->setText("UNKNOW");

    // ��ȡҪ�����Ĳ�������
    if(ui->radioButtonNo->isChecked())
        return;
    // ��ȡ����
    int rows = modelTestData->rowCount();
    QTime currenttime = QTime::currentTime();

    modelTestData->setItem(rows,0,new QStandardItem(currenttime.toString()));
    modelTestData->setItem(rows,1,new QStandardItem(statusLabel->text()));
    modelTestData->setItem(rows,2,new QStandardItem(QString::number(totalVoltage)));
    modelTestData->setItem(rows,3,new QStandardItem(QString::number(current)));
    modelTestData->setItem(rows,4,new QStandardItem(QString::number(soc)));
    modelTestData->setItem(rows,5,new QStandardItem(QString::number(maxV)));
    modelTestData->setItem(rows,6,new QStandardItem(QString::number(maxVP)));
    modelTestData->setItem(rows,7,new QStandardItem(QString::number(minV)));
    modelTestData->setItem(rows,8,new QStandardItem(QString::number(minVP)));
    modelTestData->setItem(rows,9,new QStandardItem(QString::number(maxT)));
    modelTestData->setItem(rows,10,new QStandardItem(QString::number(maxTP)));
    modelTestData->setItem(rows,11,new QStandardItem(QString::number(minT)));
    modelTestData->setItem(rows,12,new QStandardItem(QString::number(minTP)));
    uint tempSum = 0;
    for(int i=0;i<4;i++)
    {
        tempSum += temp[i];
    }
    modelTestData->setItem(rows,13,new QStandardItem(QString::number(tempSum/4)));
    modelTestData->setItem(rows,14,new QStandardItem(QString::number(vDlt)));

}

void MainWindow::dataReceived(VCI_CAN_OBJ &data)
{
    int rows = model->rowCount();
    if(rows > ROWNUM)
    {
       initModel();
       rows = 0;
    }
    //����tableView�ؼ�������
    model->setItem(rows,0,new QStandardItem("����"));
    //�����ַ���ɫ
    model->item(rows,0)->setForeground(QBrush(QColor(0, 0, 255)));
    //�����ַ�λ��
    model->item(rows,0)->setTextAlignment(Qt::AlignCenter);
    model->setItem(rows,1,new QStandardItem(QString::number(QDateTime::currentDateTime().toTime_t())));
    model->item(rows,1)->setTextAlignment(Qt::AlignCenter);
    model->setItem(rows,2,new QStandardItem(QString::number(data.ID,16).toUpper()));
    model->item(rows,2)->setTextAlignment(Qt::AlignCenter);
    model->setItem(rows,4,new QStandardItem(QString::fromLocal8Bit("����֡")));
    model->item(rows,4)->setTextAlignment(Qt::AlignCenter);

    if(data.ExternFlag)
        model->setItem(rows,3,new QStandardItem(QString::fromLocal8Bit("��չ֡")));
    else
        model->setItem(rows,3,new QStandardItem(QString::fromLocal8Bit("��׼֡")));
    if(data.RemoteFlag)
        data.DataLen = 0;
    model->item(rows,3)->setTextAlignment(Qt::AlignCenter);
    model->setItem(rows,5,new QStandardItem(QString::number(data.DataLen,10)));
    model->item(rows,5)->setTextAlignment(Qt::AlignCenter);

    model->setItem(rows,6,new QStandardItem(QString("%1").arg(data.Data[7]&0xff,2,16,QLatin1Char('0')).toUpper()
                                            + " " + QString("%1").arg(data.Data[6]&0xff,2,16,QLatin1Char('0')).toUpper()
                                            + " " + QString("%1").arg(data.Data[5]&0xff,2,16,QLatin1Char('0')).toUpper()
                                            + " " + QString("%1").arg(data.Data[4]&0xff,2,16,QLatin1Char('0')).toUpper()
                                            + " " + QString("%1").arg(data.Data[3]&0xff,2,16,QLatin1Char('0')).toUpper()
                                            + " " + QString("%1").arg(data.Data[2]&0xff,2,16,QLatin1Char('0')).toUpper()
                                            + " " + QString("%1").arg(data.Data[1]&0xff,2,16,QLatin1Char('0')).toUpper()
                                            + " " + QString("%1").arg(data.Data[0]&0xff,2,16,QLatin1Char('0')).toUpper()));
    model->item(rows,6)->setTextAlignment(Qt::AlignCenter);
    model->item(rows,6)->setForeground(QBrush(QColor(255, 0, 0)));

    ui->tableView->selectRow(rows);
    QCoreApplication::processEvents();
}
MainWindow::~MainWindow()
{
    delete ui;
    delete m_timer;
}

void MainWindow::on_actionSet_triggered()
{
    if(m_devsetdlg->exec() == QDialog::Accepted)
    {
        m_devsetdlg->getValueFrmUi();
    }
}

void MainWindow::on_pushButtonOpen_clicked()
{
    int ret = 0;
    int devType = m_devsetdlg->getCan()->getDevType();
    int devIndex = m_devsetdlg->getCan()->getDevIndex();
    int canIndex = m_devsetdlg->getCan()->getCanIndex();
    PVCI_INIT_CONFIG pInitConfig = m_devsetdlg->getCan()->getInitConfig();

    //����豸�Ǵ򿪵ľ͹ر�
    if(m_devsetdlg->getCan()->getIsOpen() &&
            m_devsetdlg->getCan()->getIsStart())
    {
        m_devsetdlg->getCan()->CanResetCAN(devType,devIndex,canIndex);//CAN��λ
        if(m_devsetdlg->getCan()->CanClose(devType,devIndex))
        {
            m_devCanLight->setColor(Qt::red);
            QIcon  icon(":/src/Open.jpg");
            ui->pushButtonOpen->setIconSize(QSize(42,42));
            ui->pushButtonOpen->setIcon(icon);

            m_devsetdlg->getCan()->setStartorStop(false);//�ر�CAN
            m_dataProcess->timerStop();
        }
        if(modelTestData->rowCount() > 0)
            ui->pushButtonOutputData->setEnabled(true);
        else
            ui->pushButtonOutputData->setEnabled(false);
        ui->pushButtonWritePrograme->setEnabled(false);

    }
    else //��
    {
        if(!m_devsetdlg->getCan()->getIsOpen())
            m_devsetdlg->getCan()->CanOpen(devType,devIndex,0);

        if(m_devsetdlg->getCan()->getIsOpen())
        {
            ret = m_devsetdlg->getCan()->CanInit(devType,devIndex,canIndex,pInitConfig);
            if(ret)
                m_devsetdlg->getCan()->CanStartCAN(devType,devIndex,canIndex);
        }
        if(m_devsetdlg->getCan()->getIsStart())
        {
            m_devCanLight->setColor(Qt::green);
            QIcon  icon(":/src/Close.jpg");
            ui->pushButtonOpen->setIconSize(QSize(36,36));
            ui->pushButtonOpen->setIcon(icon);
            //����CAN���ͽ���
            m_dataProcess->timerStart();
            ui->pushButtonOutputData->setEnabled(false);
            ui->pushButtonWritePrograme->setEnabled(true);
        }
    }
}

//�鿴�豸��Ϣ
void MainWindow::on_actionBoardInfo_triggered()
{
    QDialog boardInfoWidget(this);
    VCI_BOARD_INFO boardInfo;
    QLabel label(&boardInfoWidget);
    QPushButton button(&boardInfoWidget);
    CanBus *can = CanBus::getInstance();

    connect(&button,SIGNAL(clicked()),&boardInfoWidget,SLOT(close()));

    int devType = m_devsetdlg->getCan()->getDevType();
    int devIndex = m_devsetdlg->getCan()->getDevIndex();

    boardInfoWidget.setWindowTitle(QObject::tr("�豸��Ϣ"));

    if(!can->getIsOpen())
        can->CanOpen(devType,devIndex,0);

    if(can->getIsOpen() &&
            (!can->CanReadBoardInfo(devType,devIndex,&boardInfo)))
    {
        QMessageBox::warning(this,QObject::tr("��ȡʧ��"),
                             QObject::tr("��ȡ�豸��Ϣʧ��"),QMessageBox::Ok);
    }
    else
    {
        label.setText(QString("\n")+
                    QObject::tr("hw_Version:")+QString::number(boardInfo.fw_Version)+"\n"
                            +QObject::tr("fw_Version:") + QString::number(boardInfo.fw_Version)+"\n"
                            +QObject::tr("dr_Version:") + QString::number(boardInfo.dr_Version)+"\n"
                            +QObject::tr("in_Version:") + QString::number(boardInfo.in_Version)+"\n"
                            +QObject::tr("irq_Num:") + QString::number(boardInfo.irq_Num)+"\n"
                            +QObject::tr("can_Num:") + QString::number(boardInfo.can_Num)+"\n"
                            +QObject::tr("str_hw_Type:") + boardInfo.str_hw_Type+"\n"
                            +"\n");
        label.resize(150,115);
        label.move(10,6);
        button.setText(QObject::tr("ȷ��"));
        button.resize(40,30);
        boardInfoWidget.resize(150,160);
        button.move(110,125);
        boardInfoWidget.exec();
    }
}

void MainWindow::on_pushButtonOutputData_clicked()
{
    uint cnt = m_dataProcess->dataStore(NULL,modelTestData);
    //uint cnt = m_dataProcess->dataStore(NULL,model);
    if(cnt)
        QMessageBox::information(this,QObject::tr("������"),QString("%1 �����ݱ���ɹ�").arg(cnt),QMessageBox::Ok);
    else
        QMessageBox::information(this,QObject::tr("������"),QObject::tr("����ʧ��"),QMessageBox::Ok);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    int devType = m_devsetdlg->getCan()->getDevType();
    int devIndex = m_devsetdlg->getCan()->getDevIndex();
    int canIndex = m_devsetdlg->getCan()->getCanIndex();

    //TODO: ���˳�����֮ǰ��ʵ��ϣ�����Ĳ���
    if(m_devsetdlg->getCan()->getIsOpen() &&
            m_devsetdlg->getCan()->getIsStart())
    {
        m_devsetdlg->getCan()->CanResetCAN(devType,devIndex,canIndex);//CAN��λ
        if(m_devsetdlg->getCan()->CanClose(devType,devIndex))
        {
            m_devCanLight->setColor(Qt::red);
            QIcon  icon(":/src/Open.jpg");
            ui->pushButtonOpen->setIconSize(QSize(42,42));
            ui->pushButtonOpen->setIcon(icon);

            m_devsetdlg->getCan()->setStartorStop(false);//�ر�CAN
            m_dataProcess->timerStop();
        }
    }
    qDebug() << "Close";
}

void MainWindow::on_pushButtonWritePrograme_clicked()
{
    m_timer->stop();
    m_dataProcess->timerStop();

    DownLoadUi downloadui;
    downloadui.exec();

    m_timer->start();
    m_dataProcess->timerStart();
}


