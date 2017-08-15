#ifndef CONFPARAS_H
#define CONFPARAS_H

#include <QWidget>

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
private:
    Ui::ConfParas *ui;
};

#endif // CONFPARAS_H
