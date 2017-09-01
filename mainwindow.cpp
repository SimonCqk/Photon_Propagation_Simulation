#include "mainwindow.h"
#include "mainwidgets/about.h"
#include "mainwidgets/confparas.h"
#include "mainwidgets/runresults.h"
#include"mainwidgets/history.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QProxyStyle>
#include<QTextStream>

class CustomTabStyle : public QProxyStyle {
public:
  QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                         const QSize &size, const QWidget *widget) const {
    QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
    if (type == QStyle::CT_TabBarTab) {
      s.transpose();
      s.rwidth() = 135;
      s.rheight() = 85;
    }
    return s;
  }

  void drawControl(ControlElement element, const QStyleOption *option,
                   QPainter *painter, const QWidget *widget) const {
    if (element == CE_TabBarTabLabel) {
      if (const QStyleOptionTab *tab =
              qstyleoption_cast<const QStyleOptionTab *>(option)) {
        QRect allRect = tab->rect;

        if (tab->state & QStyle::State_Selected) {
          painter->save();
          painter->setPen(0x0077dd);
          painter->setBrush(QBrush(0x0077dd));
          painter->drawRect(allRect.adjusted(6, 6, -6, -6));
          painter->restore();
        }
        QTextOption option;
        option.setAlignment(Qt::AlignCenter);
        if (tab->state & QStyle::State_Selected) {
          painter->setPen(0xf8fcff);
        } else {
          painter->setPen(0x5d5d5d);
        }

        painter->drawText(allRect, tab->text, option);
        return;
      }
    }

    if (element == CE_TabBarTab) {
      QProxyStyle::drawControl(element, option, painter, widget);
    }
  }
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  openAndRead();

  ui->statusBar->showMessage(QString("May it helps you. :)     # Number of Running: %1").arg(QString::number(no_run,10)));
  ui->statusBar->setStyleSheet("background-color: rgb(190,190,190);\
                                font-family: Consolas;");
  ui->TabWidget->setStyleSheet("QTabWidget{border:none;\
                                           margin:0px;\
                                           outline:none;}");

  // let main windows can not scale
  this->setFixedSize(625, 380);
  // delete default tabs in Tab Widget
  delete ui->tab;
  delete ui->tab_2;
  // ui->TabWidget->setTabPosition(QTabWidget::West);
  ui->TabWidget->tabBar()->setStyle(new CustomTabStyle);
  ui->TabWidget->tabBar()->setStyleSheet(
      "background-color: rgb(130, 130, 130);\
                                            font-size: 10pt; font-family: Consolas;");
  // set up tab widgets
  // First page: configure parameters
  ConfParas *confparas = ConfParas::getInstance();
  ui->TabWidget->addTab(confparas, "Configure Parameters");
  // Second page: show run results
  RunResults *runresults = RunResults::getInstance();
  ui->TabWidget->addTab(runresults, "Run Results");
  // Third page: show running history
  History *RunHistory =History::getInstance();
  ui->TabWidget->addTab(RunHistory, "History");
  // Fourth page: show about information
  About *about = About::getInstance();
  ui->TabWidget->addTab(about, "About");

  connect(confparas, &ConfParas::isDone, [runresults, this] {
    ui->TabWidget->setCurrentWidget(runresults);
    runresults->getOutputData();
    runresults->showAllTheResults();
    ++no_run;
    InsertHistory(no_run);
  });
}

MainWindow::~MainWindow() {
    delete ui;
    writeAndClose();
}

void MainWindow::on_actionSample_One_triggered() {
  ConfParas *conf = ConfParas::getInstance();
  conf->setSampleOneDatas();
}

void MainWindow::on_actionSample_Two_triggered() {
  ConfParas *conf = ConfParas::getInstance();
  conf->setSampleTwoDatas();
}

void MainWindow::openAndRead()
{
    run_t.setFileName("RunTimes.txt");
    if(run_t.open(QIODevice::ReadWrite|QIODevice::Truncate)){
        QTextStream read(&run_t);
        QString temp;
        read>>temp;
        if(temp.isEmpty()){
            no_run=1;
            return;
        }
        no_run=temp.toInt(nullptr,10);
    }
    else
        QMessageBox::critical(0, QObject::tr("Open File Error"),
                              "Can not open Run-Time File.");
}

void MainWindow::writeAndClose()
{
    if(!run_t.isOpen())
        run_t.open(QIODevice::ReadWrite|QIODevice::Truncate);
    QTextStream write(&run_t);
    write<<no_run;
    run_t.close();
}
