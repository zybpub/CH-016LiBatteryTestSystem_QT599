#include "mainwindow.h"
#include "config.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->menu_modify,SIGNAL(triggered()),this,SLOT(on_open_config_wind_clicked()));
     connect(ui->menu_close,SIGNAL(triggered()),this,SLOT(on_menu_close_clicked()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void  MainWindow::debug(QString msg){
     QMessageBox::information(this,"提示",msg);
}

void MainWindow::on_start_clicked()
{
    //创建QTcpSocket对象，并初始化
    mSocket = new QTcpSocket;
    //发起连接connectToHost
    mSocket->connectToHost(ui->ipEdit->text(),ui->portEdit->text().toInt());
    //关联连接成功信号
    connect(mSocket,SIGNAL(connected()),this,SLOT(connect_suc()));
    //关联读数据信号
     connect(mSocket,SIGNAL(readyRead()),this,SLOT(read_data()));
     ui->start->setEnabled(false);
}
//连接成功
void MainWindow::connect_suc()
{
    QMessageBox::information(this,"提示","连接成功");
     ui->stop->setEnabled(true);
}

//菜单打开配置窗体
void MainWindow::on_open_config_wind_clicked()
{
    //conf = new config(); //子窗口类
    //    conf->setWindowModality(Qt::ApplicationModal);

    QMessageBox::information(this,"提示","连接成功");
     ui->stop->setEnabled(true);
}
void MainWindow::on_menu_close_clicked()
{
    QMessageBox::information(this,"提示","连接成功");
     ui->stop->setEnabled(true);
}

//读数据
void MainWindow::read_data()
{
      //获取客户端IP
    QTcpSocket *obj = (QTcpSocket *)sender();
    QString msg = obj->readAll();
    QString peerIP = obj->peerAddress().toString().remove("::ffff:");
    ui->recvmsg->addItem(peerIP + ":" + msg);
    //自动下滚
    ui->recvmsg->setCurrentRow(ui->recvmsg->count() - 1);
    //接收字节数
    recvsize += msg.size();
   // ui->recv_size->setText(QString::number(recvsize));
}



void MainWindow::on_stop_clicked()
{
    mSocket->disconnectFromHost();
    if (
            mSocket->state() == QAbstractSocket::UnconnectedState \
            ||(  mSocket->waitForDisconnected(1000))
       )
    {
         QMessageBox::information(this,"提示","断开成功");
        ui->start->setEnabled(true);
        ui->stop->setEnabled(false);
    }
}
void MainWindow::on_pushButton1260_clicked()
{
   // QString msg = ui->sendmsg->toPlainText();
    QString msg = "volt 12.60\n";
    //mSocket->write(msg.toUtf8());
   mSocket->write( msg.toLatin1());
}

void MainWindow::on_pushButton1500_clicked()
{
    QString msg = "volt 14.90\r\n";
    mSocket->write(msg.toLatin1());
}

void MainWindow::on_pushButton_displayError_clicked()
{
    QString msg = "syst:err?\n";
    //mSocket->write(msg.toUtf8());
   mSocket->write( msg.toLatin1());

}

void MainWindow::on_pushButton_DeviceInfo_clicked()
{
    QString msg = "*idn?\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_DeviceInfo_2_clicked()
{
    QString msg = "SYST:REM\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_500_clicked()
{
    QString msg = "volt 5.00\n";
    mSocket->write(msg.toLatin1());
}

void MainWindow::on_btn_setV_clicked()
{
    QString msg ="volt "+ ui->lineEdit_Vset->text()+"\n";
   // mSocket->write( msg.toLatin1());
    mSocket->write( msg.toUtf8());
}

void MainWindow::on_btn_setC_clicked()
{
    QString msg ="Curr "+ ui->lineEdit_Cset->text()+"\n";
     QMessageBox::information(this,"提示",msg);
    mSocket->write( msg.toLatin1());
}

void MainWindow::on_pushButton_DeviceInfo_3_clicked()
{
    QString msg = "SYST:LOC\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_MeaCurr_clicked()
{
    QString msg = "MEAS:CURR?\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_MeaPow_clicked()
{
    QString msg = "MEAS:POW?\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_MeaVol_clicked()
{
    QString msg = "MEAS:VOLT?\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_MeaCap_clicked()
{
    QString msg = "MEAS:CAP?\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_clicked()
{
    QString msg = "MEAS?\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_outpon_clicked()
{
    QString msg = "outp on\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_outpoff_clicked()
{
    QString msg = "outp off\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_Pri_Vol_clicked()
{
    QString msg = "FUNC:PRI VOLT\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_Pri_Cur_clicked()
{
    QString msg = "FUNC:PRI Curr\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_OutpDelay_clicked()
{
    QString msg = "time on\n";
   mSocket->write( msg.toUtf8());
    msg = "time del"+ui->lineEdit_Cset->text()+"\n";
  mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_shutPWM_clicked()
{
    QString msg = "PWM OFF\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_trigerKey_clicked()
{
    QString msg = "TRIG:BATT:SOUR KEYP\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_btn_setChargeV_clicked()
{
    QString msg = "BATT:CHAR:VOLT "+ui->lineEdit_chargeV->text()+"\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_btn_setChargeC_clicked()
{
    QString msg = "BATT:CHAR:CURR "+ui->lineEdit_chargeC->text()+"\n";
   // debug(msg);
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_btn_setChargeT_clicked()
{
    QString msg = "BATT:SHUT:TIME "+ui->lineEdit_chargeT->text()+"\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_btn_shutV_clicked()
{
    QString msg = "BATT:SHUT:VOLT "+ui->lineEdit_shutV->text()+"\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_btn_shutC_clicked()
{
    QString msg = "BATT:SHUT:CURR "+ui->lineEdit_shutC->text()+"\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_btn_shutCap_clicked()
{
    QString msg = "BATT:SHUT:CAP "+ui->lineEdit_shutCap->text()+"\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_battOn_clicked()
{
    QString msg = "BATT ON\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_busTrig_clicked()
{
    QString msg = "TRIG\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_trigerBUS_clicked()
{
    QString msg = "TRIG:BATT:SOUR BUS\n";
   mSocket->write( msg.toUtf8());
}

void MainWindow::on_pushButton_clear_reply_clicked()
{
    ui->recvmsg->clear();
}
