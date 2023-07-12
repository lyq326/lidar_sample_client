#include "framepanel/ByteButton.h"

ByteButton::ByteButton(QWidget *parent) :QPushButton(parent){
    this->setCheckable(true);
    this->setContentsMargins(0, 0, 0, 0);
    this->setFont(QFont("Consolas", 12));
    this->setText("  ");
    menu = new QMenu();
    menuSection = new QMenu("Add To Section");
    actionsAddSection.append(new QAction("Add To Section"));
    this->menu->addMenu(menuSection);

    for(auto& s :(const QMetaType::Type[]){QMetaType::Short, QMetaType::UShort,QMetaType::Int, QMetaType::UInt, QMetaType::Float}){
        QAction* a = new QAction(QMetaType::typeName(s));
        actionsAddSection.append(a);
        this->menuSection->addAction(a);
        connect(a, &QAction::triggered, this, [this, s]() mutable{
            emit(actTriggered(this, s));
        });
    }



    // this->menuSection->addActions()
}

// void ByteButton::actTriggered(const ByteButton* index, const QMetaType::Type t){}

ByteButton::ByteButton(const QString &text, QWidget *parent)
    :QPushButton(text, parent){

}
ByteButton::~ByteButton(){

}

void ByteButton::contextMenuEvent(QContextMenuEvent *event){
    this->menu->exec(QCursor::pos());
}