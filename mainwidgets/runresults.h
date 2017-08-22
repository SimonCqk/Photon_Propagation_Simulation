#ifndef RUNRESULTS_H
#define RUNRESULTS_H

#include <QWidget>

namespace Ui {
class RunResults;
}

class RunResults : public QWidget
{
    Q_OBJECT

public:
    explicit RunResults(QWidget *parent = 0);
    ~RunResults();

private:
    Ui::RunResults *ui;
};

#endif // RUNRESULTS_H
