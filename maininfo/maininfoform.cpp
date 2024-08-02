#include "maininfoform.h"
#include "ui_maininfoform.h"

MainInfoForm::MainInfoForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainInfoForm)
{
    ui->setupUi(this);

    
    infoWidget = new InstanceInfoForm(this);
    infoWidget->setObjectName(QString::fromUtf8("infoWidget"));

    ui->verticalLayout->addWidget(infoWidget);
    ui->tabWidget->setVisible(false);
    // infoWidget->setVisible(false);

    // connect(&infoWidget->ui->pushButton_sample)
}

MainInfoForm::~MainInfoForm()
{
    delete ui;
}
