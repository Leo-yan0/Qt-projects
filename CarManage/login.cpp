#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowTitle("登录");

    //添加mysql数据库
    QSqlDatabase db= QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("ly12345");
    db.setDatabaseName("Qsql");

    //打开数据库
    if(!db.open())
    {
        QMessageBox::warning(this,"failure",db.lastError().text());
        return;
    }
//   QSqlQuery query;
//    //一行一行遍历
//    query.exec("select * from student");
//    while(query.next())
//    {
//        //取出当前行的内容
//        qDebug()<<query.value(0).toInt()<<query.value(1).toString()
//               <<query.value("age").toInt()<<query.value("score").toInt();

//    }
}

Login::~Login()
{
    delete ui;
}



void Login::on_loginBtn_clicked()
{
    this->userName=ui->nameLine->text();
    this->passWord=ui->pwdLine->text();
    QSqlDatabase::database();
    QSqlQuery query;
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
    //获取到的密码等于输入的密码
    if(userPwd==this->passWord)
    {
        QMessageBox::information(this,"success","登录成功!");
        myMainWindow=new MainWindow;
        //设置显示的位置为之前login窗口的位置
        myMainWindow->setGeometry(this->geometry());

        this->hide();
        myMainWindow->show();
    }
    else{
        QMessageBox::information(this,"failed","用户名或密码错误");
    }
}

void Login::on_closeBtn_clicked()
{
    this->close();
}
