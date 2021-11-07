#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
#include "mainwindow.h"
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:


    void on_loginBtn_clicked();

    void on_closeBtn_clicked();

private:
    Ui::Login *ui;
    QString userName;
    QString passWord;
    MainWindow *myMainWindow;
};

#endif // LOGIN_H
