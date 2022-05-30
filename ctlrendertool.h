#ifndef CTLRENDERTOOL_H
#define CTLRENDERTOOL_H

#include <string>
#include "parseamf.h"

// 这里面涉及到ACES管线的渲染相关function

class ctlrendertool
{
public:
    ctlrendertool();
    void usingctlrender();           // 用于测试用的重载函数
    void usingctlrender(QStringList CTLrender_paras, std::string PicLocation_input, std::string PicLocation_output);            // 用于测试用的重载函数,将ctlrender的参数,以及要输入的图片位置，输出的图片位置

    /// TODO：要删除调原来的管道，因为它要拿CTLrender命令行的路径要从parseamf中去拿，而那个值是初始化的值
    std::string usingctlrender(ParseAMF);   // 根据AMF管线里的内容输出信息，最多最多三个转换 IDT LMT ODT，同时考虑到这管道的“appiled”是否为ture

    std::string usingctlrender(std::vector<std::string>);   //[新的]使用amf-util的方式，渲染图片，不考虑渲染LMT的问题，https://github.com/pomfort/amf-util/issues/5
                                                            // 但是amf-util还有一个问题是，即使IDT是false，它也会选择渲染
    QStringList usingctlrender_forLut(ParseAMF);            // 给输出LUT使用的管线，返回的是渲染参数，渲染在外面执行





    // 获取私有变量
    std::string Get_PicLUT_Location();

private:
    std::string CTLrenderCLI_Location="/usr/local/Cellar/ctl/1.5.2_6/bin/ctlrender";
    std::string PicLUT_Location="/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/TestFile/Pic_LUT/lut_origin.tif";

};

#endif // CTLRENDERTOOL_H
