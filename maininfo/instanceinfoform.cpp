#include "instanceinfoform.h"
#include "ui_instanceinfoform.h"

InstanceInfoForm::InstanceInfoForm(QWidget *parent, bool sample_mode) :
    QWidget(parent),
    ui(new Ui::InstanceInfoForm)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(1);

    m_sample_mode = sample_mode;

    pbSampleCnt = ui->progressBar;
    lnSampleCnt = ui->lcdNumber_2;
    lLastFrameTime = ui->label_lastframetime;
    btnDataSub = ui->pushButton_sub;
    btnImport = ui->pushButton;

    pbSampleCnt->setValue(0);

    ui->pushButton_sample->setCheckable(true);

    // connect(ui->checkBox, &QCheckBox::stateChanged, this, [=](){
    //     isSampleContinuous = ui->checkBox->isChecked();
    // });

    // ctMode = new ParaCtrlTool(this, 4, "采集模式", true, false);
    // drawSampleOptions();
    // connect(ui->horizontalSlider_Resolution())

    QTimer *timerNowTime = new QTimer(this);
    connect(timerNowTime, &QTimer::timeout, this, [=](){
        QDateTime time = QDateTime::currentDateTime();
        QString str = time.toString("hh:mm:ss");
        this->ui->lcdNumber_ctime->display(str);

    });
    timerNowTime->start(1000);

    connect(ui->pushButton_sample, &QPushButton::clicked, this, [=](bool checked){
        qDebug()<<"sampleStartStop "<<(!isSampleing)<<" Manual by QPushButton";
        isSampleContinuous = ui->checkBox->isChecked();
        sampleStartStop(checked);
    });

    connect(ui->pushButtonModeAna, &QPushButton::clicked, this, [&](){
        m_sample_mode = false;
        drawSampleOptions();
        // ui->stackedWidget->setCurrentIndex(0);
    });

    connect(ui->pushButtonModePhoton, &QPushButton::clicked, this, [&](){
        m_sample_mode = true;
        drawSampleOptions();
        ui->stackedWidget->setCurrentIndex(0);
    });
}

void InstanceInfoForm::drawSampleOptions()
{
    ctChn = new ParaCtrlTool(this, 4, "通道选择", false, false);
    ctResolution = new ParaCtrlTool(this, (m_sample_mode?4:5), "分辨率(M)");
    ctALength = new ParaCtrlTool(this, 6, "累加长度(s)");
    ctSLength = new ParaCtrlTool(this, 4, "采集长度(点)");
    ctThreshold = new ParaCtrlTool(this, 0, "触发阈值(V)");

    if (m_sample_mode)
    {
        ctResolution->setSegmentValues((QVector<float>){3.75f, 7.5f, 15.0f, 30.0f});
        ctResolution->selectSegment(2);
    }
    else
    {
        ctResolution->setSegmentValues((QVector<float>){1.5f, 3.0f, 6.0f, 7.5f, 15.0f});
        ctResolution->selectSegment(4);
    }

    ctChn->setSegmentValues((QVector<qint32>){0, 1, 2, 3});
    ctALength->setSegmentValues((QVector<qint32>){1, 10, 30, 60, 90, 180});
    ctSLength->setSegmentValues((QVector<qint32>){1000, 2000, 4000, 8000});

    ctChn->selectSegment(0);
    ctALength->selectSegment(1);
    ctSLength->selectSegment(1);
    ctThreshold->setCurrentValue("1.65");

    // updateResolution(15);
    // updateAccumulateLength(30);
    // updateSampleLength(2048);

    ui->verticalLayout_gb->insertWidget(0, ctThreshold);
    ui->verticalLayout_gb->insertWidget(0, ctALength);
    ui->verticalLayout_gb->insertWidget(0, ctSLength);
    ui->verticalLayout_gb->insertWidget(0, ctResolution);
    ui->verticalLayout_gb->insertWidget(0, ctChn);
}

void InstanceInfoForm::sampleStartStop(bool en){
    if(en){
        emit sampleStart();
    }
    else{
        emit sampleStop();
    }

    setSampleUI(en);
}

void InstanceInfoForm::setSampleUI(bool en)
{
    QString str_btn = en ? "停止采集" : "采集";
    isSampleing = en;

    
    ui->pushButton_sample->setChecked(en);
    ui->pushButton_sample->setText(str_btn);
    for (auto i = 0; i < 4; i++)
    {
        QWidget *w = ui->verticalLayout_gb->itemAt(i)->widget();
        if (w != nullptr)
        {
            w->setDisabled(en);
        }
    }
}

InstanceInfoForm::~InstanceInfoForm()
{
    delete ui;
}

int InstanceInfoForm::Threshold()
{
    return ctThreshold->currentValue().toDouble()*1000;
}

int InstanceInfoForm::Resolution()
{
    return ctResolution->currentValue().toDouble();
}

int InstanceInfoForm::Chn()
{
    return ctChn->currentValue().toInt();
}

int InstanceInfoForm::SampleLength()
{
    return ctSLength->currentValue().toInt();
}

int InstanceInfoForm::AccumulateLength()
{
    return ctALength->currentValue().toInt();
}
