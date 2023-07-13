#ifndef CONFIG_PANEL_H
#define CONFIG_PANEL_H

#include <QtWidgets>


class ConfigPanel:public QDockWidget
{
private:
    
public:
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QSpinBox *sboxFrameLength;
    QLineEdit *leMqttIp;
    QLabel *label;
    QLabel *label_2;
    QPushButton *btnMqttForward;
    QComboBox *comboComPort;
    QRadioButton *rbtnAutoParse;
    QPushButton *btnComConnect;
    QToolButton *tbtnComSetting;
    QLabel *label_3;

    explicit ConfigPanel(const QString &title, QWidget *parent = nullptr,
                         Qt::WindowFlags flags = Qt::WindowFlags());
    // explicit FramePanel(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~ConfigPanel();


};
#endif