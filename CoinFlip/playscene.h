#ifndef PLAYSCENE_H
#define PLAYSCENE_H

#include <QMainWindow>
#include <QPaintEvent>
#include <QPainter>
#include "mycoin.h"

class PlayScene : public QMainWindow
{
    Q_OBJECT
public:
    //explicit PlayScene(QWidget *parent = nullptr);
    PlayScene(int levelIndex);

    //重写paint事件
    void  paintEvent(QPaintEvent*);

    int level_index;

    int gameArray[4][4];  //二维数组维护每个关卡的具体数据

    bool isWin; //胜利的标志

    MyCoin *btnCoin[4][4];
signals:
    void chooseSceneBack();


public slots:
};

#endif // PLAYSCENE_H
