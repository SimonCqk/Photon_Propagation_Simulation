#include "mainwindow.h"
#include "mcml/mcml.h"
#include <QApplication>
#include <QDialog>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow main_window;
  /*
  main_window.setWindowTitle(
      QObject::tr(" 生物光子传播模拟分析系统     Copyright@ Laboratory of Bioinstrumentation"));
  */
  main_window.setWindowTitle(
      QObject::tr(" Biological Photon Propagation Simulation Analysis System"));
  main_window.setFont(QFont("Consolas"));
  main_window.setWindowOpacity(0.85);
  main_window.show();

  return app.exec();
}
