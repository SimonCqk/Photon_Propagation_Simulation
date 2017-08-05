#ifndef START_H
#define START_H

#include <QWidget>

namespace Ui {
class start;
}

class start : public QWidget
{
    Q_OBJECT

public:
    explicit start(QWidget *parent = 0);
    ~start();

private slots:
    void on_QuitButton_clicked();

    void on_ConfrimButton_clicked();


private:
    Ui::start *ui;
};

#endif // START_H
