#include "paractrltool.h"
#include "ui_paractrltool.h"

// template <typename T>
ParaCtrlTool::ParaCtrlTool(QWidget *parent, qint8 numSegment, QString name, bool isExclusive, bool hasTextInput) :
    QWidget(parent),
    m_name(name),
    m_numSegment(numSegment),
    m_isExclusive(isExclusive),
    m_hasTextInput(hasTextInput),
    ui(new Ui::ParaCtrlTool)
{
    ui->setupUi(this);

    if(!m_hasTextInput){
        ui->lineEdit->setHidden(true);
    }
    

    ui->label->setText(name);
    ui->lineEdit->setValidator(new QRegularExpressionValidator(QRegularExpression("^[1-9]\\d*\\.\\d+$|^0\\.\\d+$|^[1-9]\\d*$|^0$")));

    btnGroupSegments.setExclusive(m_isExclusive);
    vecValSegments.resize(numSegment);
    for(qint8 i = 0;i<numSegment; i++){
        auto pb = new QPushButton(this);
        pb->setMinimumWidth(30);
        pb->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        // pb->setMaximumWidth(50);
        vecBtnSegments.append(pb);
        ui->horizontalLayout_btn->addWidget(vecBtnSegments[i]);
        btnGroupSegments.addButton(vecBtnSegments[i], i);
        vecBtnSegments[i]->setCheckable(true);
        // vecValSegments.append(QVariant());
        // vecBtnSegments[i]->setAutoExclusive(true);
    }

    connect(ui->lineEdit, &QLineEdit::editingFinished, this, [=](){
        qreal val = ui->lineEdit->text().toDouble();
        qDebug()<<ui->lineEdit->text()<<val;
        // bool hasMatch = false;
        m_useCustomVal = true;
        for(auto i = 0;i<vecValSegments.size();i++){
            if(val == vecValSegments[i].toDouble()){
                selectSegment(i);
                m_useCustomVal = false;
                break;
            }
        }
        if(m_useCustomVal){
            ValSegmentCustom.setValue(val);
        }
    });
    connect(&btnGroupSegments, &QButtonGroup::idClicked, this, [=](int id){
        ui->lineEdit->setText(this->vecBtnSegments[id]->text());
        m_useCustomVal = false;
        if(isExclusive){
            m_idxSegment = id;            
        }
        else{
            if(this->vecBtnSegments[id]->isChecked()){
                m_idxSegment |= (1<<id);
            }
            else{
                m_idxSegment &= ~(1<<id);
            }
                
        }

    });

}

ParaCtrlTool::~ParaCtrlTool()
{

    delete ui;
}

void ParaCtrlTool::selectSegment(int id){
    btnGroupSegments.button(id)->click();
}

void ParaCtrlTool::setCurrentValue(const QString& val){
    ui->lineEdit->setText(val);
    
    QMetaObject::invokeMethod(ui->lineEdit, "editingFinished", Qt::QueuedConnection);
}

void ParaCtrlTool::setSegmentValues(const QVector<qint32> & values)
{
    for(qint8 i = 0;i<vecBtnSegments.length();i++){
        if(i<values.length()){
            // vecBtnSegments[i]->setText(QString::number(values[i]));
            vecValSegments[i].setValue(values[i]);
            vecBtnSegments[i]->setText(vecValSegments[i].toString());
        }
    }
}

void ParaCtrlTool::setSegmentValues(const QVector<float> & values)
{
    for(qint8 i = 0;i<vecBtnSegments.length();i++){
        if(i<values.length()){
            // vecBtnSegments[i]->setText(QString::number(values[i],'g', 3));
            vecValSegments[i].setValue(values[i]);
            vecBtnSegments[i]->setText(vecValSegments[i].toString());
        }
    }
}

QVariant ParaCtrlTool::currentValue()
{
    // qDebug()<<m_useCustomVal<<ValSegmentCustom;
    if(m_useCustomVal){
        return ValSegmentCustom;
    }
    else{
        if(m_isExclusive){
            return vecValSegments[m_idxSegment];
        }
        else{
            return QVariant(m_idxSegment);
        }
    }

}
