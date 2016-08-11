#include "CanBus.h"
#include <QMessageBox>
#include <QDebug>

CanBus* CanBus::cInstance = NULL;

CanBus::CanBus()
{
    m_isOpen = false;
    m_isStart = false;
    m_pInitConfig = new VCI_INIT_CONFIG();
}

DWORD CanBus::CanOpen(DWORD devType,DWORD devIndex,int reserved)
{
    DWORD ret = VCI_OpenDevice(devType,devIndex,reserved);
    if(STATUS_OK != ret)
    {
        QMessageBox::warning(NULL,"���豸","���豸ʧ��",QMessageBox::Ok);
        m_isOpen = false;
    }
    else
    {
        m_isOpen = true;
    }
    return ret;
}

DWORD CanBus::CanClose(DWORD devType,DWORD devIndex)
{
    DWORD ret = VCI_CloseDevice(devType,devIndex);
    if(ret)
    {
        m_isOpen = false;
    }
    else
    {
        m_isOpen = true;
        QMessageBox::warning(NULL,"�ر�CAN","�ر�CANʧ��",QMessageBox::Ok);
    }

    return ret;
}

DWORD CanBus::CanInit(DWORD devType,DWORD devIndex,DWORD canIndex,
              PVCI_INIT_CONFIG pInitConfig)
{
    DWORD ret = VCI_InitCAN(devType, devIndex, canIndex, pInitConfig);
    if(STATUS_ERR == ret)
    {
        //CanClose(devType,devIndex);
        QMessageBox::warning(NULL,"ʧ��","��ʼ���豸ʧ��",QMessageBox::Ok);
    }
    return ret;
}

DWORD CanBus::CanReadBoardInfo(DWORD devType, DWORD devIndex, PVCI_BOARD_INFO pInfo)
{
    DWORD ret;

    ret = VCI_ReadBoardInfo(devType, devIndex,pInfo);

    if(!ret)
    {
        QMessageBox::warning(NULL,"ʧ��","��ȡ��������Ϣʧ��",QMessageBox::Ok);
    }

    return ret;
}

DWORD CanBus::CanReadErrInfo(DWORD devType, DWORD devIndex, DWORD canIndex,
                     PVCI_ERR_INFO pErrInfo)
{
    DWORD ret;
    ret = VCI_ReadErrInfo(devType, devIndex, canIndex, pErrInfo);
    if(!ret)
    {
        QMessageBox::warning(NULL,"ʧ��","��ȡ������Ϣʱʧ��",QMessageBox::Ok);
    }
    return ret;
}

DWORD CanBus::CanReadCanStatus(DWORD devType, DWORD devIndex,
                     DWORD canIndex, PVCI_CAN_STATUS pCANStatus)
{
    DWORD ret;
    ret = VCI_ReadCANStatus(devType,devIndex,canIndex,pCANStatus);
    if(!ret)
    {
        QMessageBox::warning(NULL,"ʧ��","��ȡCAN״̬ʱʧ��",QMessageBox::Ok);
    }
    return ret;
}

DWORD CanBus::CanGetReference(DWORD devType, DWORD devIndex, DWORD canIndex,
                      DWORD refType,PVOID pData)
{
    DWORD ret;
    ret = VCI_GetReference(devType,devIndex,canIndex,refType,pData);
    if(!ret)
    {
        QMessageBox::warning(NULL,"ʧ��","��ȡ�豸�����Ϣʱʧ��",QMessageBox::Ok);
    }
    return ret;
}

DWORD CanBus::CanSetReference(DWORD devType, DWORD devIndex,
                      DWORD canIndex, DWORD refType,PVOID pData)
{
    DWORD ret;
    ret = VCI_SetReference(devType,devIndex,canIndex,refType,pData);
    if(!ret)
    {
        QMessageBox::warning(NULL,"ʧ��","�����豸����Ӧ����ʱʧ��",QMessageBox::Ok);
    }
    return ret;
}

ULONG CanBus::CanGetReceiveNum(DWORD devType, DWORD devIndex, DWORD canIndex)
{
    ULONG ret;

    ret = VCI_GetReceiveNum(devType,devIndex,canIndex);

    return ret;
}

DWORD CanBus::CanClearBuffer(DWORD devType, DWORD devIndex, DWORD canIndex)
{
    DWORD ret;
    ret = VCI_ClearBuffer(devType,devIndex,canIndex);
    if(!ret)
    {
        QMessageBox::warning(NULL,"ʧ��","���ָ��������ʱʧ��",QMessageBox::Ok);
    }
    return ret;
}

DWORD CanBus::CanStartCAN(DWORD devType, DWORD devIndex, DWORD canIndex)
{
    DWORD ret;
    ret = VCI_StartCAN(devType,devIndex,canIndex);
    if(!ret)
    {
        setStartorStop(false);
        QMessageBox::warning(NULL,"ʧ��","����CANʱʧ��",QMessageBox::Ok);
    }
    else
    {
        setStartorStop(true);
    }
    return ret;
}

DWORD CanBus::CanResetCAN(DWORD devType, DWORD devIndex, DWORD canIndex)
{
    DWORD ret;
    ret = VCI_ResetCAN(devType,devIndex,canIndex);
    if(!ret)
    {
        QMessageBox::warning(NULL,"ʧ��","��λCANʱʧ��",QMessageBox::Ok);
    }

    return ret;
}

ULONG CanBus::CanTransmit(DWORD devType, DWORD devIndex, DWORD canIndex,
                  PVCI_CAN_OBJ pSend,ULONG len)
{
    ULONG ret;
    ret = VCI_Transmit(devType,devIndex,canIndex,pSend,len);
    return ret;
}

ULONG CanBus::CanReceive(DWORD devType, DWORD devIndex, DWORD canIndex,
                 PVCI_CAN_OBJ pReceive,ULONG len, INT waitTime=-1)
{
    ULONG ret;
    ret = VCI_Receive(devType,devIndex,canIndex,pReceive,len,waitTime);
    return ret;
}

