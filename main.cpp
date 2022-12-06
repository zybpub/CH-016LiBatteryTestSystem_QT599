#include "mainwindow.h"
#include "loginwindow.h"
#include "configwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
   // LoginWindow w;
    w.show();
    return a.exec();
}
