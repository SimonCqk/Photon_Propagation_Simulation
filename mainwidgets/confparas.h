#ifndef CONFPARAS_H
#define CONFPARAS_H

#include "../mcml/mcml_fwd.h"
#include <QPlainTextEdit>
#include <QVector>
#include <QWidget>
#include<QLineEdit>

namespace Ui {
class ConfParas;
}

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

  void setSampleOneDatas();
  void setSampleTwoDatas();
  void setSampleThreeDatas();

protected:
  void paintEvent(QPaintEvent *); // ready for style sheet setting
private slots:
  void on_SpeLayerButton_clicked();

  void on_ClearButton_clicked();

  void on_RunButton_clicked();

signals:
  void isDone(); // signal for Running-is-done.

private:
  Ui::ConfParas *ui;
  QVector<QVector<QLineEdit*>> layers_params;
  bool judgeParamsNotEmpty();
  bool readDatas(InputClass &In_Ptr);
  void doOneRun(InputClass &In_Ptr);
  void setSampleTemplate(const int& num_layer,const std::vector<std::vector<double>>& data);
  static ConfParas *theConfParas;
};

#endif // CONFPARAS_H
