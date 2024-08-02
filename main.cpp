#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    // qSetMessagePattern("[ %{file}: %{line} ] %{message}");
    QApplication a(argc, argv);

    QFont f = a.font();
    f.setFamily("Segoe UI");
    a.setFont(f);

    // QFile qss("../res/style/StyleSheet.qss");
    // qss.open(QFile::ReadOnly);
    // a.setStyleSheet(qss.readAll());
    // qss.close();

    MainWindow w;
    w.resize(774, 440);
    w.show();
    return a.exec();
}
