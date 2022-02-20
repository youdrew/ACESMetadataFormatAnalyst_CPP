#include "parseamf.h"
#include "mainwindow.h"

#include <QtDebug>
#include <QFile>
#include <QDomDocument>
#include <QApplication>
#include <iostream>
#include <QString>

ParseAMF::ParseAMF(QString Path)
{
    QDomDocument doc(Path);
    QFile file(Path);

    // 以只读方式打开    
    if (!file.open(QIODevice::ReadOnly))
        return;
    if (!doc.setContent(&file)) {
        file.close();
        return;
    }
    file.close();

    //根据标签名tagname(aces:amfInfo),来检索amf文件中的内容，将结果以list(node形式的)的形式返回
    QDomNodeList aNodeList = doc.elementsByTagName("aces:amfInfo");


    if(!aNodeList.isEmpty()){
        for (int i = 0; i < aNodeList.size(); i++)
        {
            //aNodeList.at(i)代表一个结点(相当于一对aces:amfInfo框住的部分)
            //toElement,是因为aNodeList.at(i)的类型是node的
            QDomElement first_title = aNodeList.at(i).toElement().firstChildElement("aces:file");
            qDebug() << first_title.text();
        }
    }
}


void ParseAMF::PrintAllAmfInfo(){
    std::cout << "test" << std::endl;
};
