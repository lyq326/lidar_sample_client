#include "configpanel/ConfigPanel.h"
#include "ConfigPanel.h"



ConfigPanel::ConfigPanel(const QString& title, QWidget* parent, Qt::WindowFlags flags)
    :QDockWidget(title, parent, flags)
{
    
    this->setObjectName(QString::fromUtf8("dockWidget"));
    // dockWidgetContents = new QWidget();
    // dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    gridLayoutWidget = new QWidget(this);
    gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
    // gridLayoutWidget->setGeometry(QRect(10, 50, 461, 81));
    gridLayout = new QGridLayout(gridLayoutWidget);
    gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
    gridLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
    gridLayout->setContentsMargins(10, 10, 10, 10);
    sboxFrameLength = new QSpinBox(gridLayoutWidget);
    sboxFrameLength->setObjectName(QString::fromUtf8("sboxFrameLength"));
    QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(sboxFrameLength->sizePolicy().hasHeightForWidth());
    sboxFrameLength->setSizePolicy(sizePolicy);

    gridLayout->addWidget(sboxFrameLength, 1, 1, 1, 1);

    leMqttIp = new QLineEdit(gridLayoutWidget);
    leMqttIp->setObjectName(QString::fromUtf8("leMqttIp"));
    sizePolicy.setHeightForWidth(leMqttIp->sizePolicy().hasHeightForWidth());
    leMqttIp->setSizePolicy(sizePolicy);

    gridLayout->addWidget(leMqttIp, 2, 1, 1, 1);

    label = new QLabel(gridLayoutWidget);
    label->setObjectName(QString::fromUtf8("label"));
    QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
    sizePolicy1.setHorizontalStretch(0);
    sizePolicy1.setVerticalStretch(0);
    sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
    label->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(label, 0, 0, 1, 1);

    label_2 = new QLabel(gridLayoutWidget);
    label_2->setObjectName(QString::fromUtf8("label_2"));
    sizePolicy1.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
    label_2->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(label_2, 1, 0, 1, 1);

    btnMqttForward = new QPushButton(gridLayoutWidget);
    btnMqttForward->setObjectName(QString::fromUtf8("btnMqttForward"));
    sizePolicy1.setHeightForWidth(btnMqttForward->sizePolicy().hasHeightForWidth());
    btnMqttForward->setSizePolicy(sizePolicy1);
    btnMqttForward->setCheckable(true);

    gridLayout->addWidget(btnMqttForward, 2, 3, 1, 1);

    comboComPort = new QComboBox(gridLayoutWidget);
    comboComPort->setObjectName(QString::fromUtf8("comboComPort"));
    sizePolicy.setHeightForWidth(comboComPort->sizePolicy().hasHeightForWidth());
    comboComPort->setSizePolicy(sizePolicy);

    gridLayout->addWidget(comboComPort, 0, 1, 1, 1);

    rbtnAutoParse = new QRadioButton(gridLayoutWidget);
    rbtnAutoParse->setObjectName(QString::fromUtf8("rbtnAutoParse"));

    gridLayout->addWidget(rbtnAutoParse, 1, 2, 1, 1);

    btnComConnect = new QPushButton(gridLayoutWidget);
    btnComConnect->setObjectName(QString::fromUtf8("btnComConnect"));
    sizePolicy1.setHeightForWidth(btnComConnect->sizePolicy().hasHeightForWidth());
    btnComConnect->setSizePolicy(sizePolicy1);
    btnComConnect->setCheckable(true);

    gridLayout->addWidget(btnComConnect, 0, 3, 2, 1);

    tbtnComSetting = new QToolButton(gridLayoutWidget);
    tbtnComSetting->setObjectName(QString::fromUtf8("tbtnComSetting"));
    sizePolicy1.setHeightForWidth(tbtnComSetting->sizePolicy().hasHeightForWidth());
    tbtnComSetting->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(tbtnComSetting, 0, 2, 1, 1);

    label_3 = new QLabel(gridLayoutWidget);
    label_3->setObjectName(QString::fromUtf8("label_3"));
    sizePolicy1.setHeightForWidth(label_3->sizePolicy().hasHeightForWidth());
    label_3->setSizePolicy(sizePolicy1);

    gridLayout->addWidget(label_3, 2, 0, 1, 1);

    gridLayout->setColumnStretch(0, 1);
    gridLayout->setColumnStretch(1, 8);
    gridLayout->setColumnStretch(2, 1);
    gridLayout->setColumnStretch(3, 1);
    this->setWidget(gridLayoutWidget);
    // MainWindow->addDockWidget(static_cast<Qt::DockWidgetArea>(1), dockWidget);

    label->setText(QApplication::translate("MainWindow", "Com", nullptr));
    label_2->setText(QApplication::translate("MainWindow", "Parse", nullptr));
    btnMqttForward->setText(QApplication::translate("MainWindow", "Forward", nullptr));
    rbtnAutoParse->setText(QApplication::translate("MainWindow", "Auto", nullptr));
    // btnComConnect->setText(QApplication::translate("MainWindow", "Connect", nullptr));
    tbtnComSetting->setText(QApplication::translate("MainWindow", "...", nullptr));
    label_3->setText(QApplication::translate("MainWindow", "MQTT", nullptr));
    

}

ConfigPanel::~ConfigPanel()
{
}