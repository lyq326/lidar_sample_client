#ifndef MAININFOFORM_H
#define MAININFOFORM_H

#include <QWidget>
#include "instanceinfoform.h"

namespace Ui {
class MainInfoForm;
}

class MainInfoForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainInfoForm(QWidget *parent = nullptr);
    ~MainInfoForm();

    InstanceInfoForm *infoWidget;

private:
    Ui::MainInfoForm *ui;
};

#endif // MAININFOFORM_H
