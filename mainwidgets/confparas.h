#ifndef CONFPARAS_H
#define CONFPARAS_H

#include <QWidget>
#include<QPlainTextEdit>


namespace Ui {
class ConfParas;
}

class ConfParas : public QWidget
{
    Q_OBJECT

public:
    explicit ConfParas(QWidget *parent = 0);
    ~ConfParas();
protected:
    void paintEvent(QPaintEvent *);  // ready for style sheet setting
private slots:
    void on_SpeLayerButton_clicked();
    void saveLayerDatas(QPlainTextEdit *LayerEdit);
private:
    Ui::ConfParas *ui;
    QString LayerDatas;
};

#endif // CONFPARAS_H
