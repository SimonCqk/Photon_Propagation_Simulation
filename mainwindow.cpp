#include "mainwindow.h"
#include "mainwidgets/about.h"
#include "mainwidgets/confparas.h"
#include "mainwidgets/history.h"
#include "mainwidgets/runresults.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QPainter>
#include <QProxyStyle>


class CustomTabStyle : public QProxyStyle {
public:
  QSize sizeFromContents(ContentsType type, const QStyleOption *option,
                         const QSize &size, const QWidget *widget) const {
    QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
    if (type == QStyle::CT_TabBarTab) {
      s.transpose();
      s.rwidth() = 120;
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
          painter->setPen(0x3B3B3B);
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
  ui->statusBar->setStyleSheet("background-color: rgb(190,190,190);\
                                font-family: Consolas;");
  ui->TabWidget->setStyleSheet("QTabWidget{border:none;\
                                           margin:0px;\
                                           outline:none;}");

  // let main windows can not scale
  this->setFixedSize(710, 390);
  // delete default tabs in Tab Widget
  delete ui->tab;
  delete ui->tab_2;
  // ui->TabWidget->setTabPosition(QTabWidget::West);
  ui->TabWidget->tabBar()->setStyle(new CustomTabStyle);
  ui->TabWidget->setFixedSize(this->size());
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
  // Third page: query for running history
  History *runhistory = History::getInstance();
  ui->TabWidget->addTab(runhistory, "History");
  // Fourth page: show about information
  About *about = About::getInstance();
  ui->TabWidget->addTab(about, "About");

  ui->statusBar->showMessage(
      QString("May it helps you. :)     # Number of Running: %1")
          .arg(QString::number(runhistory->getNumberOfRunTimes(), 10)));

  connect(confparas, &ConfParas::isDone, [runresults, this, runhistory] {
    ui->TabWidget->setCurrentWidget(runresults);
    runresults->getOutputData();
    runresults->showAllTheResults();
    int num = runhistory->getNumberOfRunTimes();
    InsertHistory(num);
    runhistory->addNumberOfRunTimes();
  });
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::_show() { show(); }

void MainWindow::on_actionSample_One_triggered() {
  ConfParas *conf = ConfParas::getInstance();
  conf->setSampleOneDatas();
}

void MainWindow::on_actionSample_Two_triggered() {
  ConfParas *conf = ConfParas::getInstance();
  conf->setSampleTwoDatas();
}
