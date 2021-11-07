#include "levelchoosescene.h"
#include <QMenuBar>
#include <QPainter>
#include "mypushbutton.h"
#include <QTimer>
#include <QLabel>
#include <QDebug>
#include "playscene.h"
#include <QSound>
LevelChooseScene::LevelChooseScene(QWidget *parent) : QMainWindow(parent)
{
    //配置选择关卡场景
    //设置窗口固定大小
    this->setFixedSize(400,650);

    //设置图标
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("选择关卡");

    //创建菜单栏
    QMenuBar *bar=menuBar();
    setMenuBar(bar);

    //创建开始菜单
    QMenu *startMenu= bar->addMenu("开始");
    //创建退出菜单项
    QAction *quitAction=startMenu->addAction("退出");

    //点击退出实现退出游戏
    connect(quitAction,&QAction::triggered,[=](){
        this->close();
    });

    QSound *chooseLevelSound=new QSound(":/res/LevelWinSound.wav",this);
    QSound *BackSound=new QSound(":/res/BackButtonSound.wav",this);

    //返回按钮
    MyPushButton *backBtn=new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

    //点击按钮，触发自定义信号
    connect(backBtn,&MyPushButton::clicked,[=](){
        BackSound->play();
        QTimer::singleShot(200,this,[=](){
            this->hide();
            emit this->chooseSceneBack();
        });
    });


    //创建选择关卡的按钮
    for(int i=0;i<20;i++)
    {
        //创建20个按钮
        MyPushButton *menuBtn=new MyPushButton(":/res/LevelIcon.png");
        menuBtn->setParent(this);
        //设置位置
        menuBtn->move(60+i%4*70,150+i/4*70);

        //监听每个按钮的点击事件
        connect(menuBtn,&MyPushButton::clicked,[=](){

            //播放选择关卡的音效
            chooseLevelSound->play();

            //qDebug()<<"选择的是第"<<i+1<<"关";
            //进入到游戏关卡

            this->hide();//隐藏选关场景
            //点击后创建新的游戏场景
            play=new PlayScene(i+1);
            //设置游戏场景的初始位置
            play->setGeometry(this->geometry());
            play->show();  //显示Game场景

            connect(play,&PlayScene::chooseSceneBack,[=](){
                this->setGeometry(play->geometry());
                this->show();
                delete play;  //退出后 删除游戏场景
                play=NULL;
            });

        });

        QLabel *label=new QLabel;
        label->setParent(this);
        label->setFixedSize(menuBtn->size());
        label->setText(QString::number(i+1));
        label->move(60+i%4*70,150+i/4*70);

        //设置label上文字的对齐方式
        label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);

        //让鼠标进行穿透
        label->setAttribute(Qt::WA_TransparentForMouseEvents);


    }


}

void LevelChooseScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    //绘制背景图
    pix.load(":/res/OtherSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    //绘制Icon
    pix.load(":/res/Title.png");
    painter.drawPixmap((this->width()-pix.width())/2,30,pix.width(),pix.height(),pix);
}
