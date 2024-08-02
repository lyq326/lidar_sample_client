#include "netclient.h"

NetClient::NetClient(QObject *parent, QHostAddress ip) : QObject(parent), m_ipaddr(ip)
{
    

    if(!m_udp_ctrl.bind(9000)){
        qDebug()<<m_udp_ctrl.error();
    };
    

    connect(&m_udp_ctrl, &QUdpSocket::readyRead, this, [=](){
        QByteArray data;
        data.resize(m_udp_ctrl.pendingDatagramSize());
        QHostAddress addr;
        quint16 port;
        m_udp_ctrl.readDatagram(data.data(),data.size(),&addr,&port);

        quint8 ctrl = data[0]&0x0f;
        quint8 chn = (data[0]&0xf0)>>5;
        bool intok;
        quint32 payload = QString(data.mid(1, 4).toHex()).toInt(&intok, 16);

        if(ctrl == 0x08){
            // qDebug()<<QString("通道%1进度:%2").arg(chn).arg(payload);
            if(payload != 0){
                emit progressUpdate(payload, chn);
            }
        }
        // qDebug()<<

    });

    m_udp_data_buffer.resize(8);

    for(int i = 0;i<8;i++){
    // int i = 4;
        m_udplist_data.append(new QUdpSocket(this));
        m_udplist_data.at(i)->bind(9001+i);

        connect(m_udplist_data.at(i), &QUdpSocket::readyRead, this, [=](){
            QUdpSocket *m_udp=qobject_cast<QUdpSocket *>(sender());
            QByteArray data;
            data.resize(m_udp->pendingDatagramSize());
            QHostAddress addr;
            quint16 port;
            // m_udp->read(10240);
            m_udp->readDatagram(data.data(),data.size(),&addr,&port);
            m_udp_data_buffer[i] += data;
            // qDebug()<<9001+i<<": "<<m_udp_data_buffer[i].size()<<" of "<<m_current_rx_length;

            if(m_udp_data_buffer[i].size() == m_current_rx_length){
                qDebug()<<"chn"<<i<<"sampleDataRxFinish";
                emit sampleDataRxFinish(m_udp_data_buffer[i], i);
                
            }
        });
    }



    // m_timer_progress_query.start(1000);

}


void NetClient::startProgressTimer(void)
{
    m_timer_progress_query.start(200);
}
void NetClient::startSampleDataRx(quint32 length)
{
    // m_timer_progress_query.start(1000);
    m_current_rx_length = length;
    // for(auto i =  0; i<8; i++){
    //     m_udp_data_buffer[i].resize(0);
    // }
    for(auto& d:m_udp_data_buffer){
        d.resize(0);
    }
    // qDebug()<<"clear transform bytearray"<<m_udp_data_buffer[0].size();
}
void NetClient::stopProgressTimer(void)
{
    m_timer_progress_query.stop();
}

QByteArray NetClient::getPayload(quint8 chn, PayloadType pType, qint32 dat){
    qint32 dat_be = qToBigEndian(dat);
    // qint8 cPType = pType|((0x2u*chn)<<4u);
    qint8 cPType = pType|((0x1u*chn)<<4u);
    QByteArray payload = QByteArray::fromRawData(reinterpret_cast<const char*>(&cPType), 1)
        +QByteArray::fromRawData(reinterpret_cast<const char*>(&dat_be), 4);
    payload+=calCRC16(payload);
    // qDebug()<<"getPayload:"<<QString::number(chn, 16)<<pType<<dat;
    return payload;
}

qint64 NetClient::writePrivateDatagram(quint8 chn, PayloadType pType, qint32 dat)
{
    return m_udp_ctrl.writeDatagram(getPayload(chn,pType, dat), m_ipaddr, 9000);
}

QByteArray NetClient::calCRC16(QByteArray in)
{
    uint8_t* data = reinterpret_cast<uint8_t*>(in.data());
    quint32 size = in.size();
    uint32_t crc = 0;
    const uint8_t* dataEnd = data + size;
    while(data < dataEnd)
    crc = updateCRC16(crc, *data++);
    crc = updateCRC16(crc, 0);
    crc = updateCRC16(crc, 0);
    quint32 crc_be = qToBigEndian(crc & 0xffffu);
    return QByteArray::fromRawData(reinterpret_cast<const char*>(&crc_be), 4).right(2);

}

quint32 NetClient::updateCRC16(quint16 crcIn, quint8 byte){
    uint32_t crc = crcIn;
    uint32_t in = byte | 0x100;
    do
    {
        crc <<= 1;
        in <<= 1;
        if(in & 0x100)
        ++crc;
        if(crc & 0x10000)
        crc ^= 0x1021;
    }
    while(!(in & 0x10000));
    return crc & 0xffffu;

}
