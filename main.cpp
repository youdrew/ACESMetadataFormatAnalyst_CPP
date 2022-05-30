#include "parseamf.h"
#include "amfanalyst.h"
#include "amfutil.h"
#include <QApplication>
#include "ctlrendertool.h"
#include <iostream>
#include "view.h"




using namespace cv;

int main(int argc, char *argv[])
{

    /// TODO:暂时关闭
    // 创建QT的应用程序对象
    QApplication AMFAnalyst(argc, argv);

    // 建一个主窗口
    amfanalyst a1;
    a1.show();

     //让应用程序入事件循环
    return AMFAnalyst.exec();


}
