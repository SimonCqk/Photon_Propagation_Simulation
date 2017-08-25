#include "runresults.h"
#include "ui_runresults.h"

RunResults* RunResults::theRunResults=nullptr;

RunResults::RunResults(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RunResults)
{
    ui->setupUi(this);
}

RunResults::~RunResults()
{
    delete ui;
}

void RunResults::getOutputData()
{
    this->out_param=out_temp;
}
