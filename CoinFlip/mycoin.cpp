#include "mycoin.h"
#include <QDebug>

MyCoin::MyCoin(QString btnImg)
{
    QPixmap pix;
    bool ret=pix.load(btnImg);
    if(!ret)
    {
        qDebug()<<btnImg<<"加载失败";
        return;
    }
    this->setFixedSize(pix.width(),pix.height());
    this->setStyleSheet("QPushButton{border:0px;}");
    this->setIcon(pix);
    this->setIconSize(QSize(pix.width(),pix.height()));

    timer1=new QTimer(this);
    timer2=new QTimer(this);

    //监听正面翻反面的信号，并且翻转金币
    connect(timer1,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str=QString(":/res/Coin000%1.png").arg(this->min++);

        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));

        //判断 如果翻完了 将min重置为1
        if(this->min>this->max)
        {
            this->min=1;
            isAnimation=false;
            timer1->stop();
        }
    });


    //监听反面翻转信息
    connect(timer2,&QTimer::timeout,[=](){
        QPixmap pix;
        QString str=QString(":/res/Coin000%1.png").arg(this->max--);
        pix.load(str);

        this->setFixedSize(pix.width(),pix.height());
        this->setStyleSheet("QPushButton{border:0px;}");
        this->setIcon(pix);
        this->setIconSize(QSize(pix.width(),pix.height()));

        //判断 如果翻完了 将min重置为1
        if(this->max<this->min)
        {
            this->max=8;
            isAnimation=false;
            timer2->stop();
        }
    });
}

void MyCoin::changeFlag()
{
    //如果是正面 翻成反面
    if(this->flag)
    {
        //开始正面的翻转的定时器
        timer1->start(30);
        isAnimation=true;//开始动画
        this->flag=false;
    }
    else //翻反面
    {
        timer2->start(30);
        isAnimation=true; //开始动画
        this->flag=true;
    }

}

void MyCoin::mousePressEvent(QMouseEvent *e)
{
    if(this->isAnimation==true|this->iswin==true)
    {
        return ;
    }
    else
    {
        QPushButton::mousePressEvent(e);
    }
}
