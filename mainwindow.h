#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "mainwidgets/confparas.h"
#include <QMainWindow>
#include <fstream>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT
  friend class ConfParas;

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
public slots:
  void _show();
protected:
  void paintEvent(QPaintEvent *);
private slots:
  void on_actionSample_One_triggered();

  void on_actionSample_Two_triggered();

  void on_actionSample_Three_triggered();

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
