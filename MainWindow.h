#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>
#include "DataProcess.h"
#include <QCloseEvent>

#define IndictorLightNum 5 //ͨ��ָʾ������

class QHBoxLayout;
class QVBoxLayout;
class VoltageUI;
class CellMinMaxInfo;
class AmpereMeter;
class BatterySoc;
class IndictorLight;
class QLabel;
class DevSetDialog;
class QStandardItemModel;
class ExcelEngine;
class SysPraModel;

namespace Ui {
class MainWindow;
}

//ϵͳ�ı�����Ϣ����
typedef union
{
    uint32_t all;

    struct
    {
        unsigned COV :2;        // �����ع�ѹ
        unsigned CUV :2;        // ������Ƿѹ
        unsigned COT :2;		// ���������
        unsigned CUT :2;		// ���������
        unsigned DOT :2;		// ����ŵ����
        unsigned DUT :2;		// ����ŵ����
        unsigned COC :2;		// ������
        unsigned DOC :2;		// �ŵ����
        unsigned CIB :2;     	// ������һ����
        unsigned PCBOT :2;    	// PCB�����
        unsigned PCBUT :2;    	// PCB�����
        unsigned POV :2;		// ������ѹ��
        unsigned PUV :2;		// ������ѹ��
        unsigned TIB :2;		// ����²��
        unsigned ISO :2;		// ��Ե����
        unsigned     :2;
    };
}SysWarningDef;

class MainWindow : public QMainWindow
{
    Q_OBJECT

private:
    QTimer *m_timer;
    BatterySoc* m_batterySoc;
    VoltageUI* m_voltageui;
    AmpereMeter * ampereMeter;
    IndictorLight* m_indictor_light[IndictorLightNum];
    QLabel *m_indictor_light_label[IndictorLightNum];
    IndictorLight *m_devCanLight;
    CellMinMaxInfo* m_cellMinMaxInfoUi;
    DevSetDialog* m_devsetdlg;
    DataProcess* m_dataProcess;
    SysPraModel* m_sysPraModel;
    QStandardItemModel *model;
    QLabel *statusLabel;
    QStandardItemModel *modelTestData;

protected:
     void closeEvent(QCloseEvent *event);
public:
    void initUI();
    explicit MainWindow(QWidget *parent = 0);

    void initModel();

    void setWarnRadioButton(int level,QRadioButton *radio1,QRadioButton *radio2);

    ~MainWindow();
signals:
    void   openButtonClicked();
private slots:
    void timeUpdate();

    void on_actionSet_triggered();

    void on_pushButtonOpen_clicked();

    void on_actionBoardInfo_triggered();

    void dataReceived(VCI_CAN_OBJ &dat);

    void on_pushButtonOutputData_clicked();

    void on_pushButtonWritePrograme_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
