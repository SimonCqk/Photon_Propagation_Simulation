#include "history.h"
#include "ui_history.h"

History *History::theHistory = nullptr;

History::History(QWidget *parent) : QWidget(parent), ui(new Ui::History) {
  ui->setupUi(this);
}

History::~History() { delete ui; }
