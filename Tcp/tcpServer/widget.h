#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_sendBtn_clicked();

    void on_closeBtn_clicked();

private:
    Ui::Widget *ui;
    QTcpServer *tcpServer; //定义监听socket
    QTcpSocket *tcpSocket;//定义通信socket
};

#endif // WIDGET_H
