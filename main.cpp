#include "mainwindow.h"
#include <QApplication>
#include"mcml/mcml.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle(QString("Model of Light Transport in Multi-layered Tissues"));
    w.show();

    return a.exec();
}
