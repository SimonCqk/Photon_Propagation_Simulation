#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //set color of pushbutton
    ui->ConfigButton->setStyleSheet("background-color: rgb(103, 103, 103);");
    ui->ResultButton->setStyleSheet("background-color: rgb(103, 103, 103);");
    ui->HistoryButton->setStyleSheet("background-color: rgb(103, 103, 103);");
    ui->AboutButton->setStyleSheet("background-color: rgb(103, 103, 103);");

}

MainWindow::~MainWindow()
{
    delete ui;
}
