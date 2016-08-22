#ifndef DATAPROCESS_H
#define DATAPROCESS_H

#include <QTimer>
#include <QMap>
#include "ControlCAN.h"
#include <QMutex>


//���յ��㲥���ݵı�ʾ��
#define BRO_SYS_WARN        0x180150F4  //��ѹ���ܵ��������ϱ���״̬��
#define BRO_BATT_INFO       0x180250F4
#define BRO_CELLV_INFO      0x180350F4
#define BRO_CELLT_INFO      0x180450F4
#define BRO_CELL_VOLT1      0x181050F4
#define BRO_CELL_VOLT2      0x181150F4
#define BRO_CELL_VOLT3      0x181250F4
#define BRO_CELL_VOLT4      0x181350F4
#define BRO_CELL_VOLT5      0x181450F4
#define BRO_TEMP_01         0x182050F4

// GUI������������ı�ʾ��
#define REQ_BMS_VERSION     0x1800F4FD
#define REQ_BATT_STAT       0x1801F4FD
#define REQ_CELL_STAT       0x1802F4FD
#define REQ_PACK_SOH        0x180CF4FD
#define REQ_PACK_WARN       0x180DF4FD
#define REQ_PACK_PRA        0x1820F4FD
#define REQ_READ_COC        0x1822F4FD
#define REQ_READ_DOC        0x1823F4FD
#define REQ_READ_COT        0x1824F4FD
#define REQ_READ_DOT        0x1825F4FD
#define REQ_READ_CUT        0x1826F4FD
#define REQ_READ_DUT        0x1827F4FD
#define REQ_READ_OUC        0x1828F4FD

#define REQ_READ_NOR        0x1840F4FD // SOH ���δ����¼
#define REQ_FALT_OC         0x1841F4FD // ����
#define REQ_FALT_OUV        0x1842F4FD // ��Ƿѹ
#define REQ_FALT_COUT       0x1843F4FD // ���ߵ���
#define REQ_FALT_DOUT       0x1844F4FD // �ŵ�ߵ���
#define REQ_FALT_HARD       0x1846F4FD // ϵͳӲ������ Ŀǰ��Ҫ��6803

// GUI���յ����������ݵı�ʾ��
#define REC_BMS_VERSION     0x1800FDF4
#define REC_BATT_STAT       0x1801FDF4
#define REC_CELL_STAT       0x1802FDF4
#define REC_PACK_SOH        0x180CFDF4
#define REC_PACK_WARN       0x180DFDF4
#define REC_PACK_PRA        0x1820FDF4
#define REC_READ_COC        0x1822FDF4
#define REC_READ_DOC        0x1823FDF4
#define REC_READ_COT        0x1824FDF4
#define REC_READ_DOT        0x1825FDF4
#define REC_READ_CUT        0x1826FDF4
#define REC_READ_DUT        0x1827FDF4
#define REC_READ_OUC        0x1828FDF4

#define REC_READ_NOR        0x1840FDF4 // SOH ���δ����¼
#define REC_FALT_OC         0x1841FDF4 // ����
#define REC_FALT_OUV        0x1842FDF4 // ��Ƿѹ
#define REC_FALT_COUT       0x1843FDF4 // ���ߵ���
#define REC_FALT_DOUT       0x1844FDF4 // �ŵ�ߵ���
#define REC_FALT_HARD       0x1846FDF4 // ϵͳӲ������ Ŀǰ��Ҫ��6803

// GUI��д�ڲ�EEPROM
#define REQ_WRITE_EEPROM        0x1830F4FD
#define REQ_READ_EEPROM         0x1830F4FD

// ��λ������BMS���ֲ���
#define  CAN_GUI_CONFIG_COV_TH                 ((uint8_t)0x10)
#define  CAN_GUI_CONFIG_CUV_TH                 ((uint8_t)0x11)
#define  CAN_GUI_CONFIG_COT_TH                 ((uint8_t)0x12)
#define  CAN_GUI_CONFIG_CUT_TH                 ((uint8_t)0x13)
#define  CAN_GUI_CONFIG_DOT_TH                 ((uint8_t)0x14)
#define  CAN_GUI_CONFIG_DUT_TH                 ((uint8_t)0x15)
#define  CAN_GUI_CONFIG_COC_TH                 ((uint8_t)0x16)
#define  CAN_GUI_CONFIG_DOC_TH                 ((uint8_t)0x17)
#define  CAN_GUI_CONFIG_DLV_TH                 ((uint8_t)0x18)
#define  CAN_GUI_CONFIG_DLT_TH                 ((uint8_t)0x19)
#define  CAN_GUI_CONFIG_POV_TH                  0x1A
#define  CAN_GUI_CONFIG_PUV_TH                  0x1B
#define  CAN_GUI_CONFIG_ISO_TH                  0x1C

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

    float m_cellsVolt[20];

public:
    explicit DataProcess();
    int receiveData();
    int dataAnalyze();
    int dataStore(QTableView *tableView, QStandardItemModel *model);
    short getCurrent();
    uint16_t getTotalVoltage();
    uchar getSoc();

    float getMaxCellVoltage();
    float getMinCellVoltage();
    char getMinCellTemp();
    char getMaxCellTemp();
    uchar getMaxCellVolNum();
    uchar getMinCellVolNum();
    uchar getMaxCellTempNum();
    uchar getMinCellTempNum();
    ushort getCellVolt(uchar num);
    char getCellTemp(uchar num);
    ushort getChgCircNum();         //��ŵ�ѭ������
    void getCellsVoltage(float cellVol[], int len);//��ȡ���е�оʵʱ��ѹ

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
