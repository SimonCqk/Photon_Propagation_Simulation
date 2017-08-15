#include "about.h"
#include "ui_about.h"
#include<QDesktopServices>
#include<QPainter>
#include<QStyleOption>


About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->textBrowser->setOpenLinks(false);  // Indispensable!!Or Qt will try to open hyperlink locally
    ui->textBrowser->setStyleSheet("font-size: 11px;\
                                    font-family: Consolas;\
                                    background-color: rgb(215,215,215);");  // seems font-set doesn't work
}

About::~About()
{
    delete ui;
}

void About::paintEvent(QPaintEvent *)
 {
     QStyleOption opt;
     opt.init(this);
     QPainter p(this);
     style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
 }


void About::on_textBrowser_anchorClicked(const QUrl &arg1)
{
    QDesktopServices::openUrl(arg1);
}
