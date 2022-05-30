#define BOOST_PYTHON_STATIC_LIB
#include "amfutil.h"
#include <QMessageBox>
#include <iostream>
#include <string>
#include <iostream>
#include "ctlrendertool.h"
#include <QProcess>
#include <streambuf>
#include <cstring>
#include <fstream>
#include <sstream>
#include <boost/regex.hpp>

amfutil::amfutil()
{
}

amfutil::~amfutil()
{
}

int amfutil::amfutil_python_api(const char* path,const char* module,const char* func, std::vector<const char*> paras)
{
    QDir dir;                                                                                                                           // 新建Log目录在运行的目录里面
    QDir dirlog = dir.currentPath()+"/log";
    if(!dirlog.exists())
    {
        dir.mkdir("Log");
    }
    std::string LogFileName = "Log/Amf-Util_Log__"+QDateTime::currentDateTime().toString("yyyy_MMdd_hhmm").toStdString()+".log";

//__________________________________________________________________        // 截取Log

    int fd;
    fpos_t pos;

    fflush(stdout);
    fgetpos(stdout, &pos);
    fd = dup(fileno(stdout));
    freopen((char*)LogFileName.data(), "w", stdout);

//__________________________________________________________________

    Py_Initialize();// 开启python环境

    if(!Py_IsInitialized()){                                                                                                            //如果Python调用失败，弹出失败对话框，在这里终结调用。
        return false;   // 在这里终结整个函数
    }

    try
    {
        PyRun_SimpleString("print('python running amfutil_python_api success!')");

        PyRun_SimpleString("import sys\n");                                              // 导入python的sys库
        std::string Currentpath = path;
        std::string importDir = "sys.path.append('" + Currentpath + "')\n";  // 导入python脚本的运行路径
        std::cout << importDir << std::endl;

        PyRun_SimpleString(importDir.c_str());                                           // 把Python脚本所在的目录导入到系统路径下
        PyObject* moduleName = PyUnicode_FromString(module);                            // 得到python函数的模块
        PyObject* pModule = PyImport_Import(moduleName);                                // 如果得不到，报错
        if (!pModule)
        {
            std::cout << "Import Module Failed!" << std::endl;
            return false;
        }

        PyObject* pFunc = PyObject_GetAttrString(pModule, func);                        //得到python函数的方法
        if (!pFunc || !PyCallable_Check(pFunc))                                         // 如果得不到，报错
        {
            std::cout << "Get Function Failed!" << std::endl;
            return false;
        }


        PyObject* args = PyTuple_New((int)paras.size());                                // args储存参数

        for (int i = 0; i < (int)paras.size(); i++)
        {
            PyObject* eachparas = PyUnicode_FromString(paras[i]);
            PyTuple_SetItem(args, i, eachparas);

            std::cout << eachparas << std::endl;
        }


       PyObject* pResult = nullptr;                                                    // pResult储存返回值
       pResult = PyObject_CallObject(pFunc, args);

       if (!pResult)
       {
           std::cout << "Get Result of Function Failed!" << std::endl;
           return false;
       }
       std::cout << "Result = " << PyLong_AsLong(pResult) << " by python api" << std::endl;


    }
    catch (...)
    {
        PyErr_Print();
        PyErr_Clear();
        Py_Finalize();
        return false;
    }


//__________________________________________________________________        // 关闭截取Log

    fflush(stdout);
    dup2(fd, fileno(stdout));
    close(fd);
    clearerr(stdout);
    fsetpos(stdout, &pos);        /* for C9X */

//__________________________________________________________________

    std::cout << LogFileName;

    this->PreviousSaveAMFLog = LogFileName;                                                // 导出完成之后，将刚刚导出的文件的地址储存到私有变量PreviousSaveAMFLog里面

    Py_Finalize();// 关闭python环境
    return true;
}

QStringList amfutil::Get_CTLrender_paras(const char* AMFfile_Location, const char* CTLfiles_Location)
{
    QStringList CTLrender_paras;
    QDir dir;


    std::vector<const char*> paras1 = {AMFfile_Location,CTLfiles_Location};

        // 验证
    const char* a = AMFfile_Location;
    const char* b =CTLfiles_Location;

    std::cout << a << "//" << b << std::endl;



    this->amfutil_python_api(this->Get_pythonscript_path(),"renderPro","render", paras1);


    std::vector<std::string> ret;
    ret = this->getinfo(dir.currentPath().toStdString()+"/"+this->Get_PreviousSaveAMFLog());

    // 验证
    std::string Log_FIle_Location = dir.currentPath().toStdString()+"/"+this->Get_PreviousSaveAMFLog();
    std::cout << "amfutil调用Get_CTLrender_paras所使用的Log文件位置： " << Log_FIle_Location << std::endl;

    for (std::vector<std::string>::const_iterator i = ret.begin(); i != ret.end(); ++i) {
        QString a=QString::fromStdString(*i);
        std::cout << *i << std::endl;
        CTLrender_paras.push_back(a);
    }

    return CTLrender_paras;
}

std::vector<std::string> amfutil::getinfo(std::string inputfilename)            // 从LOG文件中提取信息
{
    std::vector<std::string> retvector;
        std::string phone = "( +)(-.+)( .+)$";
        boost::regex r(phone);
        int key = -1;
        std::ifstream inFile;
        std::string line;
        std::string sum;
        inFile.open(inputfilename.c_str());
        if (!inFile) {
            std::cout << "Unable to open file";
            exit(1); // terminate with error
        }
        std::string outline;
        while (getline(inFile, line)) {

            if (!line.compare("     \"$INPUTIMAGEPATH\" \\"))
                key--;

            if (key > -1)
            {
                outline = regex_replace(line, r, "$2");
                retvector.emplace_back(outline);
            }

            if (!line.compare("$CTLRENDER \\"))
                key++;

        }

        return retvector;
}


// 获取私有变量的函数
const char* amfutil::Get_pythonscript_path(){return this->pythonscript_path;}
std::string amfutil::Get_PreviousSaveAMFLog(){return this->PreviousSaveAMFLog;}

















