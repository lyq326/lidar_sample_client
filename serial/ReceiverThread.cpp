#include "serial/ReceiverThread.h"

    // explicit ReceiverThread::ReceiverThread(QObject *parent):QThread(parent){};
    // ~ReceiverThread();

void ReceiverThread::startReceiver(const QString& portName, int waitTimeout, int frameLen, const QString& response)
{
    const QMutexLocker locker(&m_mutex);
    m_portName = portName;
    m_waitTimeout = waitTimeout;
    m_response = response;
    m_frameLen = frameLen;
    m_quit = false;
    if (!isRunning())
        start();
}

void ReceiverThread::stopReceiver()
{
    const QMutexLocker unlocker(&m_mutex);

    m_quit = true;
    if (isRunning()){
        quit();
        wait();
    }
}

void ReceiverThread::run()
{
    bool currentPortNameChanged = false;

    // m_mutex.lock();
    // QString currentPortName;
    // if (currentPortName != m_portName) {
    //     currentPortName = m_portName;
    //     currentPortNameChanged = true;
    // }

    // int currentWaitTimeout = m_waitTimeout;
    // QString currentRespone = m_response;
    // m_mutex.unlock();
    QSerialPort serial;
    QByteArray requestData;


    serial.close();
    serial.setPortName(m_portName);
    // serial.setBaudRate(serial.Baud115200);
    serial.setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections);//设置波特率和读写方向
    serial.setDataBits(QSerialPort::Data8);		//数据位为8位
    serial.setFlowControl(QSerialPort::NoFlowControl);//无流控制
    serial.setParity(QSerialPort::NoParity);	//无校验位
    serial.setStopBits(QSerialPort::OneStop); //一位停止位

    serial.setReadBufferSize(200);


    if (!serial.open(QIODevice::ReadWrite)) {
        emit error(tr("Can't open %1, error code %2")
            .arg(m_portName).arg(serial.error()));
        return;
    }
    

    while (!m_quit) {
        // if (currentPortNameChanged) {
        //     serial.close();
        //     serial.setPortName(currentPortName);

        //     if (!serial.open(QIODevice::ReadWrite)) {
        //         emit error(tr("Can't open %1, error code %2")
        //             .arg(m_portName).arg(serial.error()));
        //         return;
        //     }
        // }

        if (serial.waitForReadyRead(m_waitTimeout)) {
            // read request
            if(serial.bytesAvailable()>=m_frameLen){
                requestData = serial.read(m_frameLen);
                // if((requestData[0] = 0xaa)&&(requestData[0] = 0xaa))
                if(requestData.length() == m_frameLen)
                    if((requestData[0] = 0xaa)&&(requestData[1] = 0x55)){
                        emit this->request(requestData);
                    }
                    else{
                        serial.clear();
                    }
                else{
                    serial.read(m_frameLen - requestData.length());
                }
            }

            // while (serial.waitForReadyRead(10))
            //     requestData += serial.readAll();
        }
        else {
            emit timeout(tr("Wait read request timeout %1")
                .arg(QTime::currentTime().toString()));
        }
        // write response
        // const QByteArray responseData = currentRespone.toUtf8();
        // serial.write(responseData);
        // if (serial.waitForBytesWritten(m_waitTimeout)) {
        //     const QString request = QString::fromUtf8(requestData);
        //     emit this->request(request);
        // }
        // else {
        //     emit timeout(tr("Wait write response timeout %1")
        //         .arg(QTime::currentTime().toString()));
        // }
        
    }
}