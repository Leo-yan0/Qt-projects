#ifndef MYTEXTEDIT_H
#define MYTEXTEDIT_H

#include <QObject>
#include <QWidget>
#include <QTextEdit>
#include <QFocusEvent>
class myTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    //explicit myTextEdit(QObject *parent = nullptr);
    myTextEdit(QWidget *parent = nullptr);

    //鼠标进入事件
    void enterEvent(QEvent *event);



     //鼠标离开
     void leaveEvent(QEvent *event);  //重写leaveEvent函数

     int flag;

signals:




public slots:
};

#endif // MYTEXTEDIT_H
