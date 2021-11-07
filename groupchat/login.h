#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QIcon>
namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);

    ~Login();
signals:


private:

    Ui::Login *ui;
    QVector<bool>IsShow;
};

#endif // LOGIN_H
