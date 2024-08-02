#ifndef NETCLIENT_H
#define NETCLIENT_H

#include <QObject>
#include <QWidget>
#include <QHostAddress>
#include <QUdpSocket>
#include <QDebug>
#include <QTimer>
#include <QtEndian>
#include <QMessageBox>

class NetClient : public QObject
{
    Q_OBJECT
// private:
public:
    QTimer m_timer_progress_query;
    QUdpSocket m_udp_ctrl;
    QList<QUdpSocket*> m_udplist_data;
    QHostAddress m_ipaddr;

    enum PayloadType : uint8_t{
        Prescaler = 0,
        AccTimes = 1,
        TriggerNum,
        Source,
        TriggerMode,
        TriggerDelay = 6,
        SampleEnable,
        AccCntQuery,
        Range,
        AccDataQuery
    };
    Q_ENUM(PayloadType)

    explicit NetClient(QObject *parent = nullptr, QHostAddress ip = QHostAddress("192.168.0.56"));

    void startProgressTimer(void);
    void startSampleDataRx(quint32 length);
    void stopProgressTimer(void);

    QByteArray getPayload(quint8 chn, PayloadType pType, qint32 dat);
    qint64 writePrivateDatagram(quint8 chn, PayloadType pType, qint32 dat);
    
    QByteArray calCRC16(QByteArray in);
    quint32 updateCRC16(quint16 crcIn, quint8 byte);

    QVector<QByteArray> m_udp_data_buffer;
    quint32 m_current_rx_length;
    quint32 m_chn_finished = 0;
signals:
    void progressUpdate(qint32 pgCnt, qint32 chn);
    void sampleDataRxFinish(const QByteArray s_data, qint32 chn);
};

#endif // NETCLIENT_H
