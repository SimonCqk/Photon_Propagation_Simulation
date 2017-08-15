#include "mainwindow.h"
#include <QApplication>
#include"start.h"
#include"mcml/mcml.h"



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow main_window;
    main_window.setWindowTitle(QString("Model of Light Transport in Multi-layered Tissues"));
    main_window.show();
    /*
    start start_widget;
    start_widget.show();
    */
    return app.exec();
}
