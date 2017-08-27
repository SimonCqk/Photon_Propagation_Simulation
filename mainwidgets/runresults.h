#ifndef RUNRESULTS_H
#define RUNRESULTS_H

#include <QWidget>
#include<sstream>
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
    void getOutputData();
    void showAllTheResults();

private slots:
    void on_View_diff_reflect_2d_Button_clicked();

    void on_View_Abs_prob_layer_Button_clicked();

private:
    Ui::RunResults *ui;
    OutClass out_param;
    static RunResults* theRunResults;
    void setRAT();
};

template<typename T>
QString Convert2String(const T& t)
{
    std::stringstream temp;
    temp<<t;
    return QString::fromStdString(temp.str());
}

QString LinkDataFromVector(const QVector<double>& vec);

#endif // RUNRESULTS_H
