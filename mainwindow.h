#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QSerialPortInfo>
#include "maininfo\maininfoform.h"
#include "datasub\datasubwidget.h"
#include "../sample_lib/Database/database.h"

#include <qDebug>
// #include <QtMqtt>
// #include "netclient\netclient.h"



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    bool importRecManual(QFile &f);
    void writeDataPerChn(qint32 sample_chn, bool sample_mode, NetClient *nc, const QPair<NetClient::PayloadType, qint32> &p);
    ~MainWindow();

    Sample* m_sample;
    Database* m_database;

private:
    Ui::MainWindow *ui;
    // void writeDataPerChn(qint32 sample_chn, bool sample_mode, NetClient *nc, QPair<NetClient::PayloadType, qint32> &p);
};
#endif // MAINWINDOW_H
