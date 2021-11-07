#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    tcpServer=nullptr;
    tcpSocket=nullptr;
    this->setWindowTitle("服务器(端口:8000)");
    //创建一个监听socket
    tcpServer=new QTcpServer(this);//指定父类对象，自动回收空间

    //监听端口 ip地址+端口号  此端口号即是服务器端口号
    tcpServer->listen(QHostAddress::Any,8000);

    //tcpServer 连接成功会发出 newConnection信号 并且通过nextPendingConnection生成一个通信socket
    //将其赋值给我们自定义的tcpSocket指针，方便后期使用和维护
    connect(tcpServer,&QTcpServer::newConnection,[=](){
        //取出通信socket
        tcpSocket=tcpServer->nextPendingConnection();

        //获取客户端的ip和端口
        QString IPAdress=tcpSocket->peerAddress().toString();
        quint16 port=tcpSocket->peerPort();
        QString temp=QString("[%1:%2]:成功链接").arg(IPAdress).arg(port);

        ui->textShow->setText(temp);


        //接收数据  只要链接端口的客户端向服务端传送数据 数据传送成功就会触发readyRead()信号
        connect(tcpSocket,&QTcpSocket::readyRead,[=](){
            //从通信socket获取数据
            QByteArray array=tcpSocket->readAll();
            //把数据写到展示框中
            ui->textShow->append(array);
        });
    });





}

Widget::~Widget()
{
    delete ui;
}


//点击发送按钮
void Widget::on_sendBtn_clicked()
{
    if(nullptr==tcpSocket)
        return;
    //获取服务器编辑区内容
    QString str=ui->textEdit->toPlainText();

    //给链接端口的客户端发送数据
    tcpSocket->write(str.toUtf8().data());

    //清空输入框，获取光标
    ui->textEdit->clear();
    ui->textEdit->setFocus();

}

void Widget::on_closeBtn_clicked()
{
    if(nullptr==tcpSocket)
        return;
    //断开链接
    tcpSocket->disconnectFromHost();
    tcpSocket->close();
    tcpSocket=nullptr;


}
