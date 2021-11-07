#include "mypushbutton.h"
#include <QPixmap>
#include <QDebug>
#include <QPropertyAnimation>
MyPushButton::MyPushButton(QString normalImg, QString pressImg)
{
    this->normalImgPath=normalImg;
    this->pressImgPath=pressImg;

    QPixmap pix;
    bool ret =pix.load(normalImg);
    if(!ret)
    {
        qDebug()<<normalImg<<" 图片加载失败";
        return ;
    }

    //设置图片固定大小
    this->setFixedSize(pix.width(),pix.height());

    //设置不规则图片的样式
    this->setStyleSheet("QPushButton{border:0px;}");

    //设置图标
    this->setIcon(pix);

    //设置图标大小
    this->setIconSize(QSize(pix.width(),pix.height()));



}

void MyPushButton::zoom(int direction)
{
    //创建动画对象
    QPropertyAnimation *animation=new QPropertyAnimation(this,"geometry");
    //设置动画间隔
    animation->setDuration(200); //200ms

    if(direction==MYPUSHBUTTON_UP)
    {
        //起始位置
        animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
        //结束位置
        animation->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    }
    else  if(direction==MYPUSHBUTTON_DOWN)
    {
        //起始位置
        animation->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
        //结束位置
        animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));
    }


    //设置弹跳曲线
    animation->setEasingCurve(QEasingCurve::OutBounce);

    //执行动画
    animation->start();


}

void MyPushButton::mousePressEvent(QMouseEvent *e)
{
    //选中路径不为空，显示图片
    if(this->pressImgPath!="")
    {
        QPixmap pix;
        bool ret=pix.load(pressImgPath);
        if(!ret)
        {
            qDebug()<<pressImgPath<<"加载失败";
        }
        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.size()));
    }
    //交给父类执行按下事件
    return QPushButton::mousePressEvent(e);
}

void MyPushButton::mouseReleaseEvent(QMouseEvent *e)
{

    if(normalImgPath != "") //选中路径不为空，显示选中图片
    {
            QPixmap pixmap;
            bool ret = pixmap.load(normalImgPath);
            if(!ret)
            {
                qDebug() << normalImgPath << "加载图片失败!";
            }
            this->setFixedSize( pixmap.width(), pixmap.height() );
            this->setStyleSheet("QPushButton{border:0px;}");
            this->setIcon(pixmap);
            this->setIconSize(QSize(pixmap.width(),pixmap.height()));
    }
    return QPushButton::mouseReleaseEvent(e);
}

