#ifndef CHANGEPWD_H
#define CHANGEPWD_H

#include <QWidget>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QString>
namespace Ui {
class ChangePwd;
}

class ChangePwd : public QWidget
{
    Q_OBJECT

public:
    explicit ChangePwd(QWidget *parent = nullptr);
    ~ChangePwd();
signals:
    void back();
private slots:
    void on_CancelBtn_clicked();

    void on_SureBtn_clicked();

private:
    Ui::ChangePwd *ui;
    QString userName;
    QString oldPwd;
    QString newPwd;
    QString SureNewPwd;
};

#endif // CHANGEPWD_H
