#ifndef DATASUBWIDGET_H
#define DATASUBWIDGET_H

#include <QWidget>
#include "QCustomPlot\qcustomplot.h"

namespace Ui {
class DataSubWidget;
}

class DataSubWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DataSubWidget(QWidget *parent = nullptr);
    void addData(const QVector<qint32>, qint32 chn);
    ~DataSubWidget();

    qint32 m_x = 0;
    qint32 m_min_length = 0;
    qint32 m_max_length = 1024;    
    QCPColorMap *colorMap;
    QCustomPlot *customPlot_pcolor;
    QCustomPlot *customPlot_chart;

private:
    Ui::DataSubWidget *ui;
};

#endif // DATASUBWIDGET_H
