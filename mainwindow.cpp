#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //set color of pushbutton
    ui->ConfigButton->setStyleSheet("background-color: rgb(150, 150, 150);");
    ui->ResultButton->setStyleSheet("background-color: rgb(150, 150, 150);");
    ui->HistoryButton->setStyleSheet("background-color: rgb(150, 150, 150);");
    ui->AboutButton->setStyleSheet("background-color: rgb(150, 150, 150);");

    ui->statusBar->showMessage(QString("May it helps you. :)"));
    QPalette pa;
    pa.setColor(QPalette::ColorGroup,Qt::gray);
    ui->statusBar->setPalette(pa);
    //let main windows can not scale
    this->setFixedSize(625,380);

}

MainWindow::~MainWindow()
{
    delete ui;
}
