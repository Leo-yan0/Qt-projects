#ifndef DOMXML_H
#define DOMXML_H

#include <QDomDocument>
#include <QDomElement>
#include <QString>
#include <QStringList>

class DomXML
{
public:
    DomXML();

    static void createXML(QString filePath);
    //list.at(0)="上海大众"
    static void appendXML(QString filePath,QStringList list);

    static void WriteXML(QDomDocument &doc,QDomElement &root,QStringList &list);

    static void readXML(QString filePath,QStringList &flist,
                        QStringList &blist,
                        QStringList &plist,
                        QStringList &nlist,
                        QStringList &tlist);
};


#endif // DOMXML_H
