#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QUdpSocket>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent,QString name);
    ~Widget();
    //重写关闭事件
    void closeEvent(QCloseEvent*e);

public:
    enum MsgType{Msg,UserEnter,UserLeave}; //枚举 分别代表普通信息，用户进入，用户离开
    void sendMsg(MsgType type);//广播UDP信息
    QString getName();//获取用户名字
    QString getMsg();//获取聊天内容
    void userEnter(QString userName);//处理用户进入
    void userLeave(QString userName,QString time); //处理用户离开
    void ReceiveMsg();//接收UDP信息

signals:
    void closeWidget();
private:
    Ui::Widget *ui;
    QString myName;
    quint16 port;//端口
    QUdpSocket *udpSocket;//套接字
};

#endif // WIDGET_H
