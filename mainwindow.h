#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"mainwidgets/confparas.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    friend class ConfParas;
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

private:
    Ui::MainWindow *ui;
    void setConfParas();
    void setRunResults();
    void setRunHistory();
    void setAbout();


};

#endif // MAINWINDOW_H
