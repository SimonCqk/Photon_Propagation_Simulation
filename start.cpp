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
    // set color of labels text
    QPalette pa;
    pa.setColor(QPalette::WindowText,Qt::yellow);
    ui->label->setPalette(pa);
    ui->label_2->setPalette(pa);
    ui->label_3->setPalette(pa);
}

start::~start()
{
    delete ui;
}

void start::on_QuitButton_clicked()
{
    this->close();
    QCoreApplication::quit();
}

