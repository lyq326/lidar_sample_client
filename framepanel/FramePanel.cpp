#include "framepanel/FramePanel.h"

FramePanel::FramePanel(const QString &title, QWidget *parent, Qt::WindowFlags flags)
    :QDockWidget(title, parent, flags)
{
    dockWidgetContents = new QWidget(this);
    // listSections = new QListWidget();
    dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
    this->setWidget(dockWidgetContents);
    layout = new QVBoxLayout(dockWidgetContents);
    // dockWidgetContents->setLayout(layout);

    twSections = new QTreeWidget();
    twSections->setColumnCount(4);
    twSections->setHeaderLabels(QStringList()<<"Name"<<"Length"<<"Type"<<"Value");
    twSections->header()->setSectionResizeMode(QHeaderView::Stretch);
    twSections->setRootIsDecorated(false);
    twSections->setIconSize(QSize(24, 24));
    twSections->setFont(QFont("Segoe UI", 9));
    layout->addWidget(twSections);

}

FramePanel::~FramePanel()
{
}

void FramePanel::updateBytesArray(const QByteArray &s)
{
    QString *str = new QString(s.toHex());
    // qDebug()<<s;
    // qDebug()<<*str;
    // qDebug()<<(*str).length();
    for(auto btn = this->btnArrayBytes.begin(); btn != this->btnArrayBytes.end(); btn++){
        size_t i = std::distance(this->btnArrayBytes.begin(), btn);
        (*btn)->setText(str->mid(2*i,2));
    }
}

void FramePanel::addSectionLabel(const DataFrame::DataSectionValue &section){
    // SectionLabel* sl= new SectionLabel(section);
    // labListSections.append(sl);

    // QListWidgetItem* item = new QListWidgetItem(this->listSections);
    // this->listSections->addItem(item);
    // this->listSections->setItemWidget(item, sl);

    QTreeWidgetItem* twItem = new QTreeWidgetItem(twSections);
    twiListSections.append(twItem);
    twSections->addTopLevelItem(twItem);

    // listSections->insertItem()

}

void FramePanel::updateSectionsList(const DataFrame::DataSectionsType &s)
{
    // auto it = labListSections.cbegin();

    // if(labListSections.length() != s.keys().length())
    // {
    //     listSections->clear();
    //     labListSections.clear();
    //     for(auto& d : s )
    //     {
    //         this->addSectionLabel(d); 
    //     }
    // }

    if(twiListSections.length() != s.keys().length())
    {
        // twSections->clear();
        twiListSections.clear();
        for(auto& d : s )
        {
            this->addSectionLabel(d);
        }
    }

    size_t i = 0;
    for(auto& d : s )
    {
        twiListSections[i]->setText(0, d._name);
        twiListSections[i]->setText(1, QString::number(d._length));
        twiListSections[i]->setText(2, QMetaType::typeName(d._type));
        twiListSections[i]->setText(3, d.toString());
        twiListSections[i]->setIcon(0, QIcon("../res/icon/Field.svg"));
        // twiListSections[i]->setIcon()
        i+=1;
    }
}


// explicit FramePanel::SectionLabel::SectionLabel(
//     const QString &name, const QMetaType::Type &type,  QWidget* parent, Qt::WindowFlags f)
// :QWidget(parent, f){
//     _section
// };

// explicit FramePanel::SectionLabel::SectionLabel(
//     const DataFrame::DataSectionValue &section, QWidget* parent, Qt::WindowFlags f)
// :FramePanel::SectionLabel::SectionLabel(section._name, section._type, parent, f){
// };

FramePanel::SectionLabel::SectionLabel(
    const DataFrame::DataSectionValue &section, QWidget* parent, Qt::WindowFlags f)
:QWidget(parent, f), _section(section){
    leLength = new QLineEdit(this);
    leName = new QLineEdit(this);
    labType = new QLabel(this);
    labValue = new QLabel(this);
    layout = new QHBoxLayout(this);
    layout->setSizeConstraint(QLayout::SetDefaultConstraint);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    layout->addWidget(leName);
    layout->addWidget(leLength);
    // leName->setLineWrapMode(QTextEdit::NoWrap);
    layout->addWidget(labType);
    layout->addWidget(labValue);
    layout->setStretchFactor(leName, 5);
    layout->setStretchFactor(leLength, 2);
    layout->setStretchFactor(labType, 5);
    layout->setStretchFactor(labValue, 8);
    
    // this->setLayout(layout);
}

FramePanel::SectionLabel::~SectionLabel(){};

void FramePanel::SectionLabel::updateData(const DataFrame::DataSectionValue &section){
    _section = section;
    // leLength->setText(QString::number(section._length));
    // leName->setText(section._name);
    // labValue->setText(section.toString());
    // labType->setText(QMetaType::typeName(section._type));


}

