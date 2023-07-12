#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // QFont f = font();
    // f.setFamily("Segoe UI");
    // setFont(f);

    // QFile qss("../StyleSheet.qss");
    // qss.open(QFile::ReadOnly);
    // this->setStyleSheet(qss.readAll());
    // qss.close();

    QWidget* p = takeCentralWidget();
    if(p)
        delete p;
    threadSerial = new ReceiverThread(this);
    dockFramePanel = new FramePanel("Frame", this);
    dockRawPanel = new QDockWidget("Raw", this);
    QWidget* dockWidgetContents = new QWidget(dockRawPanel);

    dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents1"));


    
    // QGridLayout* layout = new QGridLayout(dockWidgetContents);
    FlowLayout* layoutRaw = new FlowLayout(dockWidgetContents);

    dockRawPanel->setWidget(dockWidgetContents);


    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        this->ui->comboComPort->addItem(info.portName());

    

    

    
    for(size_t i = 0; i<40;i++){
        // ByteButton *btn = new ByteButton(QString("%1").arg(QString::number(i),2,QChar('0')));
        ByteButton *btn = new ByteButton(dockRawPanel);
        btnDataPanel.append(btn);
        // btn->setParent(dockRawPanel);
        layoutRaw->addWidget(btn);
        // layout->addWidget(btn, i/4, i%4);


        connect(btn, &ByteButton::actTriggered, this, 
            [i, this](const ByteButton* index, const QMetaType::Type t){
                this->stucData.addDataSection(QMetaType::typeName(t)+QString::number(i), t, i);
        });
        // btn->show();
    }
    m_client = new QMqttClient(this);
    this->addDockWidget(Qt::LeftDockWidgetArea, dockRawPanel);
    this->addDockWidget(Qt::RightDockWidgetArea, dockFramePanel);
    this->dockRawPanel->setAllowedAreas(Qt::AllDockWidgetAreas);
    this->dockFramePanel->setAllowedAreas(Qt::AllDockWidgetAreas);

    // updateDataPanel(QByteArray::fromRawData("\xAA\x55\x3F\x2D\xCA\x80\x27\x00\x4C\x73\x25\x00\x73\x19\x25\x00\x07\x01\x49\x00\x00\x00\x00\x00\x00\x00\x63\x04\xCF\x8A\x01\x00\xAB\x77\xFA\x26\x28\x00\xE4\x08", 40));
    stucData.addDataSection("Diff", QMetaType::UInt, 16);
    stucData.addDataSection("Ref", QMetaType::UInt, 4);
    stucData.addDataSection("Temp", QMetaType::Short, 26);
    stucData.addDataSection("Pres", QMetaType::UInt, 28);
    stucData.addDataSection("Cn2", QMetaType::Float, 32);
    stucData.addDataSection("Cnt", QMetaType::UShort, 2);

    ui->sboxFrameLength->setValue(40);
    ui->leMqttIp->setText("188.131.134.188");
    ui->rbtnAutoParse->setChecked(true);

    ui->btnComConnect->setIcon(QIcon("../res/icon/AddConnection.svg"));

    connect(ui->btnComConnect, &QPushButton::clicked, this,
        [this](){
            QString portName = this->ui->comboComPort->currentText();
            if(ui->btnComConnect->isChecked()){
                this->threadSerial->startReceiver(portName, 15, this->ui->sboxFrameLength->value(), "");
                ui->btnComConnect->setIcon(QIcon("../res/icon/Disconnect.svg"));
            }
            else{
                this->threadSerial->stopReceiver();
                ui->btnComConnect->setIcon(QIcon("../res/icon/AddConnection.svg"));
            }

            if(ui->rbtnAutoParse->isChecked()){
                connect(&stucData, &DataFrame::newSectionsVaild, dockFramePanel, &FramePanel::updateSectionsList);
            }
        }    
    );
    // threadSerial->startReceiver("COM10", 15, 40, "");

    stucData.fromFrameRaw(QByteArray::fromRawData("\xAA\x55\x3F\x2D\xCA\x80\x27\x00\x4C\x73\x25\x00\x73\x19\x25\x00\x07\x01\x49\x00\x00\x00\x00\x00\x00\x00\x63\x04\xCF\x8A\x01\x00\xAB\x77\xFA\x26\x28\x00\xE4\x08", 40));
    
    // stucData.toJsonObj();
    // 将json对象里的数据转换为字符串
    QJsonDocument doc;
    // 将object设置为本文档的主要对象
    doc.setObject(stucData.toJsonObj());
    QString str64 = doc.toJson(QJsonDocument::Compact).toBase64();

    // qDebug()<<str64.length()<<str64;
    m_client = new QMqttClient(this);
    m_mqttTimer = new QTimer(this);
    m_client->setHostname("188.131.134.188");
    m_client->setPort(1883);

    m_client->connectToHost();
    // connect(m_client,&QMqttClient::connected,this,[this](){
    //     if(m_client->state() == QMqttClient::Connected){
    //         m_client->subscribe(QMqttTopicFilter("hmtm2/+"));
    //     }
    // });

    connect(m_client,&QMqttClient::disconnected,this,[this](){
        qDebug()<<"QMqttClient::disconnected";
        if(!(m_mqttTimer->isActive())){
            m_mqttTimer->start(5000);
        }
    });
    connect(m_client,&QMqttClient::connected,this,[this](){
        qDebug()<<"QMqttClient::connected";
    });
    connect(m_mqttTimer,&QTimer::timeout,this,[this](){
        if(m_client->state() == QMqttClient::Disconnected){
            m_client->connectToHost();
        }
        else{
            m_mqttTimer->stop();
        }
        qDebug()<<"QTimer::timeout";
    });
    
    // connect(m_client,&QMqttClient::disconnected,this,[this](){
    //     qDebug()<<"QMqttClient::disconnected";
    // });

    // dockFramePanel->updateSectionsList(this->stucData.dataSections());
    connect(threadSerial, &ReceiverThread::request, this, &MainWindow::updateDataPanel);
    connect(threadSerial, &ReceiverThread::request, &stucData, &DataFrame::fromFrameRaw);
    connect(&stucData, &DataFrame::newSectionsVaild,  dockFramePanel, &FramePanel::updateSectionsList);
    connect(&stucData, &DataFrame::newSectionsVaild, this, [this](){
        QJsonDocument doc;
        // 将object设置为本文档的主要对象
        doc.setObject(stucData.toJsonObj());
        QByteArray bytes64 = doc.toJson(QJsonDocument::Compact).toBase64();
        if(m_client->state() == QMqttClient::Connected){
            // if(!((stucData.dataSections().value(0x0202)._data._d)%20)){
                m_client->publish(QMqttTopicName("hmtm2/0021/protocol"), bytes64, 2);
                m_client->publish(QMqttTopicName("hmtm2/0021/raw"), stucData.toFrameRaw(), 2);
            // }

        }
    });

}

MainWindow::~MainWindow()
{
    // delete btnDataPanel;
    delete dockFramePanel;
    // threadSerial->m_quit = true;

    // threadSerial->terminate();
    threadSerial->stopReceiver();
    delete threadSerial;
    
    delete ui;
}

void MainWindow::updateDataPanel(const QByteArray &s)
{
    QString *str = new QString(s.toHex());
    // qDebug()<<s;
    // qDebug()<<*str;
    // qDebug()<<(*str).length();
    for(auto btn = btnDataPanel.begin(); btn != btnDataPanel.end(); btn++){
        size_t i = std::distance(btnDataPanel.begin(), btn);
        (*btn)->setText(str->mid(2*i,2));
    }

    for(auto k : stucData.dataSections())
    {
        QString qss;
        switch (k._type)
        {
            case QMetaType::QChar:
            case QMetaType::QString:
            case QMetaType::QByteArray:
                qss = QString("background-color: rgb(255, 0, 0);");
                break;
            case QMetaType::SChar:
            case QMetaType::UChar:
            case QMetaType::Short:
            case QMetaType::UShort:
                qss = QString("background-color: rgb(255, 255, 0);");
                break;
            case QMetaType::Int:
            case QMetaType::UInt:
                qss = QString("background-color: rgb(0, 255, 0);");
                break;
            case QMetaType::Float:
                qss = QString("background-color: rgb(0, 255, 255);");
                break;
        }
        // auto& val = _dataSections[k];
        for(auto b: QVector(btnDataPanel.mid(k._offset, k._length))){
            b->setStyleSheet(qss);
        }
        // qDebug()<<val;

    }
    delete str;
}

void MainWindow::updateDataStore(const QByteArray &s)
{
    QString *str = new QString(s.toHex());

    this->strDataStore.append(*str);
    if(strDataStore.length()>10000)
    {
        strDataStore.remove(0, 1000);
    }
    // qDebug()<<s;
    // qDebug()<<*str;
    // qDebug()<<(*str).length();
    // for(auto btn = btnDataPanel.begin(); btn != btnDataPanel.end(); btn++){
    //     size_t i = std::distance(btnDataPanel.begin(), btn);
    //     (*btn)->setText(str->mid(2*i,2));
    // }
}