#include "datasubwidget.h"
#include "ui_datasubwidget.h"

DataSubWidget::DataSubWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataSubWidget)
{
    ui->setupUi(this);

    // QFile file("../CH1_2024_02_27_20_34_22.dat");
    // bool isOk = file.open(QFile::ReadOnly);
    // if (!isOk) {
    //     QMessageBox::critical(this,"ERROR","file open failed");
    //     return;
    // }
    // // 读文件
    // QByteArray byteArray = file.readAll();
    // QVector<qint32> int32Vector;




    customPlot_pcolor = new QCustomPlot(this);
    customPlot_pcolor->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    ui->verticalLayout->addWidget(customPlot_pcolor);
    

    customPlot_chart = new QCustomPlot(this);
    customPlot_chart->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
    ui->verticalLayout_2->addWidget(customPlot_chart);

    QVector<QColor> colors= {QColorConstants::Red, QColorConstants::Green, QColorConstants::Blue, QColorConstants::Yellow};

    for(int i=0;i<4;i++){
        QPen pen;
        customPlot_chart->addGraph();
        pen.setColor(colors.at(i));
        customPlot_chart->graph()->setPen(pen);         // 设置画笔
        customPlot_chart->graph()->setName("chn"+QString::number(i));    //图例名称
        // customPlot_chart->graph()->setLineStyle((QCPGraph::LineStyle)i);  // 线段风格
        // customPlot_chart->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssNone, 5));//设置散点形状
        customPlot_chart->legend->setVisible(true); // 显示图例
    }
    // customPlot_chart->addGraph();

    customPlot_pcolor->setInteractions(QCP::iRangeDrag|QCP::iRangeZoom| QCP::iSelectPlottables);
    customPlot_pcolor->setSelectionRectMode(QCP::SelectionRectMode::srmZoom);



    colorMap = new QCPColorMap(customPlot_pcolor->xAxis, customPlot_pcolor->yAxis);

    colorMap->data()->setSize(128, 2048);
    colorMap->data()->setRange(QCPRange(0, 1), QCPRange(0, 2048));
    // for (int x=0; x<50; ++x)
    //     for (int y=0; y<2048; ++y)
    //     colorMap->data()->setCell(x, y, int32Vector[y*2]);

    
    QCPColorScale *colorScale = new QCPColorScale(customPlot_pcolor);
    customPlot_pcolor->plotLayout()->addElement(0, 1, colorScale); // 将其添加到主轴矩形的右侧
    colorScale->setType(QCPAxis::atRight); // 刻度应为垂直条，刻度线/坐标轴标签右侧（实际上，右侧已经是默认值）
    colorMap->setColorScale(colorScale); // 将颜色图与色标关联
    colorScale->axis()->setLabel("Magnetic Field Strength");

    colorMap->setGradient(QCPColorGradient::gpHues);
    // colorMap->rescaleDataRange(true);

    customPlot_pcolor->rescaleAxes();
    customPlot_pcolor->replot();

    connect(ui->pushButton_set0, &QPushButton::clicked, [=](){
        m_min_length = ui->lineEdit->text().toUInt();
        m_max_length = ui->lineEdit_2->text().toUInt();

        colorMap->data()->setValueRange(QCPRange(m_min_length, m_max_length));
        for(int i=0;i<4;i++){
            customPlot_chart->graph(i)->keyAxis()->setRange(QCPRange(m_min_length, m_max_length));
        }
        customPlot_pcolor->replot();
        customPlot_chart->replot();

    });

    ui->groupBox_0->setVisible(false);
}


void DataSubWidget::addData(const QVector<qint32> int32Vector, qint32 chn)
{
    qDebug() << int32Vector.length();
    customPlot_chart->graph(chn)->data().data()->clear();

    for (int y=0; y<int32Vector.length(); ++y){
        // colorMap->data()->setData(m_x, y, int32Vector[y]);
        customPlot_chart->graph(chn)->addData(y, int32Vector[y]);
    }

    m_x += 1;

    // colorMap->data()->setSize(m_x, 2048);
    colorMap->data()->setRange(QCPRange(0, m_x), QCPRange(0, 2048));
    // colorMap->rescaleDataRange(true);

    customPlot_pcolor->rescaleAxes();
    customPlot_pcolor->replot();

    customPlot_chart->rescaleAxes();
    customPlot_chart->replot();
}

DataSubWidget::~DataSubWidget()
{
    delete ui;
}
