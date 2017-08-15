#include "mainwindow.h"
#include "ui_mainwindow.h"
#include"mainwidgets/about.h"

#include <QPainter>
#include <QProxyStyle>

class CustomTabStyle : public QProxyStyle
{
public:
    QSize sizeFromContents(ContentsType type, const QStyleOption *option,
        const QSize &size, const QWidget *widget) const
    {
        QSize s = QProxyStyle::sizeFromContents(type, option, size, widget);
        if (type == QStyle::CT_TabBarTab) {
            s.transpose();
            s.rwidth() = 135;
            s.rheight() = 85;
        }
        return s;
    }

    void drawControl(ControlElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == CE_TabBarTabLabel) {
            if (const QStyleOptionTab *tab = qstyleoption_cast<const QStyleOptionTab *>(option)) {
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
                }
                else {
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusBar->showMessage(QString("May it helps you. :)"));
    //let main windows can not scale
    this->setFixedSize(625,380);
    //delete default tabs in Tab Widget
    delete ui->tab;
    delete ui->tab_2;
    //ui->TabWidget->setTabPosition(QTabWidget::West);
    ui->TabWidget->tabBar()->setStyle(new CustomTabStyle);
    ui->TabWidget->tabBar()->setStyleSheet("background-color: rgb(130, 130, 130);\
                                            font-size: 10pt; font-family: Consolas;");
    //set up tab widgets
    setConfParas();
    setRunResults();
    setRunHistory();
    setAbout();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setConfParas()
{
    //First page: configure parameters
    QWidget *ConfParas=new QWidget();
    ui->TabWidget->addTab(ConfParas,"Configure Parameters");
}

void MainWindow::setRunResults()
{
    //Second page: show run results
    QWidget *RunResults=new QWidget();
    ui->TabWidget->addTab(RunResults,"Run Results");

}

void MainWindow::setRunHistory()
{
    //Third page: show running history
    QWidget *RunHistory=new QWidget();
    ui->TabWidget->addTab(RunHistory,"History");

}

void MainWindow::setAbout()
{

    //Fourth page: show about information
    About *about=new About();

    ui->TabWidget->addTab(about,"About");


}


