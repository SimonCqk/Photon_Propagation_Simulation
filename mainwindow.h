#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainwidgets/confparas.h"
#include <QMainWindow>
#include<fstream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
  friend class ConfParas;

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

private slots:

  void on_actionSample_One_triggered();

  void on_actionSample_Two_triggered();

private:
  Ui::MainWindow *ui;
  QFile run_t;
  size_t no_run;
  void openAndRead();
  void writeAndClose();
};

#endif // MAINWINDOW_H
