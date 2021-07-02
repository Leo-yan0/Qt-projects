#ifndef LEVELCHOOSESCENE_H
#define LEVELCHOOSESCENE_H

#include <QMainWindow>
#include <QPaintEvent>
#include "playscene.h"

class LevelChooseScene : public QMainWindow
{
    Q_OBJECT
public:
    explicit LevelChooseScene(QWidget *parent = nullptr);
    //重写绘图事件
    void paintEvent(QPaintEvent*);
    //游戏场景对象指针
    PlayScene *play=NULL;
signals:void chooseSceneBack();

public slots:
};

#endif // LEVELCHOOSESCENE_H
