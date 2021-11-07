#include"domxml.h"
#include <QFile>
#include <QDomDocument>
#include <QDomProcessingInstruction>
#include <QDomElement>
#include <QDebug>
#include <QTextStream>
#include <QDate>
#include <QDateTime>
#include <QDomText>
#define cout qDebug()<<"["<<__FILE__<<":"<<__LINE__<<"]"

DomXML::DomXML()
{

}

//创建xml文件
void DomXML::createXML(QString filePath)
{
    QFile file(filePath);  //关联文件名字
    //如果存在就不创建
    if(file.exists()==true)
    {
        cout<<"文件已经存在";
        return;
    }
    //不存在才创建
    else{
        bool isOK=file.open(QIODevice::WriteOnly);
        if(isOK==true)
        {
            //创建文档对象
            QDomDocument doc;
            //创建xml头部格式
            QDomProcessingInstruction ins;
            ins=doc.createProcessingInstruction("xml","version=\'1.0\' encoding=\'utf-8\'");
            //追加头部元素
            doc.appendChild(ins);
            //创建根节点
            QDomElement root=doc.createElement("日销售清单");
            doc.appendChild(root);
            //保存
            QTextStream stream(&file);  //文本流关联文件
            doc.save(stream,4);// 四字节缩进
            file.close();


        }
        else{
            cout<<"writeOnly error";
            return;
        }

    }

}

//追加元素
void DomXML::appendXML(QString filePath, QStringList list)
{
    QFile file(filePath);
    //打开xml文件
    if(file.open(QIODevice::ReadOnly))
    {
        //file 与xml文件关联
        QDomDocument doc;
        if(doc.setContent(&file))
        {
            file.close();
            //获取根节点元素
            QDomElement root=doc.documentElement();
            //获取当前事件
            QString date= QDateTime::currentDateTime().toString("yyyy-mm-dd");
            //判断根节点下有没有子节点
            //有子节点
            if(root.hasChildNodes())
            {
                    //查找最后一个子节点  同一天的销售记录写到同一天中去
                QDomElement lastElement=root.lastChildElement();
                //有没有当天日期
                //如果有
                if(lastElement.attribute("date")==date)
                {
                    //写有效数据
                    WriteXML(doc,lastElement,list);
                }
                //如果没有
                else {
                    //创建子节点 日期节点
                    QDomElement dateEmt=doc.createElement("日期");
                    //创建date属性
                    QDomAttr dateAttr=doc.createAttribute("date");
                    //设置属性值
                    dateAttr.setNodeValue(date);
                    //节点和属性关联
                    dateEmt.setAttributeNode(dateAttr);
                    //把日期节点追加到根节点
                    root.appendChild(dateEmt);

                    //写有效数据
                    WriteXML(doc,dateEmt,list);
                }
            }
            //无子节点
            else {
                //创建子节点 日期节点
                QDomElement dateEmt=doc.createElement("日期");
                //创建date属性
                QDomAttr dateAttr=doc.createAttribute("date");
                //设置属性值
                dateAttr.setNodeValue(date);
                //节点和属性关联
                dateEmt.setAttributeNode(dateAttr);
                //把日期节点追加到根节点
                root.appendChild(dateEmt);

                //写有效数据
                WriteXML(doc,dateEmt,list);

            }
            if(file.open(QIODevice::WriteOnly))
            {
                QTextStream stream(&file);
                doc.save(stream,4);
                file.close();
            }
        }
        //关联失败
        else {
            cout<<"setContent error";
            return;
        }
    }
    //不能打开打印错误信息
    else{
        cout<<"ReadOnly error";
        return;
    }
}
void DomXML::WriteXML(QDomDocument &doc, QDomElement &root, QStringList &list)
{
    //获取当前时间
    QString timestr=QDateTime::currentDateTime().toString("hh-mm-ss");
    //创建时间节点
    QDomElement timeEmt=doc.createElement("时间");
    QDomAttr timeAttr=doc.createAttribute("time");
    timeAttr.setNodeValue(timestr);
    timeEmt.setAttributeNode(timeAttr);
    //向后追加节点
    root.appendChild(timeEmt);

    //创建节点
    QDomElement factory=doc.createElement("厂家");
    QDomElement brand=doc.createElement("品牌");
    QDomElement price=doc.createElement("报价");
    QDomElement number=doc.createElement("数量");
    QDomElement totalPrice=doc.createElement("金额");

    QDomText text=doc.createTextNode(list.at(0));
    factory.appendChild(text);

    text=doc.createTextNode(list.at(1));
    brand.appendChild(text);

    text=doc.createTextNode(list.at(2));
    price.appendChild(text);

    text=doc.createTextNode(list.at(3));
    number.appendChild(text);

    text=doc.createTextNode(list.at(4));
    totalPrice.appendChild(text);

    //追加节点
    timeEmt.appendChild(factory);
    timeEmt.appendChild(brand);
    timeEmt.appendChild(price);
    timeEmt.appendChild(number);
    timeEmt.appendChild(totalPrice);
}


void DomXML::readXML(QString filePath, QStringList &flist, QStringList &blist, QStringList &plist, QStringList &nlist, QStringList &tlist)
{
    QFile file(filePath);
    //打开xml文件
    if(file.open(QIODevice::ReadOnly))
    {
        //file 与xml文件关联
        QDomDocument doc;
        if(doc.setContent(&file))
        {
            //获取根节点
            QDomElement root=doc.documentElement();
            file.close();
            QString date=QDateTime::currentDateTime().toString("yyyy-mm-dd");
            if(root.hasChildNodes())
            {
                //获取最后一个节点
                QDomElement lastEmt=root.lastChildElement();
                if(lastEmt.attribute("date")==date)
                {
                    QDomNodeList list =lastEmt.childNodes();
                    for(int i=0;i<list.size();i++)
                    {
                        QDomNodeList sublist= list.at(0).toElement().childNodes();
                        //厂家
                        flist.append(sublist.at(0).toElement().text());
                        //品牌
                        blist.append(sublist.at(1).toElement().text());
                        //报价
                        plist.append(sublist.at(2).toElement().text());
                        nlist.append(sublist.at(3).toElement().text());
                        tlist.append(sublist.at(4).toElement().text());
                    }
                }
            }
            else {
                cout<<"没有子节点";
                return;
            }
        }
        else {
            cout<<"关联失败";
            return;
        }

    }
    else {
        cout<<"readxml error";
        return ;
    }

}
