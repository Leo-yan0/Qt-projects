#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<iostream>
#include"data_struct.h"
#include<vector>
#include "mytextedit.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    vector<int> visited;





    //深度优先遍历
    void DFS(ALGraph* G, int v);
    //广度优先遍历
    void BFS(ALGraph* G, int v);
    //邻接矩阵转邻接表
    void trans(MatGraph *g, ALGraph*& G);
    //输出邻接表
    void DispAdj(ALGraph* G);
    //输出邻接矩阵
    void DispMat(MatGraph *g);
    //普利姆最小生成树
    void Prime(MatGraph* g, int v);
    //克鲁斯卡尔最小生成树
    void Kruskal(MatGraph* g) ;
    //狄克斯特拉算法
    void Dijkstra(MatGraph* g, int v);
    //弗洛伊德算法
    void Floyd(MatGraph* g);
    //插入排序法
    void InsertSort(Edge R[], int n);
    //输出最短路径
    void Dispath(MatGraph* g, int dist[], int path[], int s[], int v);
    void Dispath(MatGraph* g,  int path[][MAXSIZE]);


    MatGraph *mat;
    ALGraph* G;
    QString dfs_res;

    signals:

private slots:
    void on_BFS_button_clicked();

    void on_List_button_clicked();

    void on_pushButton_3_clicked();

    void on_Matrix_button_clicked();

    void on_DFS_button_clicked();

    void on_Prime_button_clicked();

    void on_Kruskal_button_clicked();

    void on_Dijkstra_button_clicked();

    void on_Floyd_button_clicked();

    void on_modify_button_clicked();

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
