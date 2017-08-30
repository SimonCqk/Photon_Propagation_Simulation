#ifndef CONFPARAS_H
#define CONFPARAS_H

#include <QWidget>
#include<QPlainTextEdit>
#include"../mcml/mcml_fwd.h"
#include<QVector>

namespace Ui {
class ConfParas;
}

class ConfParas : public QWidget
{
    Q_OBJECT

public:
    static ConfParas* getInstance()
    {
        if(!theConfParas)
            theConfParas=new ConfParas();
        return theConfParas;
    }

    explicit ConfParas(QWidget *parent = 0);
    ConfParas(const ConfParas&)=delete;
    ConfParas& operator=(const ConfParas&)=delete;

    ~ConfParas();
    void readDatas(InputClass &In_Ptr);
    void doOneRun(InputClass &In_Ptr);
    void setSampleOneDatas();
    void setSampleTwoDatas();
protected:
    void paintEvent(QPaintEvent *);  // ready for style sheet setting
private slots:
    void on_SpeLayerButton_clicked();

    void on_ClearButton_clicked();

    void on_RunButton_clicked();

signals:
    void isDone();  // signal for Running-is-done.

private:
    Ui::ConfParas *ui;
    QStringList* LayerDatas=new QStringList();
    bool checkLayerDatas();
    void setInstructor();
    bool judgeParamsNotEmpty();
    static ConfParas* theConfParas;
};

#endif // CONFPARAS_H
