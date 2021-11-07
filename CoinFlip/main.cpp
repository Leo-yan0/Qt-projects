#include "mainscene.h"
#include <QApplication>
//#include "dataconfig.h"
//#include <QDebug>

//使用NISI 编译 安装向导脚本时， 一直报错 invalid icon file Erro......一大堆
//经过测试发现确实时.ico文件的问题，不能使用那种将.jpg后缀名改成.ico的图标文件 会报错
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainScene w;
    w.show();

//    dataConfig data;
//    //测试关卡数据
//    for(int i=0;i<4;i++)
//    {
//        for(int j=0;j<4;j++)
//        {
//            qDebug()<<data.mData[1][i][j];
//        }
//        qDebug()<<"";
//    }

    return a.exec();
}
