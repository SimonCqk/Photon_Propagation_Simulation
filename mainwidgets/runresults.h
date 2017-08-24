#ifndef RUNRESULTS_H
#define RUNRESULTS_H

#include <QWidget>
#include"mcml/mcml.h"

extern OutClass out_temp;

namespace Ui {
class RunResults;
}

class RunResults : public QWidget
{
    Q_OBJECT
public:
    explicit RunResults(QWidget *parent = 0);
    ~RunResults();
    inline void getOutputData();
private:
    Ui::RunResults *ui;
    OutClass out_param;
};


#endif // RUNRESULTS_H
