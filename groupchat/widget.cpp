#include "widget.h"
#include "ui_widget.h"
#include <QDataStream>
#include <QMessageBox>
#include <QDateTime>
#include <QFont>
#include <QColor>
#include <QColorDialog>
#include <QFile>
#include <QFileDialog>
Widget::Widget(QWidget *parent,QString name) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    myName=name;
    //初始化端口
    this->port=9999;
    //创建一个套接字
    this->udpSocket=new QUdpSocket;
    //绑定端口
    udpSocket->bind(this->port,QUdpSocket::ShareAddress|QUdpSocket::ReuseAddressHint);
    //监听接收信号
    connect(udpSocket,&QUdpSocket::readyRead,this,&Widget::ReceiveMsg);
    //连接发送按钮，点击发送，则发送消息信号
    connect(ui->sendBtn,&QPushButton::clicked,[=](){
       sendMsg(Msg);
    });
    //创建聊天窗口立马发送用户进入信号
    sendMsg(UserEnter);
    //退出按钮的功能实现
    connect(ui->quitBtn,&QPushButton::clicked,[=](){
        this->close();
    });
    //字体类型
    connect(ui->fontComboBox,&QFontComboBox::currentFontChanged,[=](const QFont &font){
        ui->msgEditBox->setFontFamily(font.toString());
        ui->msgEditBox->setFocus();

    });
    //字体大小
    //字体大小有两种类型的槽函数（即有重载）需要用指针重写区分
    void (QComboBox::*sizeBtn)(const QString &text)=&QComboBox::currentTextChanged;
    connect(ui->fontSizecbox,sizeBtn,[=](const QString &text){
       ui->msgEditBox->setFontPointSize(text.toDouble());
       ui->msgEditBox->setFocus();
    });
    //字体加粗
    connect(ui->bolderBtn,&QToolButton::clicked,[=](bool checked){
        if(checked)
        {
            ui->msgEditBox->setFontWeight(QFont::Bold);
        }
        else {
            ui->msgBrowser->setFontWeight(QFont::Normal);

        }
    });
    //字体倾斜
    //checked是信号中的参数，槽函数的参数要于信号中的参数一致。在qt帮助文档中，checked默认为false，
    //点击后变成true。值是如何传递的？？点击之后checked值为什么会发生变化，
    //信号只要写声明不写实现，怎么做到的？
    connect(ui->italicBtn,&QToolButton::clicked,[=](bool checked){
        ui->msgEditBox->setFontItalic(checked);
        ui->msgEditBox->setFocus();
    });
    //字体下划线
    connect(ui->underlineBtn,&QToolButton::clicked,[=](bool checked){
       ui->msgEditBox->setFontItalic(checked);
       ui->msgEditBox->setFocus();

    });
    //清空
    connect(ui->clearBtn,&QToolButton::clicked,[=](){
         ui->msgBrowser->clear();
    });
    //颜色设置
    connect(ui->colorBtn,&QToolButton::clicked,[=](){
       QColor color=QColorDialog::getColor(color,this);
       ui->msgEditBox->setTextColor(color);
    });
    //文件保存
    connect(ui->saveBtn,&QToolButton::clicked,[=](){
        if(ui->msgBrowser->document()->isEmpty()){
            QMessageBox::warning(this,"警告","保存内容不能为空");
        }
        else{
            QString filename=QFileDialog::getSaveFileName(this,"保存聊天记录","聊天记录","*.txt");
            if(!filename.isEmpty()){
                QFile file(filename);
                file.open(QIODevice::WriteOnly|QFile::Text);
                QTextStream stream(&file);
                stream<<ui->msgBrowser->toPlainText();
                file.close();
            }
        }
    });
}


Widget::~Widget()
{
    delete ui;
}

//重写关闭事件
void Widget::closeEvent(QCloseEvent *e)
{
    //触发自定义关闭窗口信号
    emit this->closeWidget();
    //关闭窗口，发送用户离开信号
    sendMsg(UserLeave);
    //销毁套接字
    udpSocket->close();
    udpSocket->destroyed();

    QWidget::closeEvent(e);
}

//点击发送按钮时或者用户退出进入时调用此函数，获取数据，并通过udp发送数据报
//发送信号函数，发送 用户离开 用户登录 用户发送消息三种信号，并且获取三种情况下的数据,并通过udp发送数据报
void Widget::sendMsg(Widget::MsgType type)
{
    //定义数据数组
    QByteArray array;
    //以array定义一个数据流，
    QDataStream stream(&array,QIODevice::WriteOnly);
    //数据流入array中，获取信号种类，和发送信号的用户名
    stream<<type<<this->getName();
    switch (type)
    {
        //当为发送消息时，再获取消息的内容
        case Msg:
            if(ui->msgEditBox->toPlainText()==""){
                QMessageBox::warning(this,"警告","发送内容不能为空");
                return;
            }
            //获取消息内容
            stream<<this->getMsg();
            break;
        case UserEnter:

            break;
        case UserLeave:
            break;
    }

    //书写数据报，传送到9999端口
    udpSocket->writeDatagram(array.data(),array.size(),QHostAddress::Broadcast,this->port);
}

//获取用户名
QString Widget::getName()
{
    return this->myName;
}

//获取发送消息的内容 点击发送才调用
QString Widget::getMsg()
{
    QString msg=ui->msgEditBox->toHtml();//返回输入框中的内容
    //清除输入框内容 点击发送才会调用sendMsg函数，再调用getMsg函数，所以点击发送就会清空输入框
    ui->msgEditBox->clear();
    //点击发送后，输入框继续获取焦点，使得其可以继续输入，无需手动点击输入框
    ui->msgEditBox->setFocus();
    return msg;
}

void Widget::userEnter(QString userName)
{
    bool isEmpty=ui->tableBrowser->findItems(userName,Qt::MatchExactly).isEmpty();
    //没有此用户则添加
    if(isEmpty)
    {
        QTableWidgetItem *user=new QTableWidgetItem(userName);
        ui->tableBrowser->insertRow(0);
        ui->tableBrowser->setItem(0,0,user);
        ui->msgBrowser->setTextColor(Qt::gray);
        ui->msgBrowser->append(userName+"用户已上线");
        ui->numLabel->setText(QString("在线人数:%1人").arg(ui->tableBrowser->rowCount()));

        //还是不懂udp,tcp通信协议，不知道此处怎么会实现如此一个效果
        //加了这句，保证每个聊天界面都会有之前登录用户的登录信息
        sendMsg(UserEnter);
    }
}

void Widget::userLeave(QString userName, QString time)
{
    bool isEmpty=ui->tableBrowser->findItems(userName,Qt::MatchExactly).isEmpty();
    if(!isEmpty)
    {
        //寻找行
        int row=ui->tableBrowser->findItems(userName,Qt::MatchExactly).first()->row();
        //移除改行
        ui->tableBrowser->removeRow(row);
        //追加信息
        ui->msgBrowser->setTextColor(Qt::gray);
        ui->msgBrowser->append(userName+"用户于"+time+"下线");
        ui->numLabel->setText(QString("在线人数:%1人").arg(ui->tableBrowser->rowCount()));
    }
}

void Widget::ReceiveMsg()
{
    //获取udp信号的数据报大小
    qint64 size=udpSocket->pendingDatagramSize();
    //转换为整型
    int mysize=static_cast<int>(size);
    //初始化接收数据数组为0
    QByteArray *array=new QByteArray(mysize,0);
    //读取数据报中的数据到array中去
    udpSocket->readDatagram(array->data(),size);
    //以array创建数据流
    QDataStream stream(array,QIODevice::ReadOnly);
    //发送信息的类型
    int mytype;
    //用户名和发送的消息,以及消息发送时间
    QString name,msg;
    //获取当前时间
    QString msgTime=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    //从数据流中读取mytype
    stream>>mytype;
    switch (mytype) {
    case Msg:
        stream>>name>>msg;
        ui->msgBrowser->setTextColor(QColor(Qt::blue));
        ui->msgBrowser->append("["+name+"]"+msgTime);
        ui->msgBrowser->append(msg);
        break;
    case UserLeave:
        stream>>name;
        userLeave(name,msgTime);
        break;
    case UserEnter:
        stream>>name;
        userEnter(name);
        break;
    }

}
