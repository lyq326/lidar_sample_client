#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MainInfoForm* infoLidar = new MainInfoForm(this);
    DataSubWidget* dataSub = new DataSubWidget();
    // NetClient* nc = new NetClient(this);
    m_sample = new Sample(this);
    m_database = new Database(this);

    ui->verticalLayout_Central->addWidget(infoLidar);
    

    qint32 laser_freq = 1000;
    qint32 bytes_per_data = infoLidar->infoWidget->m_sample_mode?4:5;

    connect(infoLidar->infoWidget, &InstanceInfoForm::sampleStop, this, [this](){
        m_sample->sampleStop();
    });

    connect(infoLidar->infoWidget, &InstanceInfoForm::sampleStart, this, [this, infoLidar, laser_freq, &bytes_per_data](){

        // bytes_per_data = infoLidar->infoWidget->m_sample_mode?4:5;

        // qint32 psc = (infoLidar->infoWidget->Resolution())/(infoLidar->infoWidget->m_sample_mode?750:1500);
        // qint32 tnum = infoLidar->infoWidget->SampleLength();
        qint32 atime = infoLidar->infoWidget->AccumulateLength()*laser_freq;

        m_sample->initSample(infoLidar->infoWidget->m_sample_mode);

        m_sample->sampleStart(infoLidar->infoWidget->Resolution(), 
            infoLidar->infoWidget->SampleLength(),
            infoLidar->infoWidget->AccumulateLength(),
            infoLidar->infoWidget->isSampleContinuous
        );

        infoLidar->infoWidget->pbSampleCnt->setMaximum(atime);
    });

    // qint32 if_chn_finish = 0;

    connect(m_sample->m_nc, &NetClient::progressUpdate, [=](qint32 cnt, quint32 chn){
        // qDebug()<<"chn"<<chn<<"cnt"<<cnt<<"in"<<infoLidar->infoWidget->Chn();
        quint32 chn_real = chn%4;
        if(infoLidar->infoWidget->Chn()&(0x01<<chn_real)){
            infoLidar->infoWidget->pbSampleCnt->setValue(cnt);
            infoLidar->infoWidget->lnSampleCnt->display(cnt);
        }

    });

    connect(m_sample, &Sample::sampleFinished, [=](){
        infoLidar->infoWidget->setSampleUI(false);
    });


    connect(m_sample, &Sample::newDataValid, [=](QDateTime time, QMap<QString, QVector<float>> map){
        // qDebug()<<data.size();
        QString str_time = time.toString("hh:mm:ss");
        infoLidar->infoWidget->lLastFrameTime->setText(str_time);

    });

    connect(infoLidar->infoWidget->btnImport, &QPushButton::clicked, [this](){
        QStringList files = QFileDialog::getOpenFileNames(this, QString("导入"), "../", QString("File(*.csv*)"));
        qDebug() << "file " << files;
        if(files.length()<=0){
            return;
        }
        QFile f(files.at(0));

        for(auto f:files){
            QFile file(f);
            importRecManual(file);
        }

        // bool retFlag = 
        
        // m_listJsonLine = txtToJson(f);
    });

    connect(m_sample, &Sample::newDataValid, [this, infoLidar, dataSub](QDateTime time, QMap<QString, QVector<float>> map){
        int groupSize = infoLidar->infoWidget->m_sample_mode?4:5;
        QVariantMap vm{};

        for(auto k:LidarData::keysRawInt){
            if(map.contains(k)){
                QVector<qint32> data_int = QVector<qint32>(map[k].begin(), map[k].end());
                QFile file("../data/"+k+"_"+time.toString("_yyyyMMdd_HHmmss")+".csv");
                if(file.open(QIODevice::WriteOnly|QIODevice::Text)){
                    QTextStream out(&file);
                    for(int i=0;i<data_int.size();i++){

                        out<<QString::number(data_int[i])<<", "<<Qt::endl;
                    }
                }
                file.close();
                // dataSub->addData(data_int, LidarData::keysRawInt.indexOf(k));

                QByteArray z_data = qCompress(QByteArray::fromRawData(
                    reinterpret_cast<const char*>(map[k].constData()),
                    sizeof(float) * map[k].size()
                ));

                // qDebug()<<k<<map[k].size()<<QByteArray::fromRawData(
                //     reinterpret_cast<const char*>(map[k].constData()),
                //     sizeof(float) * map[k].size()
                // ).length()<<z_data.length();
                vm.insert(k, z_data);

                m_database->storeToDB(LidarData::types[1], time.toMSecsSinceEpoch(), vm);
            }
        }
    });


    connect(infoLidar->infoWidget->btnDataSub, &QPushButton::clicked, this, [=](){
        dataSub->show();
        dataSub->activateWindow();
    });
}

bool MainWindow::importRecManual(QFile &f)
{
    QDateTime t = QDateTime::fromString(QFileInfo(f).baseName().right(15), "yyyyMMdd_HHmmss");
    if (!t.isValid())
    {
        qDebug() << "日期解析失败";
        QMessageBox::information(NULL, "错误", "日期解析失败！", QMessageBox::Ok);
        t = QDateTime::currentDateTime();
    }

    qint32 chn_real = QFileInfo(f).baseName().mid(2, 1).toInt();
    chn_real %= 4;
    QString key = QFileInfo(f).baseName().left(QFileInfo(f).baseName().length() - 17);

    qDebug() << f.fileName() << t;

    QVector<float> data_float;
    if (f.open(QIODevice::ReadOnly | QIODevice::Text))
    {

        while (!f.atEnd())
        {
            QByteArray line = f.readLine();
            QString str(line);
            str.remove(',');
            str.remove('\n');
            data_float.append(str.toFloat());
        }
        f.close();
        qDebug()<<data_float.length();
    }
    else
    {
        qDebug() << f.fileName() << f.errorString();
        return false;
    }
    if (!LidarData::keysRawInt.contains(key))
    {
        key = LidarData::keysRawInt.at(chn_real);
    }

    emit m_sample->newDataValid(t, QMap<QString, QVector<float>>{{key, data_float}});
    return true;
}

MainWindow::~MainWindow()
{
    
    delete ui;
}
