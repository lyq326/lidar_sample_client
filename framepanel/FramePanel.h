#ifndef FRAME_PANEL_H
#define FRAME_PANEL_H

#include <QtWidgets>
#include "framepanel/ByteButton.h"
#include "data/DataFrame.h"

class FramePanel:public QDockWidget
{
private:
    class SectionLabel:public QWidget{
        QLabel* labValue;
        QLabel* labType;
        QLineEdit* leName;
        QLineEdit* leLength;
        QHBoxLayout* layout;

        DataFrame::DataSectionValue _section;


        // QMetaType::Type _type;
        // quint32 _length;
        // QString _name;
        // QString _value;
        QString _expr;
    public:
        // explicit SectionLabel(const QString &name = "", const QMetaType::Type &type = QMetaType::Int,  QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
        explicit SectionLabel(const DataFrame::DataSectionValue &section, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
        ~SectionLabel();

        void updateData(const DataFrame::DataSectionValue &section);
    };

    QVBoxLayout* layout;
    QWidget* dockWidgetContents;
    QTreeWidget* twSections;
    QVector<ByteButton*> btnArrayBytes;
    QVector<SectionLabel*> labListSections;
    QListWidget* listSections;
    QVector<QTreeWidgetItem*> twiListSections;
    /* data */
public:
    explicit FramePanel(const QString &title, QWidget *parent = nullptr,
                         Qt::WindowFlags flags = Qt::WindowFlags());
    // explicit FramePanel(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~FramePanel();
    
    void updateBytesArray(const QByteArray &s);
    void updateSectionsList(const DataFrame::DataSectionsType &s);
    void addSectionLabel(const DataFrame::DataSectionValue &section);



};
#endif