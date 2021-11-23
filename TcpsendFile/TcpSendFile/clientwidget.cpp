#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QFileInfo>
ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);
    isStart=true;
    ui->progressBar->setValue(0);
    tcpSocket=new QTcpSocket(this);

    this->setWindowTitle("客户端");

    connect(tcpSocket,&QTcpSocket::readyRead,[=](){
       //取出接收的内容
        QByteArray buf=tcpSocket->readAll();
        //接收头信息
        if(isStart)
        {
            isStart=false;
            //解析头部信息 格式为hello##1024 以##作为分隔符 取出第一段开始，第一段结束的内容 即hello
            //初始化
            fileName=QString(buf).section("##",0,0);
            fileSize=QString(buf).section("##",1,1).toInt();
            receiveSize=0;

            //提示接收文件的信息
            ui->textEdit->append(QString("接收到文件%1,大小为%2KB").arg(fileName).arg(fileSize/1024));
            //打开文件
            file.setFileName(QString("D:/%1").arg(fileName));

            bool isOk=file.open(QIODevice::WriteOnly);
            if(!isOk)
            {
                qDebug()<<"打开出错";
            }
        }
        //文件信息
        else {
            qint64 len=file.write(buf);
            receiveSize+=len;
            ui->progressBar->setValue(int(receiveSize/fileSize*100));
            if(receiveSize==fileSize)
            {
                file.close();
                QMessageBox::information(this,"完成","文件接收完成");
                isStart=true;
                ui->progressBar->setValue(0);
            }
        }
    });

    connect(tcpSocket,&QTcpSocket::connected,this,[=](){
        QString ip=tcpSocket->peerAddress().toString();
        quint16 port=tcpSocket->peerPort();
        QString str=QString("成功和服务器[%1:%2]连接").arg(ip).arg(port);
        ui->textEdit->append(str);
    });
}

ClientWidget::~ClientWidget()
{
    delete ui;
}

void ClientWidget::on_connectBtn_clicked()
{
    //获取服务器的ip 和端口
    QString ip=ui->IPLine->text();
    quint16 port=static_cast<quint16>(ui->portLine->text().toInt());
    tcpSocket->connectToHost(QHostAddress(ip),port);
}
