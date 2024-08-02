#ifndef INSTANCEINFOFORM_H
#define INSTANCEINFOFORM_H

#include <QtWidgets>
#include <QTimer>
#include <QDateTime>
#include "paractrltool.h"
#include "../sample_lib/sample.h"

namespace Ui {
class InstanceInfoForm;
}

class InstanceInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit InstanceInfoForm(QWidget *parent = nullptr, bool sample_mode = false);
    void drawSampleOptions();
    void sampleStartStop(bool en);
    void setSampleUI(bool en);
    ~InstanceInfoForm();

    int Threshold();
    int Resolution();
    int Chn();
    int SampleLength();
    int AccumulateLength();    
    bool isSampleing = false;

    bool m_sample_mode = false; // ModePhoton: true      ModeAna: false

    



    int m_sample_res = 1500;
    int m_sample_slength = 2048;
    int m_sample_alength = 10000;

    QProgressBar* pbSampleCnt;
    QPushButton* btnDataSub;
    QPushButton* btnImport;
    QLCDNumber* lnSampleCnt;
    QLabel* lLastFrameTime;
    
    bool isSampleContinuous;


signals:
    void sampleSwitch(bool);
    void sampleRestart();
    void sampleStart();
    void sampleStop();



private:
    Ui::InstanceInfoForm *ui;

    ParaCtrlTool* ctResolution, *ctChn;
    ParaCtrlTool* ctALength;
    ParaCtrlTool* ctSLength;
    ParaCtrlTool* ctThreshold;
};

#endif // INSTANCEINFOFORM_H
