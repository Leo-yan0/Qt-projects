#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPixmap>
#include <QPaintEvent>
#include <QRandomGenerator>
#include <QMouseEvent>
#include <QMap>
#include <QtGlobal>
#include <QDebug>
#include <QIcon>
#include <QDialog>
#include <QMessageBox>
//行数列数，炸弹数，格子宽高，窗口宽高
const int ROW_COUNT = 15;
const int COLUMN_COUNT = 15;
const int BLOCK_WIDTH = 32;
const int BLOCK_HEIGHT = 32;
const int BOOM_COUNT = 30;
const int WINDOW_WIDTH = ROW_COUNT * BLOCK_WIDTH;
const int WINDOW_HEIGHT = COLUMN_COUNT * BLOCK_HEIGHT;

//格子结构体，0~8表示数字，unknown表示没有点开的格子，旗帜用flag表示，炸弹用boom表示
enum BLOCK {
    ZERO, ONE, TWO, THREE, FOUR,
    FIVE, SIX, SEVEN, EIGHT, BOOM,
    FLAG, UNKNOWN
};

namespace Ui {
class MyWidget;
}

class MyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MyWidget(QWidget *parent = nullptr);
    ~MyWidget();


    //判断格子是否是数字
    bool isNum(BLOCK b);

    //深度遍历
    void dfs(int i,int j);

    //初始化图片，把tiles.png分割，得到与0~8,旗帜，炸弹，未点击状态相匹配的pixmap;
    //这样就可以直接通过map[ZERO]来获得0对应的格子图片;
    void Initmap();

    void InitData();

    //判断是否胜利
    bool Win();

    //鼠标按下
    void mousePressEvent(QMouseEvent *e) ;

    //鼠标松开
    void mouseReleaseEvent(QMouseEvent *e);

    //重写绘图函数
    void paintEvent(QPaintEvent *e);


private:
    Ui::MyWidget *ui;

    //数据图片;
    QMap<BLOCK, QPixmap> map;
    //格子内置数据
    BLOCK data[ROW_COUNT][COLUMN_COUNT];
    //格子的样式,即使用的图片
    BLOCK view[ROW_COUNT][COLUMN_COUNT];
    bool gameOver;
    bool youWin;
    bool showRect;
    int rx;
    int ry ;

signals: void hasWin();
        void haslose();
public slots: void dowhenWin();
        void doWhenlose();
};

#endif // MYWIDGET_H
