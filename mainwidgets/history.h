#ifndef HISTORY_H
#define HISTORY_H

#include <QWidget>

namespace Ui {
class History;
}

class History : public QWidget {
  Q_OBJECT

public:
  static History *getInstance() {
    if (!theHistory)
      theHistory = new History();
    return theHistory;
  }

  explicit History(QWidget *parent = 0);
  ~History();

private:
  Ui::History *ui;
  static History *theHistory;
};

#endif // HISTORY_H
