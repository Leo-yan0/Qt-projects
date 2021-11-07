#include "mytextedit.h"

myTextEdit::myTextEdit(QWidget *parent):QTextEdit (parent)
{
    flag=0;

}


//鼠标进入事件
void myTextEdit::enterEvent(QEvent *event)
{
    if(!this->flag)
        this->clear();
}



  //鼠标离开
void myTextEdit::leaveEvent(QEvent *event)
{
    if(this->toPlainText()==""){
        this->setText("输入矩阵时，每个数据间空格隔开，行与行之间用分号给开。注意最后一行也要加分号");
        this->flag=0;
    }
    else
        this->flag=1;
}
