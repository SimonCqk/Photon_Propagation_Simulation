#ifndef CONFPARAS_H
#define CONFPARAS_H

#include "mcml/mcml.h"
#include "mcml/mcml_fwd.h"
#include <QPlainTextEdit>
#include <QThread>
#include <QVector>
#include <QWidget>
#include <atomic>


namespace Ui {
class ConfParas;
}

class Workers : public QThread {
  Q_OBJECT
public:
  explicit Workers(OutClass &out_parm, InputClass &in_parm,
                   std::atomic<int> &flag, const int &len,
                   QObject *parent = nullptr)
      : out_(out_parm), in_(in_parm), flag_(flag), len_(len), QThread(parent) {}

protected:
  virtual void run() override;
signals:
  void flagChanged(const int &flag);

private:
  OutClass &out_;
  InputClass &in_;
  std::atomic<int> &flag_;
  int len_;
};

class ConfParas : public QWidget {
  Q_OBJECT

public:
  static ConfParas *getInstance() {
    if (!theConfParas)
      theConfParas = new ConfParas();
    return theConfParas;
  }

  explicit ConfParas(QWidget *parent = 0);
  ConfParas(const ConfParas &) = delete;
  ConfParas &operator=(const ConfParas &) = delete;

  ~ConfParas();
  void readDatas(InputClass &In_Ptr);
  void doOneRun(InputClass &In_Ptr);
  void setSampleOneDatas();
  void setSampleTwoDatas();
  void setSampleThreeDatas();

protected:
  void paintEvent(QPaintEvent *); // ready for style sheet setting
private slots:
  void on_SpeLayerButton_clicked();

  void on_ClearButton_clicked();

  void on_RunButton_clicked();
  void setProgressValue(const int &flag);
signals:
  void isDone(); // signal for Running-is-done.
private:
  Ui::ConfParas *ui;
  QStringList *LayerDatas = new QStringList();
  bool checkLayerDatas();
  void setInstructor();
  bool judgeParamsNotEmpty();
  static ConfParas *theConfParas;
};

#endif // CONFPARAS_H
