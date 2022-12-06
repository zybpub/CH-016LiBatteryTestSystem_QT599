#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <mainwindow.h>

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

      //void paintevent(QPaintEvent *event);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::LoginWindow *login;
      MainWindow *main;

};

#endif // LOGINWINDOW_H
