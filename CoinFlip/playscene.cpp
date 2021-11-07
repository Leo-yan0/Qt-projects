#include "playscene.h"
#include <QDebug>
#include <QMenuBar>
#include <QMenu>
#include <QPixmap>
#include <QTimer>
#include "mypushbutton.h"
#include <QLabel>
#include "mycoin.h"
#include "dataconfig.h"
#include <QPropertyAnimation>
#include <QSound>

PlayScene::PlayScene(int levelIndex)
{
    this->level_index=levelIndex;
    //qDebug()<<"进入到第"<<level_index<<"关";

    //初始化游戏场景
    //设置固定大小
    this->setFixedSize(400,650);
    //设置图标
    this->setWindowIcon(QPixmap(":/res/Coin0001.png"));
    //设置标题
    this->setWindowTitle("Game");

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


    //back按钮的实现
    MyPushButton *backBtn=new MyPushButton(":/res/BackButton.png",":/res/BackButtonSelected.png");
    backBtn->setParent(this);
    backBtn->move(this->width()-backBtn->width(),this->height()-backBtn->height());

    //返回按钮的音效
    QSound *BackSound=new QSound(":/res/BackButtonSound.wav",this);
    //翻金币的音效
    QSound *CoinFlip=new QSound(":/res/ConFlipSound.wav",this);
    //游戏胜利的音效
    QSound *WinSound=new QSound(":/res/LevelWinSound.wav",this);
    //点击按钮，触发自定义信号
    connect(backBtn,&MyPushButton::clicked,[=](){
        BackSound->play();
        QTimer::singleShot(200,this,[=](){
            //qDebug()<<"点击了返回按钮";
            emit this->chooseSceneBack();
        });
    });


    //显示当前关卡
    QLabel *label=new QLabel(this);
    //设置字体
    QFont font;
    font.setFamily("华文新魏");
    font.setPointSize(20);
    QString str=QString("level:%1").arg(this->level_index);
    //将字体设置加载到标签中
    label->setFont(font);
    label->setText(str);

    label->setGeometry(50,this->height()-50,120,50);


    //初始化每个关卡的二维数组
    dataConfig data;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            this->gameArray[i][j]=data.mData[this->level_index][i][j];
        }
    }



    //胜利图片的显示
    QLabel *winlabel=new QLabel(this);
    QPixmap tmpPix;
    tmpPix.load(":/res/LevelCompletedDialogBg.png");
    winlabel->setGeometry(0,0,tmpPix.width(),tmpPix.height());
    winlabel->setPixmap(tmpPix);
    winlabel->move((this->width()-tmpPix.width())/2,-tmpPix.height());


    //显示金币背景图
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<4;j++)
        {
            //绘制金币背景图片
            QLabel *label=new QLabel;
            label->setGeometry(0,0,50,50);
            label->setPixmap(QPixmap(":/res/BoardNode(1).png"));
            label->setParent(this);
            label->move(100+i*50,250+j*50);



            //创建金币
            QString str;
            if(this->gameArray[i][j]==1)
            {
                //显示金币
                str=":/res/Coin0001.png";
            }
            else
            {
                //显示银币
                str=":/res/Coin0008.png";
            }
            MyCoin *coin=new MyCoin(str);
            coin->setParent(this);
            coin->move(103+i*50,253+j*50);

            //给金币属性赋值
            coin->PosX=i;
            coin->PosY=j;
            coin->flag=gameArray[i][j];

            //将金币放入到金币指针二维数组 方便于后期的维护
            btnCoin[i][j]=coin;

            //点击 金币 进行翻转
            connect(coin,&MyCoin::clicked,[=](){
                //点击一个金币后 其他金币都禁止操作
                for(int x=0;x<4;x++)
                    for(int y=0;y<4;y++)
                    {
                        btnCoin[x][y]->iswin=true;
                    }

                //播放翻金币的音效
                CoinFlip->play();

                coin->changeFlag();
                this->gameArray[i][j]=!this->gameArray[i][j];  //翻转金币后 修改内部数据的值

                //翻转周围金币
                QTimer::singleShot(100, this,[=](){
                                    if(coin->PosX+1 <=3)
                                    {
                                      btnCoin[coin->PosX+1][coin->PosY]->changeFlag();
                                      gameArray[coin->PosX+1][coin->PosY] = gameArray[coin->PosX+1][coin->PosY]== 0 ? 1 : 0;
                                    }

                                    if(coin->PosX-1>=0)
                                    {
                                      btnCoin[coin->PosX-1][coin->PosY]->changeFlag();
                                      gameArray[coin->PosX-1][coin->PosY] = gameArray[coin->PosX-1][coin->PosY]== 0 ? 1 : 0;
                                    }

                                    if(coin->PosY+1 <=3)
                                    {
                                      btnCoin[coin->PosX][coin->PosY+1]->changeFlag();
                                      gameArray[coin->PosX][coin->PosY+1] = gameArray[coin->PosX][coin->PosY+1]== 0 ? 1 : 0;
                                    }

                                    if(coin->PosY-1 >=0)
                                    {
                                      btnCoin[coin->PosX][coin->PosY-1]->changeFlag();
                                      gameArray[coin->PosX][coin->PosY-1] = gameArray[coin->PosX][coin->PosY-1]== 0 ? 1 : 0;
                                    }
                                    //金币全翻完了 再恢复按钮的使用
                                    for(int x=0;x<4;x++)
                                        for(int y=0;y<4;y++)
                                        {
                                            btnCoin[x][y]->iswin=false;
                                        }


                                    //判断是否胜利
                                    this->isWin=true;
                                    for(int x=0;x<4;x++)
                                        for(int y=0;y<4;y++)
                                        {
                                            if(btnCoin[x][y]->flag==false)
                                            {
                                                this->isWin=false;
                                                break;
                                            }
                                        }

                                    if(this->isWin==true)
                                    {
                                        //胜利的音效
                                        WinSound->play();
                                        //胜利了
                                        //qDebug()<<"you win";
                                        //禁用按钮
                                        for(int x=0;x<4;x++)
                                            for(int y=0;y<4;y++)
                                            {
                                                btnCoin[x][y]->iswin=true;
                                            }



                                        //让胜利图片移动下来
                                        QPropertyAnimation *animation=new QPropertyAnimation(winlabel,"geometry");
                                        //设置时间间隔
                                        animation->setDuration(1000);
                                        //设置开始位置
                                        animation->setStartValue(QRect(winlabel->x(),winlabel->y(),winlabel->width(),winlabel->height()));
                                        //设置结束的位置
                                        animation->setEndValue(QRect(winlabel->x(),winlabel->y()+200,winlabel->width(),winlabel->height()));
                                        //设置缓和曲线
                                        animation->setEasingCurve(QEasingCurve::OutBounce);
                                        //执行动画
                                        animation->start();

                                    }

                                });

            });
        }
    }

}

void PlayScene::paintEvent(QPaintEvent *)
{
    //加载背景和标题图片
    QPainter painter(this);
    QPixmap pix;
    pix.load(":/res/PlayLevelSceneBg.png");
    painter.drawPixmap(0,0,this->width(),this->height(),pix);

    pix.load(":/res/Title.png");
    painter.drawPixmap(10,30,pix.width()/2,pix.height()/2,pix);
}
