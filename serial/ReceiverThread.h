#ifndef R_THREAD_H
#define R_THREAD_H

#include <QThread>
#include <QMutex>
#include <QSerialPort>
#include <QTime>

class ReceiverThread : public QThread
{
    Q_OBJECT

public:
    // explicit ReceiverThread(QObject *parent = nullptr);
    // ~ReceiverThread();
    using QThread::QThread;
    // using QThread::~QThread;


    void startReceiver(const QString &portName, int waitTimeout, int frameLen, const QString &response); 
    void stopReceiver();
    
signals:
    void request(const QByteArray &s);
    void error(const QString &s);
    void timeout(const QString &s);

private:
    void run() override;

    QString m_portName;
    QString m_response;
    int m_waitTimeout = 10;
    int m_frameLen = 40;
    QMutex m_mutex;
    bool m_quit;
};


#endif