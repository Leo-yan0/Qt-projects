#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->setWindowTitle("客户端");
    tcpSocket=nullptr;

    //分配空间，指定父对象
    tcpSocket=new QTcpSocket(this);

    connect(tcpSocket,&QTcpSocket::connected,[=](){
       ui->textShow->append("成功和服务器连接");

    });
    connect(tcpSocket,&QTcpSocket::readyRead,[=](){
       //获取发送的数据
        QByteArray array=tcpSocket->readAll();

        ui->textShow->append(array);
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_connetBtn_clicked()
{
    //获取服务器 ip 和端口
    QString ip=ui->IPEdit->text();
    quint16 port=static_cast<quint16>(ui->portEdit->text().toInt());

    //主动和服务器建立连接
    tcpSocket->connectToHost(QHostAddress(ip),port);


}

void Widget::on_sendBtn_clicked()
{
    //获取编辑框内容
    QString str=ui->textEdit->toPlainText();

    //发送数据
    tcpSocket->write(str.toUtf8().data());

    //清空输入框
    ui->textEdit->clear();
    ui->textEdit->setFocus();
}

void Widget::on_closeBtn_clicked()
{
    //断开连接
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
}
