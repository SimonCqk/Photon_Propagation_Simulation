#include "mainwindow.h"
#include "mcml/mcml.h"
#include "start.h"
#include <QApplication>
#include <QDialog>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MainWindow main_window;
  main_window.setWindowTitle(
      QString("Model of Light Transport in Multi-layered Tissues"));
  main_window.setWindowOpacity(0.85);
  //main_window.show();

  start start_widget;
  start_widget.show();
  QCoreApplication::connect(&start_widget,&start::enter,[&start_widget,&main_window]{
      start_widget._close();
      main_window._show();
  });

  return app.exec();
}
