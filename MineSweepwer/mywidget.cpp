#include "mywidget.h"
#include "ui_mywidget.h"

MyWidget::MyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyWidget)
{
    ui->setupUi(this);
    //设置窗口标题
    this->setWindowTitle("简化版扫雷");
    this->setWindowIcon(QIcon(":/img/icon.jpg"));
    this->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);
    this->setFont(QFont("微软雅黑",20));
    //初始化数据
    InitData();


    connect(this,&MyWidget::hasWin,this,&MyWidget::dowhenWin);
    connect(this,&MyWidget::haslose,this,&MyWidget::doWhenlose);
}

MyWidget::~MyWidget()
{
    delete ui;
}
//判断是否是数字
bool MyWidget::isNum(BLOCK b)
{
    return b == ONE || b == TWO || b == THREE ||
               b == FOUR || b == FIVE || b == SIX || b == SEVEN ||
            b == EIGHT;
}

//遍历点击周围的格子
void MyWidget::dfs(int i, int j)
{
    if (gameOver || youWin) return;
    if (view[i][j] != UNKNOWN) return;
    view[i][j] = data[i][j];

    for (int dx = -1; dx <= 1; ++dx) {
       for (int dy = -1; dy <= 1; ++dy) {
           if (dx == 0 && dy == 0) continue;

           int nx = i + dx, ny = j + dy;
           //判断是否越界
           if (nx >= 0 && nx < ROW_COUNT && ny >= 0 && ny < COLUMN_COUNT) {

               if (data[nx][ny] == ZERO) dfs(nx, ny);
               else if (isNum(data[nx][ny])) view[nx][ny] = data[nx][ny];
           }
       }
   }
}
//初始化map容器
void MyWidget::Initmap()
{
   auto *tiles = new QPixmap(":/img/tiles.jpg");
   //这样就可以直接通过map[ZERO]来获得0对应的格子图片;
   map.insert(ZERO, tiles->copy(0, 0, 32, 32));
   map.insert(ONE, tiles->copy(BLOCK_WIDTH * 1, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
   map.insert(TWO, tiles->copy(BLOCK_WIDTH * 2, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
   map.insert(THREE, tiles->copy(BLOCK_WIDTH * 3, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
   map.insert(FOUR, tiles->copy(BLOCK_WIDTH * 4, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
   map.insert(FIVE, tiles->copy(BLOCK_WIDTH * 5, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
   map.insert(SIX, tiles->copy(BLOCK_WIDTH * 6, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
   map.insert(SEVEN, tiles->copy(BLOCK_WIDTH * 7, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
   map.insert(EIGHT, tiles->copy(BLOCK_WIDTH * 8, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
   map.insert(BOOM, tiles->copy(BLOCK_WIDTH * 9, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
   map.insert(UNKNOWN, tiles->copy(BLOCK_WIDTH * 10, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
   map.insert(FLAG, tiles->copy(BLOCK_WIDTH * 11, 0, BLOCK_WIDTH, BLOCK_HEIGHT));
}

//初始化数据
void MyWidget::InitData()
{
    gameOver = false;
    youWin = false;
    showRect = false;
    rx = -1; ry = -1;

    Initmap();
    //初始化数据,数据都设置为空，即非炸弹，样式都设为未知
    for(int i=0;i<ROW_COUNT;i++)
        for(int j=0;j<COLUMN_COUNT;j++){
            data[i][j]=ZERO;
            view[i][j]=UNKNOWN;
        }
    //生成炸弹
    for(int i=0;i<BOOM_COUNT;i++){
        int v=QRandomGenerator::global()->generate()%(ROW_COUNT*COLUMN_COUNT);
        data[v/ROW_COUNT][v%ROW_COUNT]=BOOM;
        //qDebug()<<QString("data[%1][%2]=%3").arg(v/ROW_COUNT).arg(v%ROW_COUNT).arg(data[v/ROW_COUNT][v%ROW_COUNT]);
    }
    //统计炸弹数量
    for(int i=0;i<ROW_COUNT;i++)
        for(int j=0;j<COLUMN_COUNT;j++)
        {
            //如果本身是炸弹就直接跳过
            if(data[i][j]==BOOM)    continue;
            //周围炸弹数量
           int boomNum=0;
           for(int x=i-1;x<=i+1;x++){
               if(x<0||x>=ROW_COUNT)  continue;
               for(int y=j-1;y<=j+1;y++){
                   if(y<0||y>=COLUMN_COUNT) continue;
                   if(data[x][y]==BOOM){
                       ++boomNum;
                   }
               }
           }
           switch (boomNum) {
               case 0:data[i][j]=ZERO;break;
               case 1:data[i][j]=ONE;break;
               case 2:data[i][j]=TWO;break;
               case 3:data[i][j]=THREE;break;
               case 4:data[i][j]=FOUR;break;
               case 5:data[i][j]=FIVE;break;
               case 6:data[i][j]=SIX;break;
               case 7:data[i][j]=SEVEN;break;
               case 8:data[i][j]=EIGHT;break;
           }

        }
//    //测试数据
//    for(int i=0;i<ROW_COUNT;i++)
//        for(int j=0;j<COLUMN_COUNT;j++){
//            qDebug()<<QString("data[%1][%2]=%3").arg(i).arg(j).arg(data[i][j]);
//        }
}

//判断是否赢得游戏
bool MyWidget::Win()
{
    for(int i=0;i<ROW_COUNT;i++)
        for(int j=0;j<COLUMN_COUNT;j++)
        {
            if((view[i][j]==UNKNOWN||view[i][j]==FLAG)&&data[i][j]!=BOOM)
            {
                return false;
            }
        }
    return true;
}
//鼠标按下
void MyWidget::mousePressEvent(QMouseEvent *e)
{
    if(gameOver||youWin)    return;

    int x=e->pos().x()/BLOCK_WIDTH;
    int y=e->pos().y()/BLOCK_HEIGHT;
    if(e->button()==Qt::LeftButton&&view[x][y]==UNKNOWN)
    {
        //点到的是炸弹,游戏结束
        if(data[x][y]==BOOM)
        {
            gameOver=true;
            update();
            return ;
        }

        //周围都显示
        dfs(x,y);
        youWin=Win();
        update();

    }
    //右键
    else if(e->button()==Qt::RightButton&&(view[x][y]==UNKNOWN||view[x][y]==FLAG)){
        view[x][y]=view[x][y]==FLAG ? UNKNOWN :FLAG;
        update();
    }
    else if(e->button()==Qt::MiddleButton)
    {
        showRect=true;
        rx=x;
        ry=y;
        update();
    }


}
//鼠标释放
void MyWidget::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button()==Qt::MiddleButton)
    {
        showRect=false;
        update();
    }
    if(e->button()==Qt::LeftButton)
    {
        if(gameOver)
            emit haslose();
        if(youWin)
            emit hasWin();
    }
}
//绘图事件
void MyWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    QPainter painter(this);

    for (int i = 0; i < ROW_COUNT; ++i)
       for (int j = 0; j < COLUMN_COUNT; ++j)
           painter.drawPixmap(BLOCK_WIDTH * i, BLOCK_HEIGHT * j,
                              gameOver ? map[data[i][j]] : map[view[i][j]]);



    if(showRect)
    {
        QPen pen;
        pen.setColor(QColor(255,0,0));
        pen.setWidth(5);
        painter.setPen(pen);
        painter.drawRect((rx-1)*BLOCK_WIDTH,(ry-1)*BLOCK_HEIGHT,BLOCK_WIDTH*3,BLOCK_HEIGHT*3);

    }
}

void MyWidget::dowhenWin()
{
    if(QMessageBox::question(this,"You Win","You Win!!!是否继续游戏",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Cancel)
            ==QMessageBox::Ok){
        InitData();
        update();
    }
    else{
        this->close();
    }

}

void MyWidget::doWhenlose()
{
    if(QMessageBox::question(this,"You Lose","You Lose!!!是否重新开始游戏",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Cancel)
            ==QMessageBox::Ok){
        InitData();
        update();
    }
    else{
        this->close();
    }


}


