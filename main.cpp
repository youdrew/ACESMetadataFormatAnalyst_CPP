#include "parseamf.h"
#include "amfanalyst.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    // 创建QT的应用程序对象
    QApplication AMFAnalyst(argc, argv);

    // 建一个主窗口
    amfanalyst a1;
    a1.show();





    ParseAMF b("/Users/youdrew/Documents/python/ACES_Metadata_Format_Analyst/PythonVersion(old)/AMF_Testfile/TESTfile.amf");

    // 让应用程序进入事件循环
    return AMFAnalyst.exec();
}
