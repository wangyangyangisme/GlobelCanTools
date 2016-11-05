#ifndef HEADER_H
#define HEADER_H

//Commands
#define WR_END  0x01 //
#define WR_MEM  0x02 //д����洢��
#define ER_MEM  0x03 //��������洢��
#define RUN_APP 0x04

#define GUI             ((unsigned char)0xFD)
#define BMS             ((unsigned char)0xF4)

//Function
#define CAN_MSG_IMAGE_ERASE     ((unsigned char)0xF0)
#define CAN_MSG_IMAGE_REQUEST   ((unsigned char)0xF1) //����App��������
#define CAN_MSG_IMAGE_LOAD      ((unsigned char)0xF2)
#define CAN_MSG_OVER_LOAD       ((unsigned char)0xF3) // �������ݽ�����־
#define CAN_MSG_REQ_NEXT        ((unsigned char)0xF4) // ������һ֡����
#define CAN_MSG_REQ_AGAIN       ((unsigned char)0xF5) // �������·��͸�֡����

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
#define REQ_READ_OUV        0x1828F4FD  // ��о�Ĺ���Ƿѹ����
#define REQ_READ_DLV        0x1829F4FD  // ����һ����
#define REQ_READ_DLT        0x182AF4FD  // �²�
#define REQ_READ_POV        0x182BF4FD
#define REQ_READ_PUV        0x182CF4FD

#define REQ_READ_NOR        0x1840F4FD // SOH ���δ����¼
#define REQ_FALT_1          0x1841F4FD // �����������1
#define REQ_FALT_2          0x1842F4FD // �����������2
#define REQ_FALT_3          0x1843F4FD // �����������3
#define REQ_FALT_4          0x1844F4FD // �����������4
#define REQ_FALT_5          0x1846F4FD // �����������5

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
#define REC_READ_OUV        0x1828FDF4
#define REC_READ_DLV        0x1829FDF4  // ����һ����
#define REC_READ_DLT        0x182AFDF4  // �²�
#define REC_READ_POV        0x182BFDF4
#define REC_READ_PUV        0x182CFDF4

#define REC_READ_NOR        0x1840FDF4 // SOH ���δ����¼
#define REC_FALT_1          0x1841FDF4 // �����������1
#define REC_FALT_2          0x1842FDF4 // �����������2
#define REC_FALT_3          0x1843FDF4 // �����������3
#define REC_FALT_4          0x1844FDF4 // �����������4
#define REC_FALT_5          0x1846FDF4 // �����������5

// GUI��д�ڲ�EEPROM
#define REQ_WRITE_EEPROM        0x1830F4FD
#define REQ_READ_EEPROM         0x1831F4FD

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
#define  CAN_GUI_CONFIG_POV_TH                 ((uint8_t)0x1A)
#define  CAN_GUI_CONFIG_PUV_TH                 ((uint8_t)0x1B)
#define  CAN_GUI_CONFIG_ISO_TH                 ((uint8_t)0x1C)
#define  CAN_GUI_CONFIG_PDIM_TH                ((uint8_t)0x1D)


// ������Ϣ����
#define FALT_CUV 	0xE0 // ��о��ѹ
#define FALT_COV 	0xE1 // ��о��ѹ
#define FALT_CUT	0xE2 // ������
#define FALT_COT	0xE3 // ������
#define FALT_DUT	0xE4 // �ŵ����
#define FALT_DOT	0xE5 // �ŵ����
#define FALT_COC	0xE6 // ������
#define FALT_DOC	0xE7 // �ŵ����
#define FALT_VDIF	0xE8 // ��о��ѹһ����
#define FALT_TDIF	0xE9 // �²�
#define FALT_POV 	0xEA // ����ѹ
#define FALT_PUV	0xEB // ����ѹ
#define FALT_LTC	0xEC // 6803����


#endif // HEADER_H
