#ifndef AMFUTIL_H
#define AMFUTIL_H

#include <Python.h>
#include <boost/python.hpp>
#include <string>
#include <QDir>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <QDir>
#include <QFileDialog>

class amfutil{
public:
    amfutil();
    ~amfutil();

    QStringList Get_CTLrender_paras(const char* AMFfile_Location, const char* CTLfiles_Location);                                                                    // 在外部主要调用的就是这个
                                                                                                                                                                     // 通过输入AMF的路径，返回QStringList的CTLrender参数列表。
                                                                                                                                                                     // 它会自动调用amfutil_python_api()读取amf
                                                                                                                                                                     // 并调用getinfo()解析amf-util生成的log，并将getinfo()的返回值转为QStringList

    int amfutil_python_api(const char* path,const char* module,const char* func, std::vector<const char*> paras);       // 调用python的函数
    std::vector<std::string> getinfo(std::string inputfilename);


    //获取私有变量
    const char* Get_pythonscript_path();
    std::string Get_PreviousSaveAMFLog();

private:
    const char* pythonscript_path="/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/amf-util";            // 储存python脚本所在的上级命令
    std::string PreviousSaveAMFLog;

};

#endif // AMFANALYST_H
