#include "history.h"
#include "ui_history.h"
#include<QtSql/QSqlDatabase>
#include<QDebug>
#include<QMessageBox>

History *History::theHistory = nullptr;

QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");

bool DBconnect(const QString& db_name){
    db.setDatabaseName(db_name);
    db.setUserName("ROOT");
    db.setPassword("123456");
    if(!db.open())
        return false;
    return true;
}

void CreateTables(){

}

History::History(QWidget *parent) : QWidget(parent), ui(new Ui::History) {
  ui->setupUi(this);
  if(!DBconnect("database.db"))
      QMessageBox::critical(nullptr, QObject::tr("Collection"), QObject::tr("Failed to Connect to Database!"));
}

History::~History()
{
    delete ui;
}
