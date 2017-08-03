#include "start.h"
#include "ui_start.h"
#include<QImage>
//#include<QPalette>

start::start(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::start)
{
    ui->setupUi(this);
    QImage bground;
    bground.load("./image/photon.jpg");
    QPalette palette;
    palette.setBrush(this->backgroundRole(),QBrush(bground));
    this->setPalette(palette);
    this->setAutoFillBackground(true);
}

start::~start()
{
    delete ui;
}
