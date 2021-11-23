#include "serverwidget.h"
#include "ui_serverwidget.h"

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);

    //分配空间，指定父类对象
    tcpServer=new QTcpServer(this);

    //监听端口
    tcpServer->listen(QHostAddress::Any,8888);
    //设置标题
    this->setWindowTitle("服务器端口：8888");

    //开始创建窗口时，两个按钮都不能按
    ui->selectBtn->setEnabled(false);
    ui->sendBtn->setEnabled(false);

    //如果客户端和服务器连接成功
    connect(tcpServer,&QTcpServer::newConnection,[=](){
        //取出通信套接字
        tcpSocket=tcpServer->nextPendingConnection();
        //获取对方的ip和端口
        QString ip=tcpSocket->peerAddress().toString();
        quint16 port=tcpSocket->peerPort();
        //显示连接信息
        QString str=QString("[%1:%2] 成功连接").arg(ip).arg(port);
        ui->textEdit->append(str);

        //连接成功后，选择文件按钮才能点击
        ui->selectBtn->setEnabled(true);

    });

    connect(&mytimer,&QTimer::timeout,[=](){
        mytimer.stop();
        this->sendData();
    });
}

ServerWidget::~ServerWidget()
{
    delete ui;
}




//选择文件按钮
void ServerWidget::on_selectBtn_clicked()
{

    QString filePath=QFileDialog::getOpenFileName(this,"open","../");
    //选择文件路径有效
    if(!filePath.isEmpty())
    {
        fileName.clear();
        fileSize=0;
        //获取文件信息
        QFileInfo info(filePath);
        fileName=info.fileName();
        fileSize=info.size();
        //发送文件大小初始化为零
        sendSize=0;

        //只读方式打开文件
        file.setFileName(filePath);

        //打开文件
        bool isOk=file.open(QIODevice::ReadOnly);
        if(isOk==false)
        {
            qDebug()<<"只读文件打开失败 ";
        }

        //提示一下打开文件的路径
        ui->textEdit->append(filePath);

        //打开成功，解禁发送按钮
        ui->sendBtn->setEnabled(true);
        //ui->selectBtn->setEnabled(false);

    }
    else
    {
        QMessageBox::warning(this,"警告","此文件路径无效");
    }
}

//发送文件
void ServerWidget::on_sendBtn_clicked()
{
    //发送文件头信息
    QString head=QString("%1##%2").arg(fileName).arg(fileSize);
    //发送头部信息
    qint64 len=tcpSocket->write(head.toUtf8());
    //头信息发送成功
    if(len>0)
    {
        //发送文件数据
        //防止tcp粘包
        //需要定时器延时20ms
        mytimer.start(20);
    }
    else
    {

        qDebug()<<"头部信息发送失败";
        file.close();
        ui->selectBtn->setEnabled(true);
        ui->sendBtn->setEnabled(false);
    }

}

//发送文件数据
void ServerWidget::sendData()
{
    qint64 len=0;
    do{
        //每次发送数据的大小
        char buf[4*1024]={0};
        len=0;

        //往文件中读取数据
        len=file.read(buf,sizeof(buf));

        //发送数据，读到多少，发多少
        len=tcpSocket->write(buf,len);

        //发送数据累加
        sendSize+=len;
    }while(len>0);

    //是否发送文件完毕
    if(sendSize==fileSize)
    {
        ui->textEdit->append("文件发送完毕");
        file.close();

       ui->selectBtn->setEnabled(true);
       ui->sendBtn->setEnabled(false);
    }
}
