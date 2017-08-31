#ifndef HISTORY_H
#define HISTORY_H

#include <QWidget>
#include"mainwidgets/runresults.h"
#include<QMessageBox>

namespace Ui {
class History;
}

class History : public QWidget {
  Q_OBJECT

public:
  static History *getInstance()
  {
    if (!theHistory)
      theHistory = new History();
    return theHistory;
  }

  explicit History(QWidget *parent = 0);
  History(const History&)=delete;
  History& operator=(const History&)=delete;
  ~History();

private:
  Ui::History *ui;
  static History *theHistory;
};

class OutputToString final{
public:
    explicit OutputToString(const OutClass& output);
    OutputToString(const OutputToString&)=default;
    OutputToString& operator=(const OutputToString&)=default;
    inline QString getAll(){
        return all;
    }

private:
    QString spec_reflect;
    QString diff_reflect_2d;
    QString diff_reflect_rdl;
    QString diff_reflect_agl;
    QString diff_reflect;
    QString abs_prob_rz;
    QString abs_prob_z;
    QString abs_prob_layer;
    QString abs_prob;
    QString total_trans_2d;
    QString total_trans_agl;
    QString total_trans_rdl;
    QString total_trans;

    QString all;
    void setUpAll();
};

class InputToString final{
public:
    explicit InputToString(const InputClass& input);
    InputToString(const InputToString&)=default;
    InputToString& operator=(const InputToString&)=default;
    inline QString getAll(){
        return all;
    }

private:
    QString num_photon;
    QString dz;
    QString dr;
    QString da;
    QString nz;
    QString nr;
    QString na;
    QString num_layers;
    QString layerspecs;

    QString all;
    void setUpAll();
};


#endif // HISTORY_H
