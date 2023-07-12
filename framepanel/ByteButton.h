#ifndef BYTE_BUTTON_H
#define BYTE_BUTTON_H

#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QCursor>
#include <QMetaType>

class ByteButton : public QPushButton{
    Q_OBJECT

public:
    explicit ByteButton(QWidget *parent = nullptr);
    explicit ByteButton(const QString &text, QWidget *parent = nullptr);
    ~ByteButton();
    void contextMenuEvent(QContextMenuEvent *event);

signals:
    void actTriggered(const ByteButton* index, const QMetaType::Type t);



private:
    QMenu* menu;
    QMenu* menuSection;
    // QMenu menuSection;
    QList<QAction*> actionsAddSection;
    // QAction *addSectionAction = menu.addAction("Add To Section");
};



#endif