#include "confparas.h"
#include "ui_confparas.h"
#include<QPainter>
#include<QStyleOption>
#include<QIntValidator>
#include<QDoubleValidator>
#include<QDialog>
#include<QVBoxLayout>
#include<QDebug>

ConfParas::ConfParas(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfParas)
{
    ui->setupUi(this);
    ui->PhoNumSpinBox->setMaximum(99999);
    ui->PhoNumSlider->setMaximum(99999);
    ui->PhoNumSlider->setStyleSheet("QSlider::groove:horizontal {  \
                                    border: 1px solid #bbb;  \
                                    background: white;  \
                                    height: 10px;  \
                                    }  \
                                      \
                                    QSlider::sub-page:horizontal {  \
                                    background: qlineargradient(x1: 0, y1: 0,    x2: 0, y2: 1, \
                                        stop: 0 #66e, stop: 1 #bbf);  \
                                    background: qlineargradient(x1: 0, y1: 0.2, x2: 1, y2: 1,  \
                                        stop: 0 #bbf, stop: 1 #55f);  \
                                    border: 1px solid #777;  \
                                    height: 10px;  \
                                    }  \
                                      \
                                    QSlider::add-page:horizontal {  \
                                    background: #fff;  \
                                    border: 1px solid #777;  \
                                    height: 10px;  \
                                    }  \
                                      \
                                    QSlider::handle:horizontal {  \
                                    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, \
                                        stop:0 #eee, stop:1 #ccc);  \
                                    border: 1px solid #777;  \
                                    width: 13px;  \
                                    margin-top: -2px;  \
                                    margin-bottom: -2px;  \
                                    }  \
                                      \
                                    QSlider::handle:horizontal:hover {  \
                                    background: qlineargradient(x1:0, y1:0, x2:1, y2:1,  \
                                        stop:0 #fff, stop:1 #ddd);  \
                                    border: 1px solid #444;  \
                                    }  \
                                      \
                                    QSlider::sub-page:horizontal:disabled {  \
                                    background: #bbb;  \
                                    border-color: #999;  \
                                    }  \
                                      \
                                    QSlider::add-page:horizontal:disabled {  \
                                    background: #eee;  \
                                    border-color: #999;  \
                                    }  \
                                      \
                                    QSlider::handle:horizontal:disabled {  \
                                    background: #eee;  \
                                    border: 1px solid #aaa;  \
                                    }");
    // set line edits `int` or 'double' input only
    QIntValidator *intvalid=new QIntValidator();
    QDoubleValidator *doublevalid=new QDoubleValidator();
    intvalid->setBottom(1);
    doublevalid->setBottom(0.0);
    ui->zGridEdit->setValidator(doublevalid);
    ui->rGridEdit->setValidator(doublevalid);
    ui->No_aGridEdit->setValidator(intvalid);
    ui->No_rGridEdit->setValidator(intvalid);
    ui->No_zGridEdit->setValidator(intvalid);

    ui->Instructor->setAutoFillBackground(true);
    ui->Instructor->setAlignment(Qt::AlignCenter);
    setInstructor();
}

ConfParas::~ConfParas()
{
    delete ui;
}

void ConfParas::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void ConfParas::on_SpeLayerButton_clicked()
{
    QDialog *SpecifyLayer=new QDialog();
    SpecifyLayer->setWindowTitle("Specify parameters for layers");
    SpecifyLayer->setFixedSize(QSize(170,120));
    QPlainTextEdit *LayerEdit=new QPlainTextEdit();
    //QDoubleValidator *doublevalid=new QDoubleValidator();
    //doublevalid->setBottom(0.0);
    QPushButton *Confrim=new QPushButton("Confirm");
    Confrim->setFont(QFont("Consolas"));
    QVBoxLayout *layout=new QVBoxLayout();
    layout->addWidget(LayerEdit);
    layout->addWidget(Confrim);  

    connect(Confrim,&QPushButton::clicked,[LayerEdit,this]{
        // use lambda instead of slots
        // lambda syntax is amazing.
        *LayerDatas=LayerEdit->toPlainText().split("\n",QString::SkipEmptyParts);
    });
    connect(Confrim,&QPushButton::clicked,this,&ConfParas::setInstructor);
    connect(Confrim,&QPushButton::clicked,SpecifyLayer,&QDialog::close);

    SpecifyLayer->setLayout(layout);
    SpecifyLayer->show();

}



void ConfParas::on_ClearButton_clicked()
{
    ui->PhoNumSpinBox->clear();
    ui->MedAboveEdit->clear();
    ui->MedBelowEdit->clear();
    ui->No_aGridEdit->clear();
    ui->No_rGridEdit->clear();
    ui->No_zGridEdit->clear();
    ui->rGridEdit->clear();
    ui->zGridEdit->clear();
    LayerDatas->clear();
}

bool ConfParas::checkLayerDatas()
{
    bool flag;
    for(auto &it:(*LayerDatas))
    {
        QStringList tmp=it.split(" ");
        if(tmp.size()!=4)
            return false;
        for(auto &i:tmp)
        {
            if(i.contains("."))
            {
                i=i.toDouble(&flag);
            }
            else
            {
                i=i.toInt(&flag,10);
            }
        }
    }
    return flag;
}

void ConfParas::setInstructor()
{
    QPalette pa;
    if(LayerDatas->isEmpty())
    {
        pa.setColor(QPalette::Background,Qt::red);
        ui->Instructor->setPalette(pa);
        ui->Instructor->setText("Layer paras is empty");
    }
    else
    {
        if(checkLayerDatas())
        {
            pa.setColor(QPalette::Background,Qt::green);
            ui->Instructor->setPalette(pa);
            ui->Instructor->setText("Valid paras");
        }
        else
        {
            pa.setColor(QPalette::Background,Qt::red);
            ui->Instructor->setPalette(pa);
            ui->Instructor->setText("Invalid paras");
        }
    }
}
