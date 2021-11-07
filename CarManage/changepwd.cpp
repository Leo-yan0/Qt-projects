#include "changepwd.h"
#include "ui_changepwd.h"

ChangePwd::ChangePwd(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChangePwd)
{
    ui->setupUi(this);
    this->setWindowTitle("修改密码");





}

ChangePwd::~ChangePwd()
{
    delete ui;
}

void ChangePwd::on_CancelBtn_clicked()
{
    this->hide();
    emit back();
}

void ChangePwd::on_SureBtn_clicked()
{
    QSqlDatabase::database();
    QSqlQuery query;

    this->userName=ui->LineUserName->text();
    this->oldPwd=ui->LineOriginalPwd->text();
    this->newPwd=ui->LineNewPwd->text();
    this->SureNewPwd=ui->LineSurePwd->text();

    //执行查询语句 执行失败 则返回信息
    if(!query.exec(QString("select password from carusers where username='%1'").arg(userName)))
    {
        QMessageBox::warning(this,"warning",query.lastError().text());
    }
    QString userPwd;
    //获取用户名对应的密码
    if(query.next())
    {
        //qDebug()<<query.value(0).toString();
        userPwd=query.value(0).toString();
    }
    //如果输入的旧密码与正确的旧密码不同;报错，并且清空信息
    if(userPwd!=oldPwd)
    {
         QMessageBox::warning(this,"warning","旧密码错误");
         ui->LineOriginalPwd->clear();
         ui->LineSurePwd->clear();
         ui->LineNewPwd->clear();
         return;
    }
    //如果确认密码和新密码不同，报错并且清空信息
    if(newPwd!=SureNewPwd)
    {
        QMessageBox::warning(this,"warning","确认密码与新密码不同");
        ui->LineSurePwd->clear();
        ui->LineNewPwd->clear();
        return;
    }
    else{
        if(!query.exec(QString("update carusers set password='%1' where username='%2'").arg(SureNewPwd)
                   .arg(userName)))
        {
            QMessageBox::information(this,"fail",query.lastError().text());
        }
        else {
            QMessageBox::information(this,"success","修改成功");
            ui->LineUserName->clear();
            ui->LineOriginalPwd->clear();
            ui->LineSurePwd->clear();
            ui->LineNewPwd->clear();

            this->hide();
            emit back();

        }
    }
}
