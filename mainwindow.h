#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void addTabWidgets();
    void setConfParas(QWidget *wid);
    void setRunResults(QWidget *wid);
    void setRunHistory(QWidget *wid);
    void setAbout(QWidget *wid);


};

#endif // MAINWINDOW_H
