#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QWidget* p = takeCentralWidget();
    if(p)
        delete p;
    threadSerial = new ReceiverThread(this);
    dockFramePanel = new FramePanel("Frame", this);
    dockRawPanel = new QDockWidget("Raw", this);
    QWidget* dockWidgetContents = new QWidget(dockRawPanel);

    dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents1"));

    FlowLayout* layoutRaw = new FlowLayout(dockWidgetContents);

    dockRawPanel->setWidget(dockWidgetContents);

    dockConfigPanel = new ConfigPanel("Config", this);

    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
        this->dockConfigPanel->comboComPort->addItem(info.portName());

    

    

    
    for(size_t i = 0; i<40;i++){
        ByteButton *btn = new ByteButton(dockRawPanel);
        btnDataPanel.append(btn);

        layoutRaw->addWidget(btn);


        connect(btn, &ByteButton::actTriggered, this, 
            [i, this](const ByteButton* index, const QMetaType::Type t){
                this->stucData.addDataSection(QMetaType::typeName(t)+QString::number(i), t, i);
        });
    }
    m_client = new QMqttClient(this);
    this->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockConfigPanel);
    this->addDockWidget(Qt::LeftDockWidgetArea, dockRawPanel);
    this->addDockWidget(Qt::RightDockWidgetArea, dockFramePanel);

    this->dockConfigPanel->setAllowedAreas(Qt::AllDockWidgetAreas);
    this->dockRawPanel->setAllowedAreas(Qt::AllDockWidgetAreas);
    this->dockFramePanel->setAllowedAreas(Qt::AllDockWidgetAreas);

    // updateDataPanel(QByteArray::fromRawData("\xAA\x55\x3F\x2D\xCA\x80\x27\x00\x4C\x73\x25\x00\x73\x19\x25\x00\x07\x01\x49\x00\x00\x00\x00\x00\x00\x00\x63\x04\xCF\x8A\x01\x00\xAB\x77\xFA\x26\x28\x00\xE4\x08", 40));
    stucData.addDataSection("Diff", QMetaType::UInt, 16);
    stucData.addDataSection("Ref", QMetaType::UInt, 4);
    stucData.addDataSection("Temp", QMetaType::Short, 26);
    stucData.addDataSection("Pres", QMetaType::UInt, 28);
    stucData.addDataSection("Cn2", QMetaType::Float, 32);
    stucData.addDataSection("Cnt", QMetaType::UShort, 2);

    this->dockConfigPanel->sboxFrameLength->setValue(40);
    this->dockConfigPanel->leMqttIp->setText("188.131.134.188");
    this->dockConfigPanel->rbtnAutoParse->setChecked(true);

    this->dockConfigPanel->btnComConnect->setIcon(QIcon("/home/kylin/lib/vs2022_img_lib/images/Link.svg"));
    this->dockConfigPanel->btnComConnect->setIconSize(QSize(20, 20));
    // this->dockConfigPanel->btnComConnect->setIcon(QIcon("../res/icon/AddConnection.svg"));
    connect(this->dockConfigPanel->btnComConnect, &QPushButton::clicked, this,
        [this](){
            QString portName = this->dockConfigPanel->comboComPort->currentText();
            if(this->dockConfigPanel->btnComConnect->isChecked()){
                this->threadSerial->startReceiver(portName, 15, this->dockConfigPanel->sboxFrameLength->value(), "");
                this->dockConfigPanel->btnComConnect->setIcon(QIcon("/home/kylin/lib/vs2022_img_lib/images/Unlink.svg"));
            }
            else{
                this->threadSerial->stopReceiver();
                this->dockConfigPanel->btnComConnect->setIcon(QIcon("/home/kylin/lib/vs2022_img_lib/images/Link.svg"));
            }

            if(this->dockConfigPanel->rbtnAutoParse->isChecked()){
                connect(&stucData, &DataFrame::newSectionsVaild, dockFramePanel, &FramePanel::updateSectionsList);
            }
        }    
    );

    // stucData.fromFrameRaw(QByteArray::fromRawData("\xAA\x55\x3F\x2D\xCA\x80\x27\x00\x4C\x73\x25\x00\x73\x19\x25\x00\x07\x01\x49\x00\x00\x00\x00\x00\x00\x00\x63\x04\xCF\x8A\x01\x00\xAB\x77\xFA\x26\x28\x00\xE4\x08", 40));
    
    QJsonDocument doc;

    doc.setObject(stucData.toJsonObj());
    QString str64 = doc.toJson(QJsonDocument::Compact).toBase64();

    // qDebug()<<str64.length()<<str64;
    m_client = new QMqttClient(this);
    m_mqttTimer = new QTimer(this);
    m_client->setHostname("188.131.134.188");
    m_client->setPort(1883);

    m_client->connectToHost();

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

    connect(threadSerial, &ReceiverThread::request, this, &MainWindow::updateDataPanel);
    connect(threadSerial, &ReceiverThread::request, &stucData, &DataFrame::fromFrameRaw);
    connect(&stucData, &DataFrame::newSectionsVaild,  dockFramePanel, &FramePanel::updateSectionsList);
    connect(&stucData, &DataFrame::newSectionsVaild, this, [this](){
        QJsonDocument doc;
        doc.setObject(stucData.toJsonObj());
        QByteArray bytes64 = doc.toJson(QJsonDocument::Compact).toBase64();
        if(m_client->state() == QMqttClient::Connected){
                m_client->publish(QMqttTopicName("hmtm2/0021/protocol"), bytes64, 2);
                m_client->publish(QMqttTopicName("hmtm2/0021/raw"), stucData.toFrameRaw(), 2);
        }
    });

}

MainWindow::~MainWindow()
{
    delete dockFramePanel;

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
}