#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    login(new Ui::LoginWindow)
{
    login->setupUi(this);

}

LoginWindow::~LoginWindow()
{
    delete login;
}

void LoginWindow::on_pushButton_clicked()
{
    main=new MainWindow(this);
    main->show();
}

void LoginWindow::on_pushButton_2_clicked()
{
    QApplication* app;
    app->exit(0);

}
