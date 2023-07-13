#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QSerialPortInfo>
#include "rawpanel/flowlayout.h"
#include "serial/ReceiverThread.h"
#include "data/DataFrame.h"
#include "framepanel/ByteButton.h"
#include "framepanel/FramePanel.h"
#include "configpanel/ConfigPanel.h"
// #include <qDebug>
#include <QtMqtt>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateDataPanel(const QByteArray &s);
    void updateDataStore(const QByteArray &s);

private:
    Ui::MainWindow *ui;
    QVector<ByteButton*> btnDataPanel;
    ReceiverThread *threadSerial;
    QVector<QString> strDataStore;
    DataFrame stucData;
    FramePanel* dockFramePanel;
    QDockWidget* dockRawPanel;
    ConfigPanel* dockConfigPanel;

    QMqttClient* m_client;
    QTimer* m_mqttTimer;
};
#endif // MAINWINDOW_H
