#include "runresults.h"
#include "ui_runresults.h"
#include<QDebug>
#include<QDialog>
#include<QVBoxLayout>
#include<QPlainTextEdit>

RunResults* RunResults::theRunResults=nullptr;

OutClass out_temp;

QString LinkDataFromVector(const QVector<double> &vec)
{
    QString str="";

    for(const auto& v:vec)
    {
        str+= (QString::number(v,'f',4)+" ");
    }
    str+="\n";
    return str;
}

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
    // boring setting for white background fonts shown in UI.
    this->setPalette(palette);
    QPalette font_color;
    font_color.setColor(QPalette::WindowText,Qt::white);
    ui->label->setPalette(font_color);
    ui->label_2->setPalette(font_color);
    ui->label_3->setPalette(font_color);
    ui->label_4->setPalette(font_color);
    ui->label_5->setPalette(font_color);
    ui->label_6->setPalette(font_color);
    ui->label_7->setPalette(font_color);
    ui->label_8->setPalette(font_color);
    ui->label_9->setPalette(font_color);
    ui->label_10->setPalette(font_color);
    ui->label_11->setPalette(font_color);
    ui->label_12->setPalette(font_color);
    ui->label_13->setPalette(font_color);
    ui->label_14->setPalette(font_color);

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

void RunResults::on_View_diff_reflect_2d_Button_clicked()
{
    QDialog *viewer=new QDialog();
    //viewer->setWindowTitle("Viewer for 2D distribution of diffuse reflectance. [1/(cm2 sr)] ");
    viewer->setFixedSize(QSize(300,200));
    viewer->setFont(QFont("Consolas"));
    viewer->setWindowModality(Qt::WindowModal);
    viewer->setWindowIcon(QIcon(":/image/logo"));

    QPlainTextEdit *view_data=new QPlainTextEdit();
    view_data->setFont(QFont("Consolas"));
    view_data->setReadOnly(true);
    QString plain_text;
    plain_text="# Viewer for 2D distribution of diffuse reflectance. [1/(cm2 sr)] #\n   \n";
    for(const auto& item:out_param.out->diff_reflect_2d)
    {
        plain_text+=LinkDataFromVector(item);
    }
    view_data->setPlainText(plain_text);

    QPushButton *Close=new QPushButton("Close");
    Close->setFont(QFont("Consolas"));

    QVBoxLayout *layout=new QVBoxLayout();
    layout->addWidget(view_data);
    layout->addWidget(Close);

    viewer->setLayout(layout);
    viewer->show();

    connect(Close,&QPushButton::clicked,viewer,&QDialog::close);
}



void RunResults::on_View_Abs_prob_layer_Button_clicked()
{
    QDialog *viewer=new QDialog();
    //viewer->setWindowTitle("Viewer for 2D distribution of diffuse reflectance. [1/(cm2 sr)] ");
    viewer->setFixedSize(QSize(300,200));
    viewer->setFont(QFont("Consolas"));
    viewer->setWindowModality(Qt::WindowModal);
    viewer->setWindowIcon(QIcon(":/image/logo"));

    QPlainTextEdit *view_data=new QPlainTextEdit();
    view_data->setFont(QFont("Consolas"));
    view_data->setReadOnly(true);
    QString plain_text;
    plain_text="# Viewer for absorption probability of each layer #\n   \n";
    plain_text+=LinkDataFromVector(out_param.out->abs_prob_layer);
    view_data->setPlainText(plain_text);

    QPushButton *Close=new QPushButton("Close");
    Close->setFont(QFont("Consolas"));

    QVBoxLayout *layout=new QVBoxLayout();
    layout->addWidget(view_data);
    layout->addWidget(Close);

    viewer->setLayout(layout);
    viewer->show();

    connect(Close,&QPushButton::clicked,viewer,&QDialog::close);
}
