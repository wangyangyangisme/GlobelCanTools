#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <QTimer>
#include <QMap>
#include "ControlCAN.h"
#include <QMutex>

class CanBus;
class QTableView;
class QStandardItemModel;
class TableView2Excel;

class DataProcess : public QObject
{
    Q_OBJECT
private:
    QTimer *m_timer;
    CanBus *m_can;
    QMap<uint32_t,VCI_CAN_OBJ> m_dataRecv;
    int m_requestNum;
    TableView2Excel *m_tableView2excel;
    QStandardItemModel *model;
    float m_cellsVolt[20];

public:
    explicit DataProcess(QStandardItemModel *mod);
    int receiveData(void);
    int dataStore();
    ushort getCurrent();
    float getTotalVoltage();
    uchar getSoc();
    void setModel(QStandardItemModel *model);

    float getMaxCellVoltage();
    float getMinCellVoltage();
    char getMinCellTemp();
    char getMaxCellTemp();
    uchar getMaxCellVolNum();
    uchar getMinCellVolNum();
    uchar getMaxCellTempNum();
    uchar getMinCellTempNum();
    ushort getCellVolt(uchar num);
    void getCellTemp(char *temp, int len);
    ushort getChgCircNum();         //充放电循环次数
    void getCellsVoltage(float cellVol[], int len);//获取所有电芯实时电压
    void getBattWarnError(unsigned char *buf, int len);
    void getPackPra(unsigned char *buf, int len);
    void getPackCOC(unsigned char *buf, int len);
    void getPackDOC(unsigned char *buf, int len);
    void getPackCOT(unsigned char *buf, int len);
    void getPackDOT(unsigned char *buf, int len);
    void getPackCUT(unsigned char *buf, int len);
    void getPackDUT(unsigned char *buf, int len);
    void getCellOUV(unsigned char *buf, int len);
    void getCellDLV(unsigned char *buf, int len);
    void getPackDLT(unsigned char *buf, int len);
    void getPackOV(unsigned char *buf, int len);
    void getPackUV(unsigned char *buf, int len);

    // 错误发生次数
    void getNorRec(unsigned char *buf, int len);
    void getFaultRec1(unsigned char *buf, int len);
    void getFaultRec2(unsigned char *buf, int len);
    void getFaultRec3(unsigned char *buf, int len);
    void getFaultRec4(unsigned char *buf, int len);
    void getFaultRec5(unsigned char *buf, int len);

    void timerStart(void){m_timer->start();}
    void timerStop(void){m_timer->stop();}
    void setTimerInterval(int t){m_timer->setInterval(t);}
    void sendGetDataRequest();
    QMap<uint,VCI_CAN_OBJ>& getAllData(){return m_dataRecv;}

signals:
    void datRecSignal(VCI_CAN_OBJ &);
    
public slots:
    void onTimeout(); 
};

#endif // DATAPROCESS_H
