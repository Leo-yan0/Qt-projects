#include "widget.h"
#include "ui_widget.h"
#include<iostream>
#include <QPainter>
#include <QString>
#include <queue>
#include <QDebug>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //初始化图
    mat=new MatGraph(7,9);
    //初始化表
    G=new ALGraph(mat->n,mat->e);
    //给图赋初值
    for (int i = 0; i < mat->n; i++) {
        mat->vexs[i].no = i;
        for (int j = 0; j < mat->n; j++) {
            if (i == j)
                mat->edges[i][j] = 0;
            else
                mat->edges[i][j] = -1;
        }
    }
    mat->edges[0][1] = 9; mat->edges[1][0] = 9;
    mat->edges[1][2] = 4; mat->edges[2][1] = 4;
    mat->edges[1][6] = 3; mat->edges[6][1] = 3;
    mat->edges[2][3] = 2; mat->edges[3][2] = 2;
    mat->edges[3][6] = 5; mat->edges[6][3] = 5;
    mat->edges[3][4] = 6; mat->edges[4][3] = 6;
    mat->edges[4][6] = 7; mat->edges[6][4] = 7;
    mat->edges[4][5] = 8; mat->edges[5][4] = 8;
    mat->edges[5][0] = 1; mat->edges[0][5] = 1;

    //图转表
    trans(mat, G);

    visited=vector<int>(MAXSIZE,0);
    dfs_res="";

    ui->textEdit->setText("输入矩阵时，每个数据间空格隔开，行与行之间用分号给开。注意最后一行也需要加分号。");


}

Widget::~Widget()
{
    delete ui;
}


//邻接矩阵转邻接表
void Widget::trans(MatGraph *g, ALGraph*& G) {
    ArcNode* p;

    G = new ALGraph(g->n,g->e);
    for (int i = 0; i < G->n; i++) {
        G->adjlist[i].firstarc = nullptr;
    }
    for(int i=0;i<G->n;i++)
        for(int j=G->n-1;j>=0;j--)
            if (g->edges[i][j] > 0) {
                p = new ArcNode;
                p->adjvex = j;
                p->weight = g->edges[i][j];
                p->next = G->adjlist[i].firstarc;
                G->adjlist[i].firstarc = p;
            }
}
//输出邻接表
void Widget::DispAdj(ALGraph* G) {
    ArcNode* p;
    for (int i = 0; i < G->n; i++) {
        p = G->adjlist[i].firstarc;
        QString str=QString("%1").arg(i);
        while (p != nullptr) {

            str+=QString("-->%1[%2]").arg(p->adjvex).arg(p->weight);
            p = p->next;
        }
        ui->textBrowser->append(str);

    }
}

//输出矩阵
void Widget::DispMat(MatGraph *g)
{
    ui->textBrowser->append(QString("矩阵输出如下:"));

    for(int i=0;i<g->n;i++)
    {
        QString str="";
        for(int j=0;j<g->n;j++)
        {

            str.append(QString("%1").arg(g->edges[i][j]).rightJustified(5,' '));
        }
        ui->textBrowser->append(str);
    }
    ui->textBrowser->append(QString("\n"));
}

//普利姆最小生成树
void Widget::Prime(MatGraph *g, int v)
{
    int lowcost[MAXSIZE];
        int minVal=INT_MAX,k=0;
        int closest[MAXSIZE];
        for(int i=0;i<g->n;i++)
            for (int j = 0; j < g->n; j++)
            {
                if (g->edges[i][j] == -1)
                    g->edges[i][j] = INT_MAX;
            }
        for (int i = 0; i < g->n; i++) {
            lowcost[i] = g->edges[v][i];
            closest[i] = v;
        }
        for (int i = 1; i < g->n; i++) {
            minVal = INT_MAX;
            for (int j = 0; j < g->n; j++) {
                if (lowcost[j] != 0 && lowcost[j] < minVal) {
                    minVal = lowcost[j];
                    k = j;
                }
            }
            QString str=QString("边(%1,%2)权值为:%3").arg(closest[k]).arg(k).arg(minVal);
            ui->textBrowser->append(str);
            lowcost[k] = 0;
            for (int j = 0; j < g->n; j++) {
                if (lowcost[j] != 0 && g->edges[k][j] < lowcost[j]) {
                    lowcost[j] = g->edges[k][j];
                    closest[j] = k;
                }
            }
        }

        for(int i=0;i<g->n;i++)
            for (int j = 0; j < g->n; j++)
            {
                if (g->edges[i][j] == INT_MAX)
                    g->edges[i][j] = -1;
            }
}
//克鲁斯卡尔最小生成树
void Widget::Kruskal(MatGraph *g)
{
    int u1, v1, sn1, sn2, k,j;
        int vset[MAXSIZE];
        Edge E[MAXSIZE];
        k = 0;
        for(int i=0;i<g->n;i++)
            for (int j = 0; j < g->n; j++)
            {
                if (g->edges[i][j] == -1)
                    g->edges[i][j] = INT_MAX;
            }
        for(int i=0;i<g->n;i++)
            for(int j=0;j<=i;j++)
                if (g->edges[i][j] != 0 && g->edges[i][j] != INT_MAX)
                {
                    E[k].u = i;
                    E[k].v = j;
                    E[k].w = g->edges[i][j];
                    k++;
                }

        InsertSort(E, g->e);
        for (int i = 0; i < g->n; i++)
            vset[i] = i;
        k = 1;
        j = 0;

        while (k < g->n) {
            u1 = E[j].u;
            v1 = E[j].v;
            sn1 = vset[u1];
            sn2 = vset[v1];
            if (sn1 != sn2) {
                QString str=QString("(%1,%2):%3").arg(u1).arg(v1).arg(E[j].w);
                ui->textBrowser->append(str);
                k++;
                for (int i = 0; i < g->n; i++) {
                    if (vset[i] == sn2)
                        vset[i] = sn1;
                }
            }
            j++;
        }
        for(int i=0;i<g->n;i++)
            for (int j = 0; j < g->n; j++)
            {
                if (g->edges[i][j] == INT_MAX)
                    g->edges[i][j] = -1;
            }
}

//狄克斯特拉算法
void Widget::Dijkstra(MatGraph *g, int v)
{
    int dist[MAXSIZE], path[MAXSIZE];
        int s[MAXSIZE];
        int mindis, u=0;


        for (int i = 0; i < g->n; i++)
            for (int j = 0; j < g->n; j++)
            {
                if (g->edges[i][j] == -1)
                    g->edges[i][j] = INT_MAX;
            }

        for (int i = 0; i < g->n; i++) {
            dist[i] = g->edges[v][i];
            s[i] = 0;
            if (g->edges[v][i] < INT_MAX)
                path[i] = v;
            else
                path[i] = -1;
        }

        s[v] = 1; path[v] = 0;
        for (int i = 0; i < g->n - 1; i++) {
            mindis = INT_MAX;
            for(int j=0;j<g->n;j++)
                if (s[j] == 0 && dist[j] < mindis)
                {
                    u = j;
                    mindis = dist[j];
                }

            s[u] = 1;
            for (int j = 0; j < g->n; j++)
            {
                if(s[j]==0)
                    if (g->edges[u][j] < INT_MAX && dist[u] + g->edges[u][j] < dist[j])
                    {
                        dist[j] = dist[u] + g->edges[u][j];
                        path[j] = u;
                    }
            }
        }
        Dispath(g, dist, path, s, v);
        for(int i=0;i<g->n;i++)
            for (int j = 0; j < g->n; j++)
            {
                if (g->edges[i][j] == INT_MAX)
                    g->edges[i][j] = -1;
            }
}
//弗洛伊德算法
void Widget::Floyd(MatGraph *g)
{
    int path[MAXSIZE][MAXSIZE] = {0};
        for (int i = 0; i < g->n; i++)
            for (int j = 0; j < g->n; j++)
            {
                if (g->edges[i][j] == -1)
                    g->edges[i][j] = INT_MAX;
            }
        for (int i = 0; i < g->n; i++)
            for (int j = 0; j < g->n; j++)
                if (g->edges[i][j] != 0 && g->edges[i][j] < INT_MAX)
                    path[i][j] = j+1;
        for(int k=0;k<g->n;k++)
            for (int i = 0; i < g->n; i++) {
                if (k == i)
                    continue;
                for (int j = 0; j < g->n; j++)
                {
                    if (g->edges[i][k] + g->edges[k][j]>0&&g->edges[i][k] + g->edges[k][j] < g->edges[i][j])
                    {
                        g->edges[i][j] = g->edges[i][k] + g->edges[k][j];
                        path[i][j] = k+1;
                    }

                }
            }
        for(int i=0;i<g->n;i++)
            for (int j = 0; j < g->n; j++)
            {
                if (g->edges[i][j] == INT_MAX)
                    g->edges[i][j] = -1;
            }
        Dispath(g, path);
}
//插入排序法
void Widget::InsertSort(Edge R[], int n)
{
    int i, j;
        Edge temp;
        for (i = 1; i < n; i++)
        {
            temp = R[i];
            j = i - 1;
            while (j >= 0 && temp.w < R[j].w)
            {
                R[j + 1] = R[j];
                j--;

            }
            R[j + 1] = temp;
        }

}

void Widget::Dispath(MatGraph *g, int dist[], int path[], int s[], int v)
{

    int apath[MAXSIZE], d,k;
        for (int i = 0; i < g->n; i++)
        {
            QString str;
            if (s[i] == 1 && i != v)
            {
                str=QString("从顶点%1到顶点%2的路径长度为:%3\t 路径为:").arg(v).arg(i).arg(dist[i]);
                d = 0;
                apath[d] = i;
                k = path[i];
                if (k == -1)
                   str+="无路径";
                else
                {
                    while (k != v)
                    {
                        d++; apath[d] = k;
                        k = path[k];
                    }
                    d++; apath[d] = v;
                    //qDebug()<<v;
                    str+=QString("%1").arg(apath[d]) ;

                    for (int j = d - 1; j >= 0; j--)
                    {
                        str+=QString(",%1").arg(apath[j]) ;
                    }
                    ui->textBrowser->append(str);
                }
            }
        }
}

void Widget::Dispath(MatGraph *g, int path[][MAXSIZE])
{

        ui->textBrowser->append("最短距离矩阵如下:");

        for (int i = 0; i < g->n; i++)
        {
            QString str="";
            for (int j = 0; j < g->n; j++)
            {
                 str.append(QString("%1").arg(g->edges[i][j]).rightJustified(5,' '));

            }
           ui->textBrowser->append(str);
        }
        ui->textBrowser->append("\n\n");
        ui->textBrowser->append("注意1表示节点0，2表示节点1，以此类推。0意味着自己到自己" );
        ui->textBrowser->append("最短路径矩阵如下:" );
        for (int i = 0; i < g->n; i++)
        {
            QString str="";
            for (int j = 0; j < g->n; j++)
            {
                str.append(QString("%1").arg(path[i][j]).rightJustified(5,' '));
            }
           ui->textBrowser->append(str);
        }
}
//深度优先遍历
void Widget::DFS(ALGraph* G, int v) {
    ArcNode* p;
    visited[v] = 1;
    dfs_res+=QString("%1--").arg(v);
    p = G->adjlist[v].firstarc;
    while (p != nullptr) {
        if (visited[p->adjvex] == 0)
            DFS(G, p->adjvex);
        p = p->next;
    }
}
//广度优先遍历
void Widget::BFS(ALGraph* G, int v) {
    int w;
    ArcNode* p;
    queue<int>myque;
    for (int i = 0; i < G->n; i++)
        visited[i] = 0;
    dfs_res+=QString("%1").arg(v);
    visited[v] = 1;
    myque.push(v);
    while (!myque.empty()) {
        w = myque.front();
        myque.pop();
        p = G->adjlist[w].firstarc;
        while (p != nullptr) {
            if (visited[p->adjvex] == 0) {
                dfs_res+=QString("--%1").arg(p->adjvex);
                visited[p->adjvex] = 1;
                myque.push(p->adjvex);
            }
            p = p->next;
        }
    }
}


//BFS按钮
void Widget::on_BFS_button_clicked()
{
    ui->textBrowser->append("广度优先遍历结果如下:");
    BFS(G,0);
    ui->textBrowser->append(dfs_res);
    ui->textBrowser->append(QString("\n"));
    dfs_res="";
}

//输出邻接表按钮
void Widget::on_List_button_clicked()
{
    ui->textBrowser->append(QString("邻接表输出如下:"));
    DispAdj(G);
    ui->textBrowser->append(QString("\n"));
}

//清除按钮
void Widget::on_pushButton_3_clicked()
{
    ui->textBrowser->clear();
}

//输出矩阵按钮
void Widget::on_Matrix_button_clicked()
{
    DispMat(mat);
}

//DFS按钮
void Widget::on_DFS_button_clicked()
{
    visited=vector<int>(MAXSIZE,0);
    ui->textBrowser->append("深度优先遍历结果如下:");
    DFS(G,0);
    ui->textBrowser->append(dfs_res);
    ui->textBrowser->append(QString("\n"));
    dfs_res="";
}
//Prime按钮
void Widget::on_Prime_button_clicked()
{
    ui->textBrowser->append(QString("普利姆最小生成树结果如下:"));
    Prime(mat,0);
     ui->textBrowser->append(QString("\n"));
}
//Kruskal按钮
void Widget::on_Kruskal_button_clicked()
{
    ui->textBrowser->append(QString("克鲁斯卡尔最小生成树结果如下:"));
    Kruskal(mat);
    ui->textBrowser->append(QString("\n"));

}
//Dijkstra按钮
void Widget::on_Dijkstra_button_clicked()
{
    ui->textBrowser->append(QString("狄克斯特拉算法结果如下:"));
    Dijkstra(mat,0);
    ui->textBrowser->append("\n");

}

void Widget::on_Floyd_button_clicked()
{
    ui->textBrowser->append("弗洛伊德算法结果如下:");
    Floyd(mat);
    ui->textBrowser->append("\n\n");
}

void Widget::on_modify_button_clicked()
{
    //获取写入数据，按协议分割数据，修改mat,
    QString data=ui->textEdit->toPlainText();
    int flag=0;
    std::vector<std::vector<int>>matrix;
    std::vector<int>temp;
    for(int i=0;i<data.size();i++){
        int j=i;
        while(data[i].isNumber()||data[i]=="-")
        {
            i++;
            flag=1;
        }
        if(flag){
            temp.push_back(data.mid(j,i-j).toInt());
            flag=0;
        }

        if(data[i]==';'){
            matrix.push_back(temp);
            temp=std::vector<int>();
        }


    }

    mat->n=matrix.size();
    for(int  i=0;i<matrix.size();i++)
        for(int j=0;j<matrix.size();j++)
        {
            mat->edges[i][j]=matrix[i][j];
        }
    trans(mat,G);
    //清除输出窗口，输入窗口，并且将光标固定在输入窗口
    ui->textEdit->clear();
}

