#ifndef PARSEHEX_H
#define PARSEHEX_H

#include <QObject>
#include <QString>
#include <QFile>

class ParseHex : public QObject
{
    Q_OBJECT
public:
    explicit ParseHex(QObject *parent = 0);
    explicit ParseHex(QString fileName,QString usrid,
                      QString configBits, QString endofhex,
                      QObject *parent = 0);
    ~ParseHex();

    unsigned short GetAddr(QString &line);
    bool LinePutToArray(QString &line);    //��һ������ת�������m_binData
    bool StartParse(void);
    unsigned long GetByteNums(){return m_byteNums;} //�������StartParse֮�����
    unsigned char GetBinData(unsigned int i){return m_binData[i];}

private:
    QFile           *m_file; //Hex�ļ�
    QByteArray      m_binData;
    const QString   m_hexFileName;
    QString         m_userId;
    QString         m_configBits;
    QString         m_endOfHex;
    unsigned long   m_byteNums;   //hex�ļ�ת������ֽ���
    
signals:
    
public slots:
    
};

#endif // PARSEHEX_H
