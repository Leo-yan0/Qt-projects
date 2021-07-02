#include "mainscene.h"
#include "ui_mainscene.h"
#include <QIcon>
#include <QAction>
#include <QPainter>
#include "mypushbutton.h"
#include <QDebug>
#include <QTimer>
#include <QSound>  //需要再pro 文件添加 multimedia 模块

MainScene::MainScene(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainScene)
{
    ui->setupUi(this);

    //配置主场景
    //设置窗口固定大小
    this->setFixedSize(400,650);

    //设置图标
    this->setWindowIcon(QIcon(":/res/Coin0001.png"));

    //设置标题
    this->setWindowTitle("翻金币");

    //Quit 按钮的实现
    connect(ui->actionQuit,&QAction::triggered,[=](){
        this->close();
    });

    //准备开始按钮的音效
    QSound *startSound=new QSound(":/res/TapButtonSound.wav",this);
    ////starSound->setLoops(n);循环n次 -1即为无限循环 然后再调用play();
    //开始按钮
    MyPushButton *startBtn=new MyPushButton(":/res/MenuSceneStartButton.png");

    startBtn->setParent(this);
    startBtn->move(this->width()/2-startBtn->width()/2,this->height()*7/10);

    //实例化选择关卡场景
    chooseScene=new LevelChooseScene;

    connect(startBtn,&QPushButton::clicked,[=](){

        //播放音效
        startSound->play();
        //弹跳特效
        startBtn->zoom(MyPushButton::MYPUSHBUTTON_UP);
        startBtn->zoom(MyPushButton::MYPUSHBUTTON_DOWN);

        //延时进入选择关卡场景
        QTimer::singleShot(200,this,[=](){
            //设置关卡选择场景的位置
            chooseScene->setGeometry(this->geometry());
            //隐藏主场景
             this->hide();
            //显示关卡选择场景
            chooseScene->show();
        });

    });

    //监听关卡的返回信号
    connect(chooseScene,&LevelChooseScene::chooseSceneBack,[=](){
        this->setGeometry(chooseScene->geometry());
        chooseScene->hide();
        this->show();  //重新显示主场景
    });

}

MainScene::~MainScene()
{
    delete ui;
}


void MainScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPixmap pix;
    //绘制背景图
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);
    //绘制Icon
    pix.load(":/res/Title.png");
    painter.drawPixmap(10,30,pix.width()/2,pix.height()/2,pix);


}
