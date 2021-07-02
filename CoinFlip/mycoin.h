#ifndef MYCOIN_H
#define MYCOIN_H

#include <QPushButton>
#include <QString>
#include <QTimer>
class MyCoin : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyCoin(QWidget *parent = nullptr);

    MyCoin(QString btnImg);

    void changeFlag();  //改变标志  执行翻转效果
    QTimer *timer1;  //正面翻反面
    QTimer *timer2;  //反面翻正面
    int min=1;   //图片最小下标
    int max=8;   //图片最大下标

    int PosX;  //x坐标
    int PosY;  //y坐标
    bool flag; //正反状态
    bool iswin=false; //判断是否胜利 胜利后 按钮禁用

    //执行动画标志
    bool isAnimation=false;

    //重写 按下
    void mousePressEvent(QMouseEvent *e);

signals:

public slots:
};

#endif // MYCOIN_H
