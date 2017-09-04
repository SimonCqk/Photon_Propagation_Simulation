#ifndef START_H
#define START_H

#include <QWidget>

namespace Ui {
class start;
}

class start : public QWidget {
  Q_OBJECT

public:
  explicit start(QWidget *parent = 0);
  ~start();
signals:
  void enter();
public slots:
  void _close();
private slots:
  void on_QuitButton_clicked();
  void on_EnterButton_clicked();

private:
  Ui::start *ui;
};

#endif // START_H
