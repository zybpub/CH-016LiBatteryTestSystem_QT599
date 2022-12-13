#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings.h"
#include <QFile>
#include <QAction>
#include <QJsonObject>
#include <QJsonDocument>
#include <QByteArray>
#include <QFile>
#include <QJsonArray>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    remote_control=false;

//    connect(ui->menu_modify,&QAction::triggered,this,config_wind_clicked());
//     connect(ui->menu_open,&QAction::triggered,this,menu_open_clicked());
//    connect(ui->menu_close,&QAction::triggered,this,menu_close_clicked());
//    connect(ui->menu_about,&QAction::triggered,this,about_triggered());

    connect(ui->menu_modify,SIGNAL(triggered()),this,SLOT(config_wind_clicked()));
     connect(ui->menu_open,SIGNAL(triggered()),this,SLOT(menu_open_clicked()));
    connect(ui->menu_close,SIGNAL(triggered()),this,SLOT(menu_close_clicked()));
    connect(ui->menu_about,SIGNAL(triggered()),this,SLOT(about_triggered()));

    connect( ui->actionVer,&QAction::triggered,[=](){
           QMessageBox::information(this,"软件版本","V1.0");
        });

    read_json_data();
    //创建QTcpSocket对象，并初始化
    mSocket = new QTcpSocket;
}

void MainWindow::read_json_data(){
    QFile file("info.json");
    file.open(QIODevice::ReadOnly);
    QByteArray data=file.readAll();
    file.close();

    //解析
    QJsonParseError parseError;
    QString str=QString(data);
    //qDebug()<<str;
    QJsonDocument doc=QJsonDocument::fromJson(data,&parseError);
    if(parseError.error!=QJsonParseError::NoError){
        qDebug()<<"配置错误";
        return;
    }
    obj=doc.object();
    if(obj.contains("充电"))
    {
        QJsonValue Temp=obj.value("充电");
        QJsonObject objs=Temp.toObject();

        ui->lineEdit_chargeV->setText(objs.value("充电电压").toString());
        ui->lineEdit_chargeC->setText(objs.value("充电电流").toString());
        ui->lineEdit_chargeT->setText(objs.value("充电时间").toString());
        ui->lineEdit_shutV->setText(objs.value("关断电压").toString());
        ui->lineEdit_shutC->setText(objs.value("关断电流").toString());
        ui->lineEdit_shutCap->setText(objs.value("关断容量").toString());

//        QJsonValue genderTemp=objs.value("充电电流");
//        QString gender=genderTemp.toString();

//        QJsonValue chargeV=objs.value("充电时间");
//        QString id=chargeV.toString();
    }
    if(obj.contains("放电"))
    {
        QJsonValue Temp=obj.value("放电");
        QJsonObject objs=Temp.toObject();

        ui->lineEdit_dischargeV->setText(objs.value("放电电压").toString());
        ui->lineEdit_dischargeC->setText(objs.value("放电电流").toString());
        ui->lineEdit_dischargeT->setText(objs.value("放电时间").toString());
        ui->lineEdit_disshutV->setText(objs.value("关断电压").toString());
        ui->lineEdit_disshutC->setText(objs.value("关断电流").toString());
        ui->lineEdit_disCap->setText(objs.value("关断容量").toString());
    }
    if(obj.contains("网络设置"))
    {
        QJsonValue Temp=obj.value("网络设置");
        QJsonObject objs=Temp.toObject();

        ui->lineEdit_ip->setText(objs.value("ip").toString());
        ui->lineEdit_port->setText(objs.value("端口").toString());
    }


}


MainWindow::~MainWindow()
{
    delete ui;
}
//void MainWindow::timerEvent(QTimerEvent * ev)
//{
//    QString msg = "MEAS?\n";
//    mSocket->write( msg.toUtf8());
//    isRealData=true;
//}

void  MainWindow::debug(QString dmsg){
    //QMessageBox::information(this,"提示",msg);
    qDebug()<<dmsg;
}

void MainWindow::on_start_clicked()
{

    //发起连接connectToHost
    mSocket->connectToHost(ui->lineEdit_ip->text(),ui->lineEdit_port->text().toInt());
    //关联连接成功信号
    connect(mSocket,SIGNAL(connected()),this,SLOT(connect_suc()));
    //关联读数据信号
    connect(mSocket,SIGNAL(readyRead()),this,SLOT(read_data()));
    ui->start->setEnabled(false);



}
//连接成功
void MainWindow::connect_suc()
{
    is_net_connected=true;
    ui->label_info->setText("连接成功");
   // QMessageBox::information(this,"提示","连接成功");
    ui->stop->setEnabled(true);



    msg = "SYST:REM?\n";
    send_cmd(msg);

    //休眠
//    QEventLoop eventloop;
//    QTimer::singleShot(5000, &eventloop, SLOT(quit()));
//    eventloop.exec();

    QTimer::singleShot(2000, this, [=](){
        msg = "outp?\n";
       send_cmd(msg);
    });

    QTimer::singleShot(2000, this, [=](){
        qDebug()<<"等待2秒";
        //定时器
        timer_meas = new QTimer(this);
        //启动定时器
        timer_meas->start(1000);

        connect(timer_meas,&QTimer::timeout,[=](){
            if (cmd_mode==false){
                msg = "MEAS?\n";
                send_cmd(msg);
                isRealData=true;
            }
        });
    });


}

//菜单打开配置窗体
void MainWindow::config_wind_clicked()
{
    settings =new Settings(this);
    settings->show();
}

void MainWindow::about_triggered()
{


    about =new About(this);
    about->show();

    //模态窗体
    //    QDialog dlg(this);
    //    dlg.exec();
    //    qDebug() <<"模态窗体弹出";

    //非模态
//    QDialog *dlgabout=new QDialog(this);
//    dlgabout->resize(400,300);
//    dlgabout->show();
//    qDebug() <<"非模态窗体弹出";
}
void MainWindow::menu_open_clicked()
{
    QMessageBox::information(this,"提示","连接成功");
//    ui->stop->setEnabled(true);
    this->close();
}
void MainWindow::menu_close_clicked()
{
    QMessageBox::information(this,"提示","连接成功");
//    ui->stop->setEnabled(true);
    this->close();
}
void MainWindow::send_cmd(QString cmd){
    if (is_net_connected==false) on_start_clicked();
  //  qDebug()<<cmd;
   // timer_meas->stop();
    cmd_mode=true;
     mSocket->write(cmd.toLatin1());
    debug("发送指令:"+cmd);

     QTimer::singleShot(3000, this, [=](){
         cmd_mode=false;
      //   timer_meas->start(1000);
     });

}
//读数据
void MainWindow::read_data()
{
    //获取客户端IP
    QTcpSocket *obj = (QTcpSocket *)sender();
    QString rec = obj->readAll();
     debug("收到指令:"+msg);
    debug("仪表回复:"+rec);
    if (msg=="outp?\n"){
        if (rec=="1\n") {  //输出已打开
            ui->cb_outp_ison->setCheckState(Qt::Checked);
        }
        else
        {  //输出已关闭
            ui->cb_outp_ison->setCheckState(Qt::Unchecked);
        }
        msg="";
    }
    if (msg=="SYST:REM?\n"){
        if (rec=="1\n") {
            ui->cb_remote_allowed->setCheckState(Qt::Checked);
        }
        else
        {
            ui->cb_remote_allowed->setCheckState(Qt::Unchecked);
        }
    }

    //if (isRealData){  //显示当前实时测试值 12.6002,0.186036,2.34408,0.148254,1.86798
        QStringList values=rec.split(",");
        if (values.length()==5)
        {
            ui->lineEdit_realV->setText(values[0]);
            ui->lineEdit_realA->setText(values[1]);
            ui->lineEdit_realW->setText(values[2]);
            ui->lineEdit_realAH->setText(QString::number(values[3].toDouble(),'f',4));
            ui->lineEdit_realWH->setText(QString::number(values[4].toDouble(),'f',4));

        }
//        isRealData=false;
        else
        {

         QString peerIP = obj->peerAddress().toString().remove("::ffff:");
         ui->recvmsg->addItem(peerIP + ":" + rec);
        }
       //  cmd_mode=false;
//        //自动下滚
//        ui->recvmsg->setCurrentRow(ui->recvmsg->count() - 1);
//        //接收字节数
//        recvsize += msg.size();
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
    msg = "volt 12.60\n";
    send_cmd(msg);
}

void MainWindow::on_pushButton1500_clicked()
{
    msg = "volt 15.00\n";
    send_cmd(msg);
}

void MainWindow::on_pushButton_displayError_clicked()
{
    msg = "syst:err?\n";
    send_cmd(msg);

}

void MainWindow::on_pushButton_DeviceInfo_clicked()
{
     msg = "*idn?\n";
     send_cmd(msg);
}

void MainWindow::on_pushButton_DeviceInfo_2_clicked()
{
    msg = "SYST:REM\n";
   send_cmd(msg);
    remote_control=true;
    on_pushButton_query_remote_clicked();
}

void MainWindow::on_pushButton_500_clicked()
{
     msg = "volt 5.00\n";
    send_cmd(msg);
}

void MainWindow::on_btn_setV_clicked()
{
     msg ="volt "+ ui->lineEdit_Vset->text()+"\n";
    // mSocket->write( msg.toLatin1());
     send_cmd(msg);
}

void MainWindow::on_btn_setC_clicked()
{
    msg ="Curr "+ ui->lineEdit_Cset->text()+"\n";
    send_cmd(msg);
}

void MainWindow::on_pushButton_DeviceInfo_3_clicked()
{
     msg = "SYST:LOC\n";
    send_cmd(msg);
    on_pushButton_query_remote_clicked();
}

void MainWindow::on_pushButton_MeaCurr_clicked()
{
     msg = "MEAS:CURR?\n";
     send_cmd(msg);
}

void MainWindow::on_pushButton_MeaPow_clicked()
{
     msg = "MEAS:POW?\n";
     send_cmd(msg);
}

void MainWindow::on_pushButton_MeaVol_clicked()
{
     msg = "MEAS:VOLT?\n";
    send_cmd(msg);
}

void MainWindow::on_pushButton_MeaCap_clicked()
{
     msg = "MEAS:CAP?\n";
     send_cmd(msg);
}

void MainWindow::on_pushButton_clicked()
{
     msg = "MEAS?\n";
    send_cmd(msg);
}

void MainWindow::on_pushButton_outpon_clicked()
{
     msg = "outp on\n";
     send_cmd(msg);
     QTimer::singleShot(5000, this, [=](){
         msg = "outp?\n";
         send_cmd(msg);
     });

}

void MainWindow::on_pushButton_outpoff_clicked()
{
     msg = "outp off\n";
    send_cmd(msg);
    QTimer::singleShot(5000, this, [=](){
        msg = "outp?\n";
        send_cmd(msg);
    });
}

void MainWindow::on_pushButton_Pri_Vol_clicked()
{
     msg = "FUNC:PRI VOLT\n";
     send_cmd(msg);
}

void MainWindow::on_pushButton_Pri_Cur_clicked()
{
     msg = "FUNC:PRI Curr\n";
    send_cmd(msg);
}

void MainWindow::on_pushButton_OutpDelay_clicked()
{
     msg = "time on\n";
  send_cmd(msg);
    msg = "time del"+ui->lineEdit_Cset->text()+"\n";
    send_cmd(msg);
}

void MainWindow::on_pushButton_shutPWM_clicked()
{
     msg = "PWM OFF\n";
   send_cmd(msg);
}

void MainWindow::on_pushButton_trigerKey_clicked()
{
     msg = "TRIG:BATT:SOUR KEYP\n";
    send_cmd(msg);
}

void MainWindow::on_btn_setChargeV_clicked()
{
     msg = "BATT:CHAR:VOLT "+ui->lineEdit_chargeV->text()+"\n";
    send_cmd(msg);
}

void MainWindow::on_btn_setChargeC_clicked()
{
     msg = "BATT:CHAR:CURR "+ui->lineEdit_chargeC->text()+"\n";
   send_cmd(msg);
}

void MainWindow::on_btn_setChargeT_clicked()
{
     msg = "BATT:SHUT:TIME "+ui->lineEdit_chargeT->text()+"\n";
    send_cmd(msg);
}

void MainWindow::on_btn_shutV_clicked()
{
     msg = "BATT:SHUT:VOLT "+ui->lineEdit_shutV->text()+"\n";
    send_cmd(msg);
}

void MainWindow::on_btn_shutC_clicked()
{
     msg = "BATT:SHUT:CURR "+ui->lineEdit_shutC->text()+"\n";
    send_cmd(msg);
}

void MainWindow::on_btn_shutCap_clicked()
{
     msg = "BATT:SHUT:CAP "+ui->lineEdit_shutCap->text()+"\n";
   send_cmd(msg);
}

void MainWindow::on_pushButton_battOn_clicked()
{
     msg = "BATT ON\n";
    send_cmd(msg);
}

void MainWindow::on_pushButton_busTrig_clicked()
{
     msg = "TRIG\n";
     send_cmd(msg);
}

void MainWindow::on_pushButton_trigerBUS_clicked()
{
     msg = "TRIG:BATT:SOUR BUS\n";
     send_cmd(msg);
}

void MainWindow::on_pushButton_clear_reply_clicked()
{
    ui->recvmsg->clear();
}

//void MainWindow::on_pushButton_2_clicked()
//{
//    QString msg = "MEAS?\n";
//    mSocket->write( msg.toUtf8());
//    isRealData=true;
//}

void MainWindow::on_btn_send_command_clicked()
{
     msg = ui->lineEdit_command->text().toUtf8() +  "\n";
     send_cmd(msg);
}

void MainWindow::on_pushButton_battOff_clicked()
{
     msg = "BATT OFF\n";
    send_cmd(msg);
}

void MainWindow::on_pushButton_save_clicked()
{
    QJsonObject net;

    net.insert("ip",QJsonValue(ui->lineEdit_ip->text()));
    net.insert("端口",QJsonValue(ui->lineEdit_port->text()));

    obj["网络设置"]=QJsonValue(net);

    //写入到文件，名字为info.json
    QJsonDocument doc(obj);
    QByteArray data=doc.toJson();
    QFile file("info.json");
    bool ok=file.open(QIODevice::WriteOnly);
    if(ok)
    {
        file.write(data);
        file.close();
         qDebug()<<"write succ!"<<endl;
          QMessageBox::information(this,"提示","保存成功");
    }
    else
    {
        qDebug()<<"write error!"<<endl;
         QMessageBox::information(this,"提示","保存失败");
    }



}
void MainWindow::on_pushButton_save_charge_clicked()
{
   QJsonObject charge;

   charge.insert("充电电压",QJsonValue(ui->lineEdit_chargeV->text()));
   charge.insert("充电电流",QJsonValue(ui->lineEdit_chargeC->text()));
   charge.insert("充电时间",QJsonValue(ui->lineEdit_chargeT->text()));

   charge.insert("关断电压",QJsonValue(ui->lineEdit_shutV->text()));
   charge.insert("关断电流",QJsonValue(ui->lineEdit_shutC->text()));
   charge.insert("关断容量",QJsonValue(ui->lineEdit_shutCap->text()));


   obj["充电"]=QJsonValue(charge);

   //写入到文件，名字为info.json
   QJsonDocument doc(obj);
   QByteArray data=doc.toJson();
   QFile file("info.json");
   bool ok=file.open(QIODevice::WriteOnly);
   if(ok)
   {
       file.write(data);
       file.close();
        qDebug()<<"write succ!"<<endl;
         QMessageBox::information(this,"提示","保存成功");
   }
   else
   {
       qDebug()<<"write error!"<<endl;
        QMessageBox::information(this,"提示","保存失败");
   }

}
void MainWindow::on_pushButton_save_discharge_clicked()
{
    QJsonObject charge;

    charge.insert("放电电压",QJsonValue(ui->lineEdit_dischargeV->text()));
    charge.insert("放电电流",QJsonValue(ui->lineEdit_dischargeC->text()));
    charge.insert("放电时间",QJsonValue(ui->lineEdit_dischargeT->text()));

    charge.insert("关断电压",QJsonValue(ui->lineEdit_disshutV->text()));
    charge.insert("关断电流",QJsonValue(ui->lineEdit_disshutC->text()));
    charge.insert("关断容量",QJsonValue(ui->lineEdit_disCap->text()));


    obj["放电"]=QJsonValue(charge);

    //写入到文件，名字为info.json
    QJsonDocument doc(obj);
    QByteArray data=doc.toJson();
    QFile file("info.json");
    bool ok=file.open(QIODevice::WriteOnly);
    if(ok)
    {
        file.write(data);
        file.close();
         qDebug()<<"write succ!"<<endl;
          QMessageBox::information(this,"提示","保存成功");
    }
    else
    {
        qDebug()<<"write error!"<<endl;
         QMessageBox::information(this,"提示","保存失败");
    }
}



void MainWindow::on_pushButton_battOn_discharge_clicked()
{
    msg = "BATT ON\n";
    send_cmd(msg);
}

void MainWindow::on_pushButton_battOff__discharge_clicked()
{
    msg = "BATT OFF\n";
    send_cmd(msg);
}

void MainWindow::on_cb_remote_allowed_stateChanged(int arg1)
{
    debug(QString(arg1));
    //第一种
       if(arg1 == 2) //表示被选中
       {
//           msg = "SYST:LOC\n";
//           send_cmd(msg);
//           remote_control=true;
       }
       else if(arg1 == 0)
       {
//           msg = "SYST:REM\n";
//            send_cmd(msg);
//           remote_control=false;
       }
}

void MainWindow::on_pushButton_query_remote_clicked()
{
    msg = "SYST:REM?\n";
    send_cmd(msg);
}

void MainWindow::on_cb_outp_ison_stateChanged(int arg1)
{
    debug(QString(arg1));
    //第一种
       if(arg1 == 2) //表示被选中
       {
//           msg = "outp on\n";
//           send_cmd(msg);
//           remote_control=true;
       }
       else if(arg1 == 0)
       {

//           msg = "outp off\n";
//           send_cmd(msg);
//           remote_control=false;
       }
}

void MainWindow::on_pushButton_query_outp_clicked()
{
    msg = "outp?\n";
    send_cmd(msg);
}
