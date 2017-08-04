#include "start.h"
#include "ui_start.h"
#include<QImage>
#include<QDebug>

start::start(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::start)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    QString bground_path=":/image/background";
    qDebug()<<bground_path;
    QPixmap bground;  // set background image
    bground.load(bground_path);
    QPalette palette;
    // set image scaled
    palette.setBrush(this->backgroundRole(),QBrush(bground.scaled(this->size(),
                                                                           Qt::IgnoreAspectRatio,
                                                                           Qt::SmoothTransformation)));
    this->setPalette(palette);
}

start::~start()
{
    delete ui;
}
