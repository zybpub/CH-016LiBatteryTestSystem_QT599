#ifndef CONFIG_H
#define CONFIG_H

#include <QDialog>

namespace Ui {
class ConfigWindow;
}

class ConfigWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigWindow(QWidget *parent = nullptr);
    ~ConfigWindow();

private:
    Ui::ConfigWindow *ui_config;
};

#endif // CONFIG_H
