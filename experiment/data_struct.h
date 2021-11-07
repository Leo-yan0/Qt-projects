#ifndef DATA_STRUCT_H
#define DATA_STRUCT_H

#include<iostream>
using namespace std;
#define MAXSIZE 100
typedef struct Anode {
    int adjvex;   //边的邻接点编号
    struct Anode* next;
    int weight;
}ArcNode;

typedef struct Vnode {
    ArcNode* firstarc;
    //int info;
    Vnode() {}
}Vnode;

typedef struct AGraph {
    Vnode adjlist[MAXSIZE];
    int n, e;
    AGraph(int n,int e):n(n),e(e){}
}ALGraph;

typedef struct Point{
    int no;  //顶点编号
    int info; //顶点数据
    Point() {}
    Point(int no, int info) :no(no), info(info) {}
}VertexType;

typedef struct Mat{
    int edges[MAXSIZE][MAXSIZE];  //邻接矩阵
    int n, e;
    VertexType vexs[MAXSIZE];
    Mat(int n):n(n){}
    Mat(int n, int e) :n(n), e(e) {}
}MatGraph;

typedef struct edge{
    int u; //边的起始顶点
    int v; //边的终止顶点
    int w; //边的权值
}Edge;

#endif // DATA_STRUCT_H
