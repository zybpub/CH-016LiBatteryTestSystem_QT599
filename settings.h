#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>

namespace Ui {
class Settings;
}

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget *parent = nullptr);
    ~Settings();
    //覆写closeEvent函数
        void closeEvent(QCloseEvent *);
private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
