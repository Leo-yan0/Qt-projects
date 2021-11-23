#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QHostAddress>
#include <QFileDialog>
#include <QFile>
#include <QDebug>
#include <QMessageBox>
namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(QWidget *parent = nullptr);
    ~ClientWidget();

private slots:
    void on_connectBtn_clicked();

private:
    Ui::ClientWidget *ui;
    QTcpSocket *tcpSocket;
    QFile file; //文件对象
    QString fileName;
    qint64 fileSize;
    qint64 receiveSize;//已经接收文件大小
    bool isStart;
};

#endif // CLIENTWIDGET_H
