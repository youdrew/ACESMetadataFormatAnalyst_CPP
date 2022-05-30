#include "amfutil.h"
#include <QMessageBox>
#include <iostream>


amfutil::amfutil()
{

}

amfutil::~amfutil()
{

}

int amfutil::amfutil_python(std::string module, std::string func, std::vector<int> paras)
{
    Py_Initialize();        // 开启python环境

    if(!Py_IsInitialized()){
        QMessageBox::warning(NULL, "warning", "Can't start amf-util tool.", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

        return false;   // 在这里终结整个函数
    }

    try
    {
    /*
    this copy from github/amf-util

    __author__ = "Patrick Renner"
    __copyright__ = "Copyright 2020, Pomfort GmbH"

    __license__ = "MIT"
    __maintainer__ = "Patrick Renner"
    __email__ = "opensource@pomfort.com"
    */

    PyRun_SimpleString("print('python running success！')");

    PyRun_SimpleString("import sys\n");                             // 导入python的sys库
    std::string importDir = "sys.path.append('" + this->path + "')\n";
    PyRun_SimpleString(importDir.c_str());

    PyObject* moduleName = PyUnicode_FromString(module.c_str());
    PyObject* pModule = PyImport_Import(moduleName);
    if (!pModule)
    {
        std::cout << "Import Module Failed!" << std::endl;
        return false;
    }

    PyObject* pFunc = PyObject_GetAttrString(pModule, func.c_str());    //来得到python函数的方法
    if (!pFunc || !PyCallable_Check(pFunc))
    {
        std::cout << "Get Function Failed!" << std::endl;
        return false;
    }
    }






    catch (...)
    {
        PyErr_Print();
        PyErr_Clear();
        Py_Finalize();
        return false;
    }

    Py_Finalize();
    return true;
}
