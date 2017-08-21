#ifndef CONFPARAS_H
#define CONFPARAS_H

#include <QWidget>
#include<QPlainTextEdit>
#include"../mcml/mcml.h"
#include<QVector>

namespace Ui {
class ConfParas;
}

class ConfParas : public QWidget
{
    Q_OBJECT

public:
    explicit ConfParas(QWidget *parent = 0);
    ~ConfParas();
    void readDatas(InputClass& In_Ptr);
protected:
    void paintEvent(QPaintEvent *);  // ready for style sheet setting
private slots:
    void on_SpeLayerButton_clicked();

    void on_ClearButton_clicked();

private:
    Ui::ConfParas *ui;
    QStringList* LayerDatas=new QStringList();
    bool checkLayerDatas();
    void setInstructor();
};

#endif // CONFPARAS_H
