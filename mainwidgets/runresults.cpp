#include "runresults.h"
#include "ui_runresults.h"
#include<QDebug>

RunResults* RunResults::theRunResults=nullptr;

OutClass out_temp;

RunResults::RunResults(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RunResults)
{
    ui->setupUi(this);

    this->setAutoFillBackground(true);
    QString bground_path=":/image/image/Photon.jpg";
    QPixmap bground;  // set background image
    bground.load(bground_path);
    QPalette palette;
    // set image scaled
    palette.setBrush(this->backgroundRole(),QBrush(bground.scaled(this->size(),
                                                                           Qt::IgnoreAspectRatio,
                                                                           Qt::SmoothTransformation)));
    this->setPalette(palette);
    QPalette font_color;
    font_color.setColor(QPalette::WindowText,Qt::white);
    ui->label->setPalette(font_color);
    ui->label_2->setPalette(font_color);
    ui->label_3->setPalette(font_color);
    ui->label_4->setPalette(font_color);
    ui->label_5->setPalette(font_color);
    ui->SpecRefEdit->setReadOnly(true);
    ui->AbsFracEdit->setReadOnly(true);
    ui->DiffRefEdit->setReadOnly(true);
    ui->TransEdit->setReadOnly(true);
}

RunResults::~RunResults()
{
    delete ui;
}

void RunResults::getOutputData()
{
    this->out_param=out_temp;
}

void RunResults::showAllTheResults()
{
    setRAT();
}

void RunResults::setRAT()
{
    ui->SpecRefEdit->setText(Convert2String<double>(out_param.out->spec_reflect));
    ui->DiffRefEdit->setText(Convert2String<double>(out_param.out->diff_reflect));
    ui->AbsFracEdit->setText(Convert2String<double>(out_param.out->abs_prob));
    ui->TransEdit->setText(Convert2String<double>(out_param.out->total_trans));
}
