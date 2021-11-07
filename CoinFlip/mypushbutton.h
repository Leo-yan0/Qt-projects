#ifndef MYPUSHBUTTON_H
#define MYPUSHBUTTON_H

#include <QPushButton>
#include <QString>
#include <QMouseEvent>




class MyPushButton : public QPushButton
{
    Q_OBJECT
public:
    //构造函数 参数一 正常显示的图片路径 参数二 按下显示的图片路径
    MyPushButton(QString normalImg,QString pressImg="");
    //成员属性  保存用户传入的默认显示路径 以及按下显示的图片路径
    QString normalImgPath;
    QString pressImgPath;
    //弹跳特效 向上向下跳
    void zoom(int direction);
    //重写鼠标按下和释放事件
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);

    enum{
        MYPUSHBUTTON_UP,MYPUSHBUTTON_DOWN
    };


signals:

public slots:
};

#endif // MYPUSHBUTTON_H
