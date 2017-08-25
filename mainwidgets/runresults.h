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
    static RunResults* getInstance()
    {
        if(!theRunResults)
            theRunResults=new RunResults();
        return theRunResults;
    }
    explicit RunResults(QWidget *parent = 0);
    RunResults(const RunResults&)=delete;
    RunResults& operator =(const RunResults&)=delete;

    ~RunResults();
    inline void getOutputData();
private:
    Ui::RunResults *ui;
    OutClass out_param;
    static RunResults* theRunResults;
};


#endif // RUNRESULTS_H
