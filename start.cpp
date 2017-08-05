#include "start.h"
#include "ui_start.h"
#include"mcml/mcml_fwd.h"
#include<QImage>
//#include<QDebug>

start::start(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::start)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    QString bground_path=":/image/background";
    QPixmap bground;  // set background image
    bground.load(bground_path);
    QPalette palette;
    // set image scaled
    palette.setBrush(this->backgroundRole(),QBrush(bground.scaled(this->size(),
                                                                           Qt::IgnoreAspectRatio,
                                                                           Qt::SmoothTransformation)));
    this->setPalette(palette);
    this->setWindowTitle("Welcome Page");

    //set Run number limitation
    ui->spinBox_Run->setMaximum(10);
    ui->horizontalSlider->setMaximum(10);
}

start::~start()
{
    delete ui;
}

void start::on_QuitButton_clicked()
{
    this->close();
}

void start::on_ConfrimButton_clicked()
{
    size_t RUNS_NUM=ui->spinBox_Run->value();
    this->close();
}

