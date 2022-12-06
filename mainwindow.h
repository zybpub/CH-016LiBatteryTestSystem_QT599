#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QSystemTrayIcon> //系统托盘图标
#include <QMessageBox>
#include <QTimer>
#include <QVector>
#include <QMenu>
#include <QAction>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
   //菜单槽
    void on_open_config_wind_clicked();
  void  on_menu_close_clicked();


    void on_start_clicked();
  void connect_suc();
     void read_data();
     void on_pushButton1260_clicked();

     void on_stop_clicked();

     void on_pushButton1500_clicked();

     void on_pushButton_displayError_clicked();

     void on_pushButton_DeviceInfo_clicked();

     void on_pushButton_DeviceInfo_2_clicked();

     void on_pushButton_500_clicked();

     void on_btn_setV_clicked();

     void on_btn_setC_clicked();

     void on_pushButton_DeviceInfo_3_clicked();

     void on_pushButton_MeaCurr_clicked();

     void on_pushButton_MeaPow_clicked();

     void on_pushButton_MeaVol_clicked();

     void on_pushButton_MeaCap_clicked();

     void on_pushButton_clicked();

     void on_pushButton_outpon_clicked();

     void on_pushButton_outpoff_clicked();

     void on_pushButton_Pri_Vol_clicked();

     void on_pushButton_Pri_Cur_clicked();

     void on_pushButton_OutpDelay_clicked();

     void on_pushButton_shutPWM_clicked();

     void on_pushButton_trigerKey_clicked();

     void on_btn_setChargeV_clicked();

     void on_btn_setChargeC_clicked();

     void on_btn_setChargeT_clicked();

     void on_btn_shutV_clicked();

     void on_btn_shutC_clicked();

     void on_btn_shutCap_clicked();

     void on_pushButton_battOn_clicked();

     void on_pushButton_busTrig_clicked();

     void on_pushButton_trigerBUS_clicked();

     void on_pushButton_clear_reply_clicked();

     void debug(QString msg);
private:
    Ui::MainWindow *ui;

    QTcpSocket *mSocket;
    QTimer *mtimer;
    bool isServer;           //服务端为true,客户端为false
    QSystemTrayIcon Icon;    //托盘图标对象
    QMenu *menu;
    int online_num;          //在线用户数
    qint64 recvsize;         //接收字节数
    qint64 sendsize;         //发送字节数

    QVector <QTcpSocket *> clients; //容器用于存客户端套接字
    QAction *ac1,*ac2;
};
#endif // MAINWINDOW_H