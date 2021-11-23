#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QHostAddress>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>
#include <QTimer>
namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();
    void sendData();

private slots:
    void on_selectBtn_clicked();

    void on_sendBtn_clicked();

private:
    Ui::ServerWidget *ui;
    QTcpServer *tcpServer;  //监听套接字
    QTcpSocket *tcpSocket;  //通信套接字
    QFile file; //文件对象
    QString fileName;
    qint64 fileSize;
    qint64 sendSize;//已经发送文件大小
    QTimer mytimer;

};

#endif // SERVERWIDGET_H
