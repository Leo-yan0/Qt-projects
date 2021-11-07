#include "login.h"
#include "ui_login.h"
#include <QToolButton>
#include <QList>
#include<QString>
#include "widget.h"
#include <QMessageBox>
Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    //设置图标和标题
    this->setWindowIcon(QIcon(":/images/qq.png"));
    this->setWindowTitle("qtGroupChatSystem");

    //创建数据容器,昵称容器
    QList<QString>namelist;
    namelist<<"朽木拓哉"<<"浩星醉月"<<"Leo"<<"Chaud"<<"无人的回忆"<<"Jerry"
           <<"JoeStart"<<"Dio"<<"JOJO";
    //图标资源数据
    QStringList iconNameList;
    iconNameList << "spqy"<< "ymrl" <<"qq" <<"Cherry"<< "dr"
                  <<"jj"<<"lswh"<<"qmnn"<<"spqy";

    //创建用户数组
    QVector<QToolButton*>btnVector;

    //创建用户
    for(int i=0;i<9;i++)
    {
        QToolButton *btn= new QToolButton(this);
        //加载图标
        btn->setIcon(QPixmap(QString(":/images/%1.png").arg(iconNameList[i])));
        //设置图标大小
        btn->setIconSize(QPixmap(QString(":/images/%1.png").arg(iconNameList[i])).size());
        //设置昵称
        btn->setText(QString(namelist[i]));
        //设置为透明
        btn->setAutoRaise(true);
        //设置昵称图片显示格式
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        //放到vlayout布局中去
        ui->vLayout->addWidget(btn);
        //将九个用户按钮存入数组，方便维护和操作
        btnVector.push_back(btn);
        //判断对应的用户是否已经显示
        this->IsShow.push_back(false);
    }

    //点击用户按钮，进入聊天界面
    for(int i=0;i<btnVector.size();i++){
        connect(btnVector[i],&QToolButton::clicked,[=](){
            if(this->IsShow[i]==false){
                //点击用户头像，创建一个聊天界面
                Widget *widget=new Widget(nullptr,btnVector[i]->text());
                //设置聊天界面的图标
                widget->setWindowIcon(btnVector[i]->icon());
                //设置聊天界面的标题
                widget->setWindowTitle(btnVector[i]->text());
                //展示聊天界面
                widget->show();

                //修改IsShow标志，表示此窗口已经打开
                this->IsShow[i]=true;

                //关闭页面的时候就会把相应的isShow改成false;
                connect(widget,&Widget::closeWidget,this,[=](){
                    this->IsShow[i]=false;
                });
            }
            else {
               QMessageBox::warning(this,"警告","该聊天已打开!");
            }



        });
    }
}



Login::~Login()
{
    delete ui;
}
