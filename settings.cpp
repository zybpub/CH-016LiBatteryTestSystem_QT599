#include "settings.h"
#include "ui_settings.h"
#include <QMessageBox>
#include <QCloseEvent>

Settings::Settings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_pushButton_save_clicked()
{
 QMessageBox::information(this,"提示","保存成功");
}

void Settings::on_pushButton_cancel_clicked()
{
  this->close();
}
void Settings::closeEvent(QCloseEvent *e){
    //窗口关闭时弹出的提示窗口
//    QMessageBox msgBox;
//    msgBox.setText("提示");
//    msgBox.setInformativeText("确认退出?");
//    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
//    msgBox.setDefaultButton(QMessageBox::Ok);
//    int ret = msgBox.exec();
//    if(ret == QMessageBox::Ok){
//        //若用户点击确认，则接收这个事件,当前窗口会关闭
//        e->accept();
//    }else{
//        //若用户点击取消，则忽略这个事件，当前窗口不会关闭
//        e->ignore();
//    }
}

