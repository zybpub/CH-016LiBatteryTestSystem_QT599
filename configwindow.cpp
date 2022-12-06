#include "configwindow.h"
#include "ui_config.h"

ConfigWindow::ConfigWindow(QWidget *parent) :
    QDialog(parent),
    ui_config(new Ui::ConfigWindow)
{
    ui_config->setupUi(this);
}

ConfigWindow::~ConfigWindow()
{
    delete ui_config;
}
