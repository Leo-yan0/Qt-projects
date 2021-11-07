#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QTcpServer>
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
    void on_connetBtn_clicked();

    void on_sendBtn_clicked();

    void on_closeBtn_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *tcpSocket;
};

#endif // WIDGET_H
