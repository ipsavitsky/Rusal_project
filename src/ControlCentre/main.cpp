#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/Rusal_Logo.svg.png"));
    MainWindow w;
    w.show();

    return a.exec();
}
