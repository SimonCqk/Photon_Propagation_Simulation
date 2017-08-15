#ifndef ABOUT_H
#define ABOUT_H

#include <QWidget>

namespace Ui {
class About;
}

class About : public QWidget
{
    Q_OBJECT

public:
    explicit About(QWidget *parent = 0);
    ~About();

private slots:
    void on_textBrowser_anchorClicked(const QUrl &arg1);

private:
    Ui::About *ui;
};

#endif // ABOUT_H
