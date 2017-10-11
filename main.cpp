#include "mainwindow.h"
#include "mcml/mcml.h"
#include <QApplication>
#include <QDialog>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  MainWindow main_window;

  /*
  main_window.setWindowTitle(
      QString("Model of Light Transport in Multi-layered Tissues"));
  */
  main_window.setWindowTitle(
      QObject::tr(" 生物光子传播模拟分析系统 "));
  main_window.setFont(QFont("Consolas"));
  main_window.setWindowOpacity(0.85);
  main_window.show();

  return app.exec();
}
