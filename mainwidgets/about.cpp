#include "about.h"
#include "ui_about.h"
#include<QDesktopServices>

About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    ui->textBrowser->setOpenLinks(false);
}

About::~About()
{
    delete ui;
}

void About::on_textBrowser_anchorClicked(const QUrl &arg1)
{
    QDesktopServices::openUrl(arg1);
}