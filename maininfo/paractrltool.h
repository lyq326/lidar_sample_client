#ifndef PARACTRLTOOL_H
#define PARACTRLTOOL_H

#include <QWidget>
#include <QPushButton>
#include <QButtonGroup>
#include <QVector>
#include <QRegularExpressionValidator>
#include <QVariant>
#include <QDebug>

namespace Ui {
class ParaCtrlTool;
}

// template<typename T>
class ParaCtrlTool : public QWidget
{
    Q_OBJECT

public:
    explicit ParaCtrlTool(QWidget *parent = nullptr, qint8 numSegment = 1, QString name = "Para Ctrl", bool isExclusive = true, bool hasTextInput = true);
    ~ParaCtrlTool();

    void selectSegment(int id);
    void setCurrentValue(const QString &val);
    void setSegmentValues(const QVector<QString> &);
    void setSegmentValues(const QVector<qint32>&);
    void setSegmentValues(const QVector<float>&);
    // void setSegmentValues(const QVector<T>&);
    QVariant currentValue();
    
private:
    Ui::ParaCtrlTool *ui;
    qint8 m_numSegment;
    bool m_hasTextInput;
    bool m_isExclusive;
    bool m_useCustomVal = false;
    QVariant ValSegmentCustom;
    qint8 m_idxSegment;
    QString m_name;
    QVector<QPushButton*> vecBtnSegments;
    QVector<QVariant> vecValSegments;
    QButtonGroup btnGroupSegments;
};

#endif // PARACTRLTOOL_H
