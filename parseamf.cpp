#include "parseamf.h"
#include "amfanalyst.h"
#include "ui_amfanalyst.h"
#include <QtDebug>
#include <QFile>
#include <QDomDocument>
#include <QApplication>
#include <iostream>
#include <QString>
#include <string>
#include <QByteArray>
#include <regex>
#include <dirent.h>
#include <fstream>
#include "QCryptographicHash"
#include "QPainter"
#include "QFile"
#include "QFileDialog"
#include <vector>

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





    // Root
    QDomElement Root_element = doc.documentElement();
    if (Root_element.hasAttributes()){
        this->xmlns_aces=(Root_element.attributeNode("xmlns:aces").value()).toStdString();
        this->xmlns_cdl=(Root_element.attributeNode("xmlns:cdl").value()).toStdString();
        this->xmlns_xsi=(Root_element.attributeNode("xmlns:xsi").value()).toStdString();
        this->xsischema_Location=(Root_element.attributeNode("xsi:schemaLocation").value()).toStdString();
        this->acesMetadataFile_Version=(Root_element.attributeNode("version").value()).toStdString();
    }


    // amfInfo
    QDomNodeList amfInfo_NodeList = doc.elementsByTagName("aces:amfInfo");
    if(!amfInfo_NodeList.isEmpty()){
        for (int i = 0; i < amfInfo_NodeList.size(); i++)
        {
            QDomElement description = amfInfo_NodeList.at(i).toElement().firstChildElement("aces:description");
            this->description=(description.text()).toStdString();
        }

        for (int i = 0; i < amfInfo_NodeList.size(); i++)
        {
            QDomElement aces_author = amfInfo_NodeList.at(i).toElement().firstChildElement("aces:author");
            if (aces_author.hasChildNodes()){
                QDomElement author_name = aces_author.firstChildElement("aces:name");
                QDomElement author_emailAdress = aces_author.firstChildElement("aces:emailAddress");
                this->author_name=(author_name.text()).toStdString();
                this->author_emailAdress=(author_emailAdress.text()).toStdString();}
        }

        for (int i = 0; i < amfInfo_NodeList.size(); i++)
        {
            QDomElement aces_author = amfInfo_NodeList.at(i).toElement().firstChildElement("aces:dateTime");
            if (aces_author.hasChildNodes()){
                QDomElement dateTime_creationDateTime = aces_author.firstChildElement("aces:creationDateTime");
                QDomElement dateTime_modificationDateTime = aces_author.firstChildElement("aces:modificationDateTime");
                this->dateTime_creationDateTime=(dateTime_creationDateTime.text()).toStdString();
                this->dateTime_modificationDateTime=(dateTime_modificationDateTime.text()).toStdString();}
        }

        for (int i = 0; i < amfInfo_NodeList.size(); i++)
        {
            QDomElement uuid = amfInfo_NodeList.at(i).toElement().firstChildElement("aces:uuid");
            this->uuid=(uuid.text()).toStdString();
        }

    }


    // clipId
    QDomNodeList clipId_NodeList = doc.elementsByTagName("aces:clipId");
    if(!clipId_NodeList.isEmpty()){
        for (int i = 0; i < clipId_NodeList.size(); i++)
        {
            QDomElement ClipID_description = clipId_NodeList.at(i).toElement().firstChildElement("aces:description");
            this->ClipID_description=(ClipID_description.text()).toStdString();
        }

        for (int i = 0; i < clipId_NodeList.size(); i++)
        {
            QDomElement ClipID_cilpname = clipId_NodeList.at(i).toElement().firstChildElement("aces:clipName");
            this->ClipID_cilpname=(ClipID_cilpname.text()).toStdString();
        }

        for (int i = 0; i < clipId_NodeList.size(); i++)
        {
            QDomElement ClipID_sequence = clipId_NodeList.at(i).toElement().firstChildElement("aces:sequence");
            this->ClipID_sequence=(ClipID_sequence.text()).toStdString();
            if (ClipID_sequence.hasAttributes()){
                this->ClipID_sequence_idx=(ClipID_sequence.attributeNode("idx").value()).toStdString();
                this->ClipID_sequence_min=(ClipID_sequence.attributeNode("min").value()).toInt();
                this->ClipID_sequence_max=(ClipID_sequence.attributeNode("max").value()).toInt();
            }
        }

        for (int i = 0; i < clipId_NodeList.size(); i++)
        {
            QDomElement ClipID_path = clipId_NodeList.at(i).toElement().firstChildElement("aces:file");
            this->ClipID_path=(ClipID_path.text()).toStdString();
        }

        for (int i = 0; i < clipId_NodeList.size(); i++)
        {
            QDomElement ClipID_uuid = clipId_NodeList.at(i).toElement().firstChildElement("aces:uuid");
            this->ClipID_uuid=(ClipID_uuid.text()).toStdString();
        }
    }

    // Pipeline
    ParsePipeline(doc);

    PrintAllAmfInfo();
}

void ParseAMF::RenderPic_Whenconstruct(QString AMFPath)
{
    amfutil amfutil;
    ctlrendertool ctlrendertool;

    QStringList ctlparas = amfutil.Get_CTLrender_paras(AMFPath.toStdString().c_str(),                 // AMF位置
                                                                 this->Get_CTLpath().c_str());     // CTL查询路径



    if(this->inputTransform_applied=="false"){
        ctlparas.removeFirst();
        ctlparas.removeFirst();
    }

    std::string LUT_out = "/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/GraduationTestClassify/Out/LUT_"+QDateTime::currentDateTime().toString("yyyy_MMdd_hhmmss").toStdString()+".tif";
    this->LUT_Pic_After = LUT_out;

    ctlrendertool.usingctlrender(ctlparas,this->Get_input_picpath().c_str(),              // 渲染伊萨贝拉测试图
                                  this->Get_output_picpath().c_str());

    ctlrendertool.usingctlrender(ctlparas,this->LUT_Pic_Origin.c_str(),              // 渲染LUT_Pic测试图
                                  this->LUT_Pic_After.c_str());

}

/// 废弃，不使用amf-util这条管线输出LUTpic了
//void ParseAMF::RenderPic_WhenRenderLUT(QString AMFPath, std::string LUTSize, std::string LUTpic_OUTputLocation)
//{
//    amfutil amfutil1;
//    ctlrendertool ctlrendertool1;

//    QStringList ctlparas = amfutil1.Get_CTLrender_paras(AMFPath.toStdString().c_str(),                 // AMF位置
//                                                                 "/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/TestFile/ctl");     // CTL查询路径



//    if(this->inputTransform_applied=="false"){
//        ctlparas.removeFirst();
//        ctlparas.removeFirst();
//    }

//    if (LUTSize == "32"){
//        ctlrendertool1.usingctlrender(ctlparas,this->Get_LUT_Pic_Origin().c_str(),              // 导入的图片位置
//                                      this->Get_output_picpath().c_str());
//    }
//    else{
//        std::cout << "未执行LUT图片输出,因为输入的图片大小有问题"  << std::endl;
//    }
//    std::cout << "执行了RenderPic_WhenRenderLUT()"  << std::endl;
//}

ParseAMF::ParseAMF()            // 默认的构造函数
{
}


void ParseAMF::PrintAllAmfInfo(){
    std::cout << "xmlns_aces: " << this->xmlns_aces << std::endl;
    std::cout <<"xmlns_cdl: " << this->xmlns_cdl << std::endl;
    std::cout <<"xmlns_xsi: " << this->xmlns_xsi << std::endl;
    std::cout <<"xsischema_Location: " << this->xsischema_Location << std::endl;
    std::cout <<"acesMetadataFile_Version: " << this->acesMetadataFile_Version << std::endl;

    std::cout <<"description: " << this->description << std::endl;
    std::cout <<"author_name: " << this->author_name << std::endl;
    std::cout <<"author_emailAdress: " << this->author_emailAdress << std::endl;
    std::cout <<"dateTime_creationDateTime: " << this->dateTime_creationDateTime << std::endl;
    std::cout <<"dateTime_modificationDateTime: " << this->dateTime_modificationDateTime << std::endl;
    std::cout <<"uuid: " << this->uuid << std::endl;

    std::cout <<"ClipID_description: " << this->ClipID_description << std::endl;
    std::cout <<"ClipID_cilpname: " << this->ClipID_cilpname << std::endl;
    std::cout <<"ClipID_sequence: " << this->ClipID_sequence << std::endl;
    std::cout <<"ClipID_sequence_idx: " << this->ClipID_sequence_idx << std::endl;
    std::cout <<"ClipID_sequence_min: " << this->ClipID_sequence_min << std::endl;
    std::cout <<"ClipID_sequence_max: " << this->ClipID_sequence_max << std::endl;
    std::cout <<"ClipID_path: " << this->ClipID_path << std::endl;
    std::cout <<"ClipID_uuid: " << this->ClipID_uuid << std::endl;

    std::cout <<"pipeline_description: " << this->pipeline_description << std::endl;
    std::cout <<"pipelineInfo_author_name: " << this->pipelineInfo_author_name << std::endl;
    std::cout <<"pipelineInfo_author_emailAdress: " << this->pipelineInfo_author_emailAddress << std::endl;
    std::cout <<"pipelineInfo_dateTime_creationDateTime: " << this->pipelineInfo_dateTime_creationDateTime << std::endl;
    std::cout <<"pipelineInfo_dateTime_modificationDateTime: " << this->pipelineInfo_dateTime_modificationDateTime << std::endl;
    std::cout <<"pipelineInfo_uuid: " << this->pipelineInfo_uuid << std::endl;
    std::cout <<"pipelineInfo_systemVersion_majorVersion: " << this->pipelineInfo_systemVersion_majorVersion << std::endl;
    std::cout <<"pipelineInfo_systemVersion_minorVersion: " << this->pipelineInfo_systemVersion_minorVersion << std::endl;
    std::cout <<"pipelineInfo_systemVersion_patchVersion: " << this->pipelineInfo_systemVersion_patchVersion << std::endl;

    std::cout <<"inputTransform_applied: " << this->inputTransform_applied << std::endl;
    std::cout <<"inputTransform_description: " << this->inputTransform_description << std::endl;
    std::cout <<"inputTransform_hash: " << this->inputTransform_hash << std::endl;
    std::cout <<"inputTransform_hash_algorithm: " << this->inputTransform_hash_algorithm << std::endl;
    std::cout <<"inputTransform_uuid: " << this->inputTransform_uuid << std::endl;
    std::cout <<"inputTransform_file: " << this->inputTransform_file << std::endl;
    std::cout <<"inputTransform_transformId: " << this->inputTransform_transformId << std::endl;

    std::cout <<"inverseOutputTransform_description: " << this->inverseOutputTransform_description << std::endl;
    std::cout <<"inverseOutputTransform_hash: " << this->inverseOutputTransform_hash << std::endl;
    std::cout <<"inverseOutputTransform_hash_algorithm: " << this->inverseOutputTransform_hash_algorithm << std::endl;
    std::cout <<"inverseOutputTransform_uuid: " << this->inverseOutputTransform_uuid << std::endl;
    std::cout <<"inverseOutputTransform_file: " << this->inverseOutputTransform_file << std::endl;
    std::cout <<"inverseOutputTransform_transformId: " << this->inverseOutputTransform_transformId << std::endl;

    std::cout <<"inverseOutputDeviceTransform_description: " << this->inverseOutputDeviceTransform_description << std::endl;
    std::cout <<"inverseOutputDeviceTransform_hash: " << this->inverseOutputDeviceTransform_hash << std::endl;
    std::cout <<"inverseOutputDeviceTransform_hash_algorithm: " << this->inverseOutputDeviceTransform_hash_algorithm << std::endl;
    std::cout <<"inverseOutputDeviceTransform_uuid: " << this->inverseOutputDeviceTransform_uuid << std::endl;
    std::cout <<"inverseOutputDeviceTransform_file: " << this->inverseOutputDeviceTransform_file << std::endl;
    std::cout <<"inverseOutputDeviceTransform_transformId: " << this->inverseOutputDeviceTransform_transformId << std::endl;

    std::cout <<"inverseReferenceRenderingTransform_description: " << this->inverseReferenceRenderingTransform_description << std::endl;
    std::cout <<"inverseReferenceRenderingTransform_hash: " << this->inverseReferenceRenderingTransform_hash << std::endl;
    std::cout <<"inverseReferenceRenderingTransform_hash_algorithm: " << this->inverseReferenceRenderingTransform_hash_algorithm << std::endl;
    std::cout <<"inverseReferenceRenderingTransform_uuid: " << this->inverseReferenceRenderingTransform_uuid << std::endl;
    std::cout <<"inverseReferenceRenderingTransform_file: " << this->inverseReferenceRenderingTransform_file << std::endl;
    std::cout <<"inverseReferenceRenderingTransform_transformId: " << this->inverseReferenceRenderingTransform_transformId << std::endl;

    std::cout <<"lookTransform_applied: " << this->lookTransform_applied << std::endl;
    std::cout <<"lookTransform_description: " << this->lookTransform_description << std::endl;
    std::cout <<"lookTransform_hash: " << this->lookTransform_hash << std::endl;
    std::cout <<"lookTransform_hash_algorithm: " << this->lookTransform_hash_algorithm << std::endl;
    std::cout <<"lookTransform_uuid: " << this->lookTransform_uuid << std::endl;
    std::cout <<"lookTransform_file: " << this->lookTransform_file << std::endl;
    std::cout <<"lookTransform_transformId: " << this->lookTransform_transformId << std::endl;

    std::cout <<"cdlWorkingSpace_toCdlWorkingSpace_description: " << this->cdlWorkingSpace_toCdlWorkingSpace_description << std::endl;
    std::cout <<"cdlWorkingSpace_toCdlWorkingSpace_hash: " << this->cdlWorkingSpace_toCdlWorkingSpace_hash << std::endl;
    std::cout <<"cdlWorkingSpace_toCdlWorkingSpace_hash_algorithm: " << this->cdlWorkingSpace_toCdlWorkingSpace_hash_algorithm << std::endl;
    std::cout <<"cdlWorkingSpace_toCdlWorkingSpace_uuid: " << this->cdlWorkingSpace_toCdlWorkingSpace_uuid << std::endl;
    std::cout <<"cdlWorkingSpace_toCdlWorkingSpace_file: " << this->cdlWorkingSpace_toCdlWorkingSpace_file << std::endl;
    std::cout <<"cdlWorkingSpace_toCdlWorkingSpace_transformId: " << this->cdlWorkingSpace_toCdlWorkingSpace_transformId << std::endl;

    std::cout <<"cdlWorkingSpace_fromCdlWorkingSpace_description: " << this->cdlWorkingSpace_fromCdlWorkingSpace_description << std::endl;
    std::cout <<"cdlWorkingSpace_fromCdlWorkingSpace_hash: " << this->cdlWorkingSpace_fromCdlWorkingSpace_hash << std::endl;
    std::cout <<"cdlWorkingSpace_fromCdlWorkingSpace_hash_algorithm: " << this->cdlWorkingSpace_fromCdlWorkingSpace_hash_algorithm << std::endl;
    std::cout <<"cdlWorkingSpace_fromCdlWorkingSpace_uuid: " << this->cdlWorkingSpace_fromCdlWorkingSpace_uuid << std::endl;
    std::cout <<"cdlWorkingSpace_fromCdlWorkingSpace_file: " << this->cdlWorkingSpace_fromCdlWorkingSpace_file << std::endl;
    std::cout <<"cdlWorkingSpace_fromCdlWorkingSpace_transformId: " << this->cdlWorkingSpace_fromCdlWorkingSpace_transformId << std::endl;

    std::cout <<"ASC_SOP_Slope_R: " << this->ASC_SOP_Slope_R << std::endl;
    std::cout <<"ASC_SOP_Slope_G: " << this->ASC_SOP_Slope_G << std::endl;
    std::cout <<"ASC_SOP_Slope_B: " << this->ASC_SOP_Slope_B << std::endl;
    std::cout <<"ASC_SOP_Offset_R: " << this->ASC_SOP_Offset_R << std::endl;
    std::cout <<"ASC_SOP_Offset_G: " << this->ASC_SOP_Offset_G << std::endl;
    std::cout <<"ASC_SOP_Offset_B: " << this->ASC_SOP_Offset_B << std::endl;
    std::cout <<"ASC_SOP_Power_R: " << this->ASC_SOP_Power_R << std::endl;
    std::cout <<"ASC_SOP_Power_G: " << this->ASC_SOP_Power_G << std::endl;
    std::cout <<"ASC_SOP_Power_B: " << this->ASC_SOP_Power_B << std::endl;
    std::cout <<"ASC_SAT_Saturation: " << this->ASC_SAT_Saturation << std::endl;
    std::cout <<"ColorCorrectionRef_ref: " << this->ColorCorrectionRef_ref << std::endl;
    std::cout <<"ColorCorrectionRef_Description: " << this->ColorCorrectionRef_Description << std::endl;
    std::cout <<"ColorCorrectionRef_InputDescription: " << this->ColorCorrectionRef_InputDescription << std::endl;
    std::cout <<"ColorCorrectionRef_ViewingDescription: " << this->ColorCorrectionRef_ViewingDescription << std::endl;

    std::cout <<"outputTransform_description: " << this->outputTransform_description << std::endl;
    std::cout <<"outputTransform_hash: " << this->outputTransform_hash << std::endl;
    std::cout <<"outputTransform_hash_algorithm: " << this->outputTransform_hash_algorithm << std::endl;
    std::cout <<"outputTransform_uuid: " << this->outputTransform_uuid << std::endl;
    std::cout <<"outputTransform_file: " << this->outputTransform_file << std::endl;
    std::cout <<"outputTransform_transformId: " << this->outputTransform_transformId << std::endl;

    std::cout <<"outputDeviceTransform_description: " << this->outputDeviceTransform_description << std::endl;
    std::cout <<"outputDeviceTransform_hash: " << this->outputDeviceTransform_hash << std::endl;
    std::cout <<"outputDeviceTransform_hash_algorithm: " << this->outputDeviceTransform_hash_algorithm << std::endl;
    std::cout <<"outputDeviceTransform_uuid: " << this->outputDeviceTransform_uuid << std::endl;
    std::cout <<"outputDeviceTransform_file: " << this->outputDeviceTransform_file << std::endl;
    std::cout <<"outputDeviceTransform_transformId: " << this->outputDeviceTransform_transformId << std::endl;

    std::cout <<"referenceRenderingTransform_description: " << this->referenceRenderingTransform_description << std::endl;
    std::cout <<"referenceRenderingTransform_hash: " << this->referenceRenderingTransform_hash << std::endl;
    std::cout <<"referenceRenderingTransform_hash_algorithm: " << this->referenceRenderingTransform_hash_algorithm << std::endl;
    std::cout <<"referenceRenderingTransform_uuid: " << this->referenceRenderingTransform_uuid << std::endl;
    std::cout <<"referenceRenderingTransform_file: " << this->referenceRenderingTransform_file << std::endl;
    std::cout <<"referenceRenderingTransform_transformId: " << this->referenceRenderingTransform_transformId << std::endl;


}

std::vector<std::string>  ParseAMF::split(const std::string& str,const std::string& delim) {          //将分割后的子字符串存储在vector中
    std::vector<std::string> res;
    if("" == str) return  res;

    std::string strs = str + delim;                 //*****扩展字符串以方便检索最后一个分隔出的字符串
    size_t pos;
    size_t size = strs.size();

    for (size_t i = 0; i < size; ++i) {
        pos = strs.find(delim, i);                  //pos为分隔符第一次出现的位置，从i到pos之前的字符串是分隔出来的字符串
        if( pos < size) {                           //如果查找到，如果没有查找到分隔符，pos为string::npos
            std::string s = strs.substr(i, pos - i);//*****从i开始长度为pos-i的子字符串
            res.push_back(s);                       //两个连续空格之间切割出的字符串为空字符串，这里没有判断s是否为空，所以最后的结果中有空字符的输出，
            i = pos + delim.size() - 1;
        }

    }
    return res;
}

std::string ParseAMF::GetAllFiles(std::string path, std::string nfile)
{
    std::string ret = "";
    DIR *dp;
    struct dirent *dirp;
    std::vector<std::string> filename;

    if( (dp=opendir(path.c_str()) )==nullptr )
        perror("open dir error");

    while( (dirp=readdir(dp) )!=nullptr )
    {
        std::string ctlfile_path = path +"/"+ std::string(dirp->d_name);
        if ((strcmp(dirp->d_name,".")==0)||(strcmp(dirp->d_name,"..")==0))
            continue;

        if (dirp->d_type == DT_REG)
        {
            //文件
            std::string str_file = dirp->d_name;
            if(nfile==str_file)
            {
                return ctlfile_path;
            }
        }
        else if (dirp->d_type == DT_DIR) {
            // 文件夹
            if(GetAllFiles(ctlfile_path,nfile) != "")
                return GetAllFiles(ctlfile_path,nfile);
        }
    }

    closedir(dp);

    return ret;
}

std::string ParseAMF::GetCTLpath(std::string path, std::string name)
{
    //domainPattern正则表达式
    std::regex domainPattern("([a-zA-Z0-9_\\-]+)\\.([a-zA-Z0-9_\\-]+)\\.([a-zA-Z0-9_\\-]+)\\.");

    //搜索结果
    std::string searchResult;
    std::smatch ret;

    //判断是否搜索到，结果保存到ret
    if (regex_search(name,ret,domainPattern))
    {
        //获取ctl文件名
        std::string ctlfilename = ret[0].str()+"ctl";
        //判断这个文件是不是RRT或者invoc
        if(ret[1].str()=="RRT"||ret[1].str()=="InvRRT")
        {
            ctlfilename = ret[1].str()+".ctl";

            searchResult=GetAllFiles(path,ctlfilename);
        }
        else{
            //遍历CTL文件夹，搜索ctl文件的本地路径并打印
            searchResult=GetAllFiles(path,ctlfilename);
        }
    }
    else{
        qDebug() <<  "FALSE" ;
    }

    return searchResult;
}

void ParseAMF::ParsePipeline(QDomDocument Path)
{
    //Pipeline
    QDomNodeList pipeline_NodeList = Path.elementsByTagName("aces:pipeline");
    if(!pipeline_NodeList.isEmpty()){
    //pipelineInfo
        for (int i = 0; i < pipeline_NodeList.size(); i++)
        {
            QDomElement pipelineInfo = pipeline_NodeList.at(i).toElement().firstChildElement("aces:pipelineInfo");
            if (pipelineInfo.hasChildNodes()){
                QDomElement pipeline_description = pipelineInfo.firstChildElement("aces:description");
                this->pipeline_description = (pipeline_description.text()).toStdString();
                QDomElement pipeline_author = pipelineInfo.firstChildElement("aces:author");
                if (pipeline_author.hasChildNodes()){
                    QDomElement pipelineInfo_author_name = pipeline_author.firstChildElement("aces:name");
                    QDomElement pipelineInfo_author_emailAdress = pipeline_author.firstChildElement("aces:emailAddress");
                    this->pipelineInfo_author_name=(pipelineInfo_author_name.text()).toStdString();
                    this->pipelineInfo_author_emailAddress=(pipelineInfo_author_emailAdress.text()).toStdString();
                }
                QDomElement pipelineInfo_dateTime = pipelineInfo.firstChildElement("aces:dateTime");
                if (pipelineInfo_dateTime.hasChildNodes()){
                    QDomElement pipelineInfo_dateTime_creationDateTime = pipelineInfo_dateTime.firstChildElement("aces:creationDateTime");
                    QDomElement pipelineInfo_dateTime_modificationDateTime = pipelineInfo_dateTime.firstChildElement("aces:modificationDateTime");
                    this->pipelineInfo_dateTime_creationDateTime=(pipelineInfo_dateTime_creationDateTime.text()).toStdString();
                    this->pipelineInfo_dateTime_modificationDateTime=(pipelineInfo_dateTime_modificationDateTime.text()).toStdString();
                }
                QDomElement pipelineInfo_uuid = pipelineInfo.firstChildElement("aces:uuid");
                this->pipelineInfo_uuid = (pipelineInfo_uuid.text()).toStdString();
                QDomElement pipelineInfo_systemVersion = pipelineInfo.firstChildElement("aces:systemVersion");
                if (pipelineInfo_systemVersion.hasChildNodes()){
                    QDomElement pipelineInfo_systemVersion_majorVersion = pipelineInfo_systemVersion.firstChildElement("aces:majorVersion");
                    this->pipelineInfo_systemVersion_majorVersion=(pipelineInfo_systemVersion_majorVersion.text()).toInt();
                    QDomElement pipelineInfo_systemVersion_minorVersion = pipelineInfo_systemVersion.firstChildElement("aces:minorVersion");
                    this->pipelineInfo_systemVersion_minorVersion=(pipelineInfo_systemVersion_minorVersion.text()).toInt();
                    QDomElement pipelineInfo_systemVersion_patchVersion = pipelineInfo_systemVersion.firstChildElement("aces:patchVersion");
                    this->pipelineInfo_systemVersion_patchVersion=(pipelineInfo_systemVersion_patchVersion.text()).toInt();
                }
            }
        }
        //inputTransform
        for (int i = 0; i < pipeline_NodeList.size(); i++)
        {
            QDomElement pipeline_inputTransform = pipeline_NodeList.at(i).toElement().firstChildElement("aces:inputTransform");
            if (pipeline_inputTransform.hasAttribute("applied")){
                this->inputTransform_applied=(pipeline_inputTransform.attributeNode("applied").value()).toStdString();
            }
            if (pipeline_inputTransform.hasChildNodes()){
                QDomElement inputTransform_description = pipeline_inputTransform.firstChildElement("aces:description");
                QDomElement inputTransform_hash = pipeline_inputTransform.firstChildElement("aces:hash");
                QDomElement inputTransform_uuid = pipeline_inputTransform.firstChildElement("aces:uuid");
                QDomElement inputTransform_file = pipeline_inputTransform.firstChildElement("aces:file");
                QDomElement inputTransform_transformId = pipeline_inputTransform.firstChildElement("aces:transformId");
                this->inputTransform_description=(inputTransform_description.text()).toStdString();
                this->inputTransform_hash=(inputTransform_hash.text()).toStdString();
                if (inputTransform_hash.hasAttribute("algorithm")){
                    this->inputTransform_hash_algorithm=(inputTransform_hash.attributeNode("algorithm").value()).toStdString();
                }
                this->inputTransform_uuid=(inputTransform_uuid.text()).toStdString();
                this->inputTransform_file=(inputTransform_file.text()).toStdString();
                this->inputTransform_transformId=(inputTransform_transformId.text()).toStdString();

                // inputTransform下面三个反向转换
                QDomElement pipeline_inverseOutputTransform = pipeline_inputTransform.firstChildElement("aces:inverseOutputTransform");
                QDomElement pipeline_inverse_inverseOutputDeviceTransform = pipeline_inputTransform.firstChildElement("aces:inverseOutputDeviceTransform");
                QDomElement pipeline_inverse_inverseReferenceRenderingTransform = pipeline_inputTransform.firstChildElement("aces:inverseReferenceRenderingTransform");

                if (pipeline_inverseOutputTransform.hasChildNodes()){
                    QDomElement inverseOutputTransform_description = pipeline_inverseOutputTransform.firstChildElement("aces:description");
                    QDomElement inverseOutputTransform_hash = pipeline_inverseOutputTransform.firstChildElement("aces:hash");
                    QDomElement inverseOutputTransform_uuid = pipeline_inverseOutputTransform.firstChildElement("aces:uuid");
                    QDomElement inverseOutputTransform_file = pipeline_inverseOutputTransform.firstChildElement("aces:file");
                    QDomElement inverseOutputTransform_transformId = pipeline_inverseOutputTransform.firstChildElement("aces:transformId");
                    this->inverseOutputTransform_description=(inverseOutputTransform_description.text()).toStdString();
                    this->inverseOutputTransform_hash=(inverseOutputTransform_hash.text()).toStdString();
                    if (inverseOutputTransform_hash.hasAttribute("algorithm")){
                        this->inverseOutputTransform_hash_algorithm=(inverseOutputTransform_hash.attributeNode("algorithm").value()).toStdString();
                    }
                    this->inverseOutputTransform_uuid=(inverseOutputTransform_uuid.text()).toStdString();
                    this->inverseOutputTransform_file=(inverseOutputTransform_file.text()).toStdString();
                    this->inverseOutputTransform_transformId=(inverseOutputTransform_transformId.text()).toStdString();
                }
                if (pipeline_inverse_inverseOutputDeviceTransform.hasChildNodes()){
                    QDomElement inverseOutputDeviceTransform_description = pipeline_inverse_inverseOutputDeviceTransform.firstChildElement("aces:description");
                    QDomElement inverseOutputDeviceTransform_hash = pipeline_inverse_inverseOutputDeviceTransform.firstChildElement("aces:hash");
                    QDomElement inverseOutputDeviceTransform_uuid = pipeline_inverse_inverseOutputDeviceTransform.firstChildElement("aces:uuid");
                    QDomElement inverseOutputDeviceTransform_file = pipeline_inverse_inverseOutputDeviceTransform.firstChildElement("aces:file");
                    QDomElement inverseOutputDeviceTransform_transformId = pipeline_inverse_inverseOutputDeviceTransform.firstChildElement("aces:transformId");
                    this->inverseOutputDeviceTransform_description=(inverseOutputDeviceTransform_description.text()).toStdString();
                    this->inverseOutputDeviceTransform_hash=(inverseOutputDeviceTransform_hash.text()).toStdString();
                    if (inverseOutputDeviceTransform_hash.hasAttribute("algorithm")){
                        this->inverseOutputDeviceTransform_hash_algorithm=(inverseOutputDeviceTransform_hash.attributeNode("algorithm").value()).toStdString();
                    }
                    this->inverseOutputDeviceTransform_uuid=(inverseOutputDeviceTransform_uuid.text()).toStdString();
                    this->inverseOutputDeviceTransform_file=(inverseOutputDeviceTransform_file.text()).toStdString();
                    this->inverseOutputDeviceTransform_transformId=(inverseOutputDeviceTransform_transformId.text()).toStdString();
                }
                if (pipeline_inverse_inverseReferenceRenderingTransform.hasChildNodes()){
                    QDomElement inverseReferenceRenderingTransform_description = pipeline_inverse_inverseReferenceRenderingTransform.firstChildElement("aces:description");
                    QDomElement inverseReferenceRenderingTransform_hash = pipeline_inverse_inverseReferenceRenderingTransform.firstChildElement("aces:hash");
                    QDomElement inverseReferenceRenderingTransform_uuid = pipeline_inverse_inverseReferenceRenderingTransform.firstChildElement("aces:uuid");
                    QDomElement inverseReferenceRenderingTransform_file = pipeline_inverse_inverseReferenceRenderingTransform.firstChildElement("aces:file");
                    QDomElement inverseReferenceRenderingTransform_transformId = pipeline_inverse_inverseReferenceRenderingTransform.firstChildElement("aces:transformId");
                    this->inverseReferenceRenderingTransform_description=(inverseReferenceRenderingTransform_description.text()).toStdString();
                    this->inverseReferenceRenderingTransform_hash=(inverseReferenceRenderingTransform_hash.text()).toStdString();
                    if (inverseReferenceRenderingTransform_hash.hasAttribute("algorithm")){
                        this->inverseReferenceRenderingTransform_hash_algorithm=(inverseReferenceRenderingTransform_hash.attributeNode("algorithm").value()).toStdString();
                    }
                    this->inverseReferenceRenderingTransform_uuid=(inverseReferenceRenderingTransform_uuid.text()).toStdString();
                    this->inverseReferenceRenderingTransform_file=(inverseReferenceRenderingTransform_file.text()).toStdString();
                    this->inverseReferenceRenderingTransform_transformId=(inverseReferenceRenderingTransform_transformId.text()).toStdString();
                }
            }
        }
        // lookTransform
        for (int i = 0; i < pipeline_NodeList.size(); i++)
        {
            QDomElement pipeline_lookTransform = pipeline_NodeList.at(i).toElement().firstChildElement("aces:lookTransform");
            if (pipeline_lookTransform.hasAttribute("applied")){
                this->lookTransform_applied=(pipeline_lookTransform.attributeNode("applied").value()).toStdString();
            }
            if (pipeline_lookTransform.hasChildNodes()){
                QDomElement lookTransform_description = pipeline_lookTransform.firstChildElement("aces:description");
                QDomElement lookTransform_hash = pipeline_lookTransform.firstChildElement("aces:hash");
                QDomElement lookTransform_uuid = pipeline_lookTransform.firstChildElement("aces:uuid");
                QDomElement lookTransform_file = pipeline_lookTransform.firstChildElement("aces:file");
                QDomElement lookTransform_transformId = pipeline_lookTransform.firstChildElement("aces:transformId");
                this->lookTransform_description=(lookTransform_description.text()).toStdString();
                this->lookTransform_hash=(lookTransform_hash.text()).toStdString();
                if (lookTransform_hash.hasAttribute("algorithm")){
                    this->lookTransform_hash_algorithm=(lookTransform_hash.attributeNode("algorithm").value()).toStdString();
                }
                this->lookTransform_uuid=(lookTransform_uuid.text()).toStdString();
                this->lookTransform_file=(lookTransform_file.text()).toStdString();
                this->lookTransform_transformId=(lookTransform_transformId.text()).toStdString();
            }
            QDomElement pipeline_lookTransform_cdlWorkingSpace = pipeline_lookTransform.firstChildElement("aces:cdlWorkingSpace");
            if (pipeline_lookTransform_cdlWorkingSpace.hasChildNodes()){

                QDomElement pipeline_lookTransform_cdlWorkingSpace_toCdlWorkingSpace = pipeline_lookTransform_cdlWorkingSpace.firstChildElement("aces:toCdlWorkingSpace");
                if (pipeline_lookTransform_cdlWorkingSpace_toCdlWorkingSpace.hasChildNodes()){
                    QDomElement cdlWorkingSpace_toCdlWorkingSpace_description = pipeline_lookTransform_cdlWorkingSpace_toCdlWorkingSpace.firstChildElement("aces:description");
                    QDomElement cdlWorkingSpace_toCdlWorkingSpace_hash = pipeline_lookTransform_cdlWorkingSpace_toCdlWorkingSpace.firstChildElement("aces:hash");
                    QDomElement cdlWorkingSpace_toCdlWorkingSpace_uuid = pipeline_lookTransform_cdlWorkingSpace_toCdlWorkingSpace.firstChildElement("aces:uuid");
                    QDomElement cdlWorkingSpace_toCdlWorkingSpace_file = pipeline_lookTransform_cdlWorkingSpace_toCdlWorkingSpace.firstChildElement("aces:file");
                    QDomElement cdlWorkingSpace_toCdlWorkingSpace_transformId = pipeline_lookTransform_cdlWorkingSpace_toCdlWorkingSpace.firstChildElement("aces:transformId");
                    this->cdlWorkingSpace_toCdlWorkingSpace_description=(cdlWorkingSpace_toCdlWorkingSpace_description.text()).toStdString();
                    this->cdlWorkingSpace_toCdlWorkingSpace_hash=(cdlWorkingSpace_toCdlWorkingSpace_hash.text()).toStdString();
                    if (cdlWorkingSpace_toCdlWorkingSpace_hash.hasAttribute("algorithm")){
                        this->cdlWorkingSpace_toCdlWorkingSpace_hash_algorithm=(cdlWorkingSpace_toCdlWorkingSpace_hash.attributeNode("algorithm").value()).toStdString();
                    }
                    this->cdlWorkingSpace_toCdlWorkingSpace_uuid=(cdlWorkingSpace_toCdlWorkingSpace_uuid.text()).toStdString();
                    this->cdlWorkingSpace_toCdlWorkingSpace_file=(cdlWorkingSpace_toCdlWorkingSpace_file.text()).toStdString();
                    this->cdlWorkingSpace_toCdlWorkingSpace_transformId=(cdlWorkingSpace_toCdlWorkingSpace_transformId.text()).toStdString();
                }
                QDomElement pipeline_lookTransform_cdlWorkingSpace_fromCdlWorkingSpace = pipeline_lookTransform_cdlWorkingSpace.firstChildElement("aces:toCdlWorkingSpace");
                if (pipeline_lookTransform_cdlWorkingSpace_fromCdlWorkingSpace.hasChildNodes()){
                    QDomElement cdlWorkingSpace_fromCdlWorkingSpace_description = pipeline_lookTransform_cdlWorkingSpace_fromCdlWorkingSpace.firstChildElement("aces:description");
                    QDomElement cdlWorkingSpace_fromCdlWorkingSpace_hash = pipeline_lookTransform_cdlWorkingSpace_fromCdlWorkingSpace.firstChildElement("aces:hash");
                    QDomElement cdlWorkingSpace_fromCdlWorkingSpace_uuid = pipeline_lookTransform_cdlWorkingSpace_fromCdlWorkingSpace.firstChildElement("aces:uuid");
                    QDomElement cdlWorkingSpace_fromCdlWorkingSpace_file = pipeline_lookTransform_cdlWorkingSpace_fromCdlWorkingSpace.firstChildElement("aces:file");
                    QDomElement cdlWorkingSpace_fromCdlWorkingSpace_transformId = pipeline_lookTransform_cdlWorkingSpace_fromCdlWorkingSpace.firstChildElement("aces:transformId");
                    this->cdlWorkingSpace_fromCdlWorkingSpace_description=(cdlWorkingSpace_fromCdlWorkingSpace_description.text()).toStdString();
                    this->cdlWorkingSpace_fromCdlWorkingSpace_hash=(cdlWorkingSpace_fromCdlWorkingSpace_hash.text()).toStdString();
                    if (cdlWorkingSpace_fromCdlWorkingSpace_hash.hasAttribute("algorithm")){
                        this->cdlWorkingSpace_fromCdlWorkingSpace_hash_algorithm=(cdlWorkingSpace_fromCdlWorkingSpace_hash.attributeNode("algorithm").value()).toStdString();
                    }
                    this->cdlWorkingSpace_fromCdlWorkingSpace_uuid=(cdlWorkingSpace_fromCdlWorkingSpace_uuid.text()).toStdString();
                    this->cdlWorkingSpace_fromCdlWorkingSpace_file=(cdlWorkingSpace_fromCdlWorkingSpace_file.text()).toStdString();
                    this->cdlWorkingSpace_fromCdlWorkingSpace_transformId=(cdlWorkingSpace_fromCdlWorkingSpace_transformId.text()).toStdString();
                }
            }
            QDomElement cdl_SOPNode = pipeline_lookTransform.firstChildElement("cdl:SOPNode");
            if (cdl_SOPNode.hasChildNodes()){
                QDomElement cdl_Slope = cdl_SOPNode.firstChildElement("cdl:Slope");
                QDomElement cdl_Offset = cdl_SOPNode.firstChildElement("cdl:Offset");
                QDomElement cdl_Power = cdl_SOPNode.firstChildElement("cdl:Power");
                std::vector vector_cdl_Slope = split((cdl_Slope.text()).toStdString()," ");
                this->ASC_SOP_Slope_R = atof(vector_cdl_Slope[0].c_str());
                this->ASC_SOP_Slope_G = atof(vector_cdl_Slope[1].c_str());
                this->ASC_SOP_Slope_B = atof(vector_cdl_Slope[2].c_str());
                std::vector vector_cdl_Offset = split((cdl_Offset.text()).toStdString()," ");
                this->ASC_SOP_Offset_R = atof(vector_cdl_Offset[0].c_str());
                this->ASC_SOP_Offset_G = atof(vector_cdl_Offset[1].c_str());
                this->ASC_SOP_Offset_B = atof(vector_cdl_Offset[2].c_str());
                std::vector vector_cdl_Power = split((cdl_Power.text()).toStdString()," ");
                this->ASC_SOP_Power_R = atof(vector_cdl_Power[0].c_str());
                this->ASC_SOP_Power_G = atof(vector_cdl_Power[1].c_str());
                this->ASC_SOP_Power_B = atof(vector_cdl_Power[2].c_str());
            }
            QDomElement cdl_SatNode = pipeline_lookTransform.firstChildElement("cdl:SatNode");
            if (cdl_SatNode.hasChildNodes()){
                QDomElement ASC_SAT_Saturation = cdl_SatNode.firstChildElement("cdl:Saturation");
                this->ASC_SAT_Saturation = atof((ASC_SAT_Saturation.text().toStdString()).c_str());
            }
            QDomElement ColorCorrectionRef_ref = pipeline_lookTransform.firstChildElement("cdl:ColorCorrectionRef");
            if (ColorCorrectionRef_ref.hasAttributes()){
                this->ColorCorrectionRef_ref=(ColorCorrectionRef_ref.attributeNode("ref").value()).toStdString();
            }
            if(ColorCorrectionRef_ref.hasChildNodes())
            {
            QDomElement cdl_ColorCorrectionRef_cdl_Description = ColorCorrectionRef_ref.firstChildElement("cdl:Description");
            QDomElement cdl_ColorCorrectionRef_cdl_InputDescription = ColorCorrectionRef_ref.firstChildElement("cdl:InputDescription");
            QDomElement cdl_ColorCorrectionRef_cdl_ViewingDescription = ColorCorrectionRef_ref.firstChildElement("cdl:ViewingDescription");

            this->ColorCorrectionRef_Description=(cdl_ColorCorrectionRef_cdl_Description.text()).toStdString();
            this->ColorCorrectionRef_InputDescription=(cdl_ColorCorrectionRef_cdl_InputDescription.text()).toStdString();
            this->ColorCorrectionRef_ViewingDescription=(cdl_ColorCorrectionRef_cdl_ViewingDescription.text()).toStdString();
            }
        }
        // outputTransform
        for (int i = 0; i < pipeline_NodeList.size(); i++)
        {
            QDomElement pipeline_outputTransform = pipeline_NodeList.at(i).toElement().firstChildElement("aces:outputTransform");
            if (pipeline_outputTransform.hasChildNodes()){
                QDomElement outputTransform_description = pipeline_outputTransform.firstChildElement("aces:description");
                QDomElement outputTransform_hash = pipeline_outputTransform.firstChildElement("aces:hash");
                QDomElement outputTransform_uuid = pipeline_outputTransform.firstChildElement("aces:uuid");
                QDomElement outputTransform_file = pipeline_outputTransform.firstChildElement("aces:file");
                QDomElement outputTransform_transformId = pipeline_outputTransform.firstChildElement("aces:transformId");
                this->outputTransform_description=(outputTransform_description.text()).toStdString();
                this->outputTransform_hash=(outputTransform_hash.text()).toStdString();
                if (outputTransform_hash.hasAttribute("algorithm")){
                    this->outputTransform_hash_algorithm=(outputTransform_hash.attributeNode("algorithm").value()).toStdString();
                }
                this->outputTransform_uuid=(outputTransform_uuid.text()).toStdString();
                this->outputTransform_file=(outputTransform_file.text()).toStdString();
                this->outputTransform_transformId=(outputTransform_transformId.text()).toStdString();
            }
            QDomElement pipeline_outputDeviceTransform = pipeline_outputTransform.firstChildElement("aces:outputDeviceTransform");
            QDomElement pipeline_referenceRenderingTransform = pipeline_outputTransform.firstChildElement("aces:referenceRenderingTransform");
            if (pipeline_outputDeviceTransform.hasChildNodes()){
                QDomElement outputDeviceTransform_description = pipeline_outputDeviceTransform.firstChildElement("aces:description");
                QDomElement outputDeviceTransform_hash = pipeline_outputDeviceTransform.firstChildElement("aces:hash");
                QDomElement outputDeviceTransform_uuid = pipeline_outputDeviceTransform.firstChildElement("aces:uuid");
                QDomElement outputDeviceTransform_file = pipeline_outputDeviceTransform.firstChildElement("aces:file");
                QDomElement outputDeviceTransform_transformId = pipeline_outputDeviceTransform.firstChildElement("aces:transformId");
                this->outputDeviceTransform_description=(outputDeviceTransform_description.text()).toStdString();
                this->outputDeviceTransform_hash=(outputDeviceTransform_hash.text()).toStdString();
                if (outputDeviceTransform_hash.hasAttribute("algorithm")){
                    this->outputDeviceTransform_hash_algorithm=(outputDeviceTransform_hash.attributeNode("algorithm").value()).toStdString();
                }
                this->outputDeviceTransform_uuid=(outputDeviceTransform_uuid.text()).toStdString();
                this->outputDeviceTransform_file=(outputDeviceTransform_file.text()).toStdString();
                this->outputDeviceTransform_transformId=(outputDeviceTransform_transformId.text()).toStdString();
            }
            if (pipeline_referenceRenderingTransform.hasChildNodes()){
                QDomElement referenceRenderingTransform_description = pipeline_referenceRenderingTransform.firstChildElement("aces:description");
                QDomElement referenceRenderingTransform_hash = pipeline_referenceRenderingTransform.firstChildElement("aces:hash");
                QDomElement referenceRenderingTransform_uuid = pipeline_referenceRenderingTransform.firstChildElement("aces:uuid");
                QDomElement referenceRenderingTransform_file = pipeline_referenceRenderingTransform.firstChildElement("aces:file");
                QDomElement referenceRenderingTransform_transformId = pipeline_referenceRenderingTransform.firstChildElement("aces:transformId");
                this->referenceRenderingTransform_description=(referenceRenderingTransform_description.text()).toStdString();
                this->referenceRenderingTransform_hash=(referenceRenderingTransform_hash.text()).toStdString();
                if (referenceRenderingTransform_hash.hasAttribute("algorithm")){
                    this->referenceRenderingTransform_hash_algorithm=(referenceRenderingTransform_hash.attributeNode("algorithm").value()).toStdString();
                }
                this->referenceRenderingTransform_uuid=(referenceRenderingTransform_uuid.text()).toStdString();
                this->referenceRenderingTransform_file=(referenceRenderingTransform_file.text()).toStdString();
                this->referenceRenderingTransform_transformId=(referenceRenderingTransform_transformId.text()).toStdString();
            }
        }
    }
}


std::string ParseAMF::Get_xmlns_aces(){return this->xmlns_aces;}
std::string ParseAMF::Get_xmlns_cdl(){return this->xmlns_cdl;}
std::string ParseAMF::Get_xmlns_xsi(){return this->xmlns_xsi;}
std::string ParseAMF::Get_xsischema_Location(){return this->xsischema_Location;}
std::string ParseAMF::Get_acesMetadataFile_Version(){return this->acesMetadataFile_Version;}

std::string ParseAMF::Get_description(){return this->description;}
std::string ParseAMF::Get_author_name(){return this->author_name;}
std::string ParseAMF::Get_author_emailAdress(){return this->author_emailAdress;}
std::string ParseAMF::Get_dateTime_creationDateTime(){return this->dateTime_creationDateTime;}
std::string ParseAMF::Get_dateTime_modificationDateTime(){return this->dateTime_modificationDateTime;}
std::string ParseAMF::Get_uuid(){return this->uuid;}

std::string ParseAMF::Get_ClipID_description(){return this->ClipID_description;}
std::string ParseAMF::Get_ClipID_cilpname(){return this->ClipID_cilpname;}
std::string ParseAMF::Get_ClipID_sequence(){return this->ClipID_sequence;}
std::string ParseAMF::Get_ClipID_sequence_idx(){return this->ClipID_sequence_idx;}
int ParseAMF::Get_ClipID_sequence_min(){return this->ClipID_sequence_min;}
int ParseAMF::Get_ClipID_sequence_max(){return this->ClipID_sequence_max;}
std::string ParseAMF::Get_ClipID_path(){return this->ClipID_path;}
std::string ParseAMF::Get_ClipID_uuid(){return this->ClipID_uuid;}

std::string ParseAMF::Get_pipeline_description(){return this->pipeline_description;}
std::string ParseAMF::Get_pipelineInfo_author_name(){return this->pipelineInfo_author_name;}
std::string ParseAMF::Get_pipelineInfo_author_emailAdress(){return this->pipelineInfo_author_emailAddress;}
std::string ParseAMF::Get_pipelineInfo_dateTime_creationDateTime(){return this->pipelineInfo_dateTime_creationDateTime;}
std::string ParseAMF::Get_pipelineInfo_dateTime_modificationDateTime(){return this->pipelineInfo_dateTime_modificationDateTime;}
std::string ParseAMF::Get_pipelineInfo_uuid(){return this->pipelineInfo_uuid;}
int ParseAMF::Get_pipelineInfo_systemVersion_majorVersion(){return this->pipelineInfo_systemVersion_majorVersion;}
int ParseAMF::Get_pipelineInfo_systemVersion_minorVersion(){return this->pipelineInfo_systemVersion_minorVersion;}
int ParseAMF::Get_pipelineInfo_systemVersion_patchVersion(){return this->pipelineInfo_systemVersion_patchVersion;}

bool ParseAMF::Get_inputTransform_applied(){
    if(this->inputTransform_applied=="true"){
        return true;
    }
    if(this->inputTransform_applied=="false"){
        return false;
    }
    else {
        return false;
    };
}

std::string ParseAMF::Get_inputTransform_description(){return this->inputTransform_description;}
std::string ParseAMF::Get_inputTransform_hash(){return this->inputTransform_hash;}

std::string ParseAMF::Get_inputTransform_hash_algorithm()
{
    return this->inputTransform_hash_algorithm;
}
std::string ParseAMF::Get_inputTransform_uuid(){return this->inputTransform_uuid;}
std::string ParseAMF::Get_inputTransform_file(){return this->inputTransform_file;}
std::string ParseAMF::Get_inputTransform_transformId(){return this->inputTransform_transformId;}

std::string ParseAMF::Get_inverseOutputTransform_description(){return this->inverseOutputTransform_description;}
std::string ParseAMF::Get_inverseOutputTransform_hash(){return this->inverseOutputTransform_hash;}

std::string ParseAMF::Get_inverseOutputTransform_hash_algorithm()
{
    return this->inverseOutputDeviceTransform_hash_algorithm;
}
std::string ParseAMF::Get_inverseOutputTransform_uuid(){return this->inverseOutputTransform_uuid;}
std::string ParseAMF::Get_inverseOutputTransform_file(){return this->inverseOutputTransform_file;}
std::string ParseAMF::Get_inverseOutputTransform_transformId(){return this->inverseOutputTransform_transformId;}

std::string ParseAMF::Get_inverseOutputDeviceTransform_description(){return this->inverseOutputDeviceTransform_description;}
std::string ParseAMF::Get_inverseOutputDeviceTransform_hash(){return this->inverseOutputDeviceTransform_hash;}

std::string ParseAMF::Get_inverseOutputDeviceTransform_hash_algorithm()
{
    return this->inverseOutputDeviceTransform_hash_algorithm;
}
std::string ParseAMF::Get_inverseOutputDeviceTransform_uuid(){return this->inverseOutputDeviceTransform_uuid;}
std::string ParseAMF::Get_inverseOutputDeviceTransform_file(){return this->inverseOutputDeviceTransform_file;}
std::string ParseAMF::Get_inverseOutputDeviceTransform_transformId(){return this->inverseOutputDeviceTransform_transformId;}

std::string ParseAMF::Get_inverseReferenceRenderingTransform_description(){return this->inverseReferenceRenderingTransform_description;}
std::string ParseAMF::Get_inverseReferenceRenderingTransform_hash(){return this->inverseReferenceRenderingTransform_hash;}

std::string ParseAMF::Get_inverseReferenceRenderingTransform_hash_algorithm()
{
    return  this->inverseReferenceRenderingTransform_hash_algorithm;
}
std::string ParseAMF::Get_inverseReferenceRenderingTransform_uuid(){return this->inverseReferenceRenderingTransform_uuid;}
std::string ParseAMF::Get_inverseReferenceRenderingTransform_file(){return this->inverseReferenceRenderingTransform_file;}
std::string ParseAMF::Get_inverseReferenceRenderingTransform_transformId(){return this->inverseReferenceRenderingTransform_transformId;}

bool ParseAMF::Get_lookTransform_applied(){
    if(this->lookTransform_applied=="true"){
        return true;
    }
    if(this->lookTransform_applied=="false"){
        return false;
    }
    else {
        return false;
    };
}
std::string ParseAMF::Get_lookTransform_description(){return this->lookTransform_description;}
std::string ParseAMF::Get_lookTransform_hash(){return this->lookTransform_hash;}

std::string ParseAMF::Get_lookTransform_hash_algorithm()
{
    return this->lookTransform_hash_algorithm;
}
std::string ParseAMF::Get_lookTransform_uuid(){return this->lookTransform_uuid;}
std::string ParseAMF::Get_lookTransform_file(){return this->lookTransform_file;}
std::string ParseAMF::Get_lookTransform_transformId(){return this->lookTransform_transformId;}

std::string ParseAMF::Get_cdlWorkingSpace_toCdlWorkingSpace_description(){return this->cdlWorkingSpace_toCdlWorkingSpace_description;}
std::string ParseAMF::Get_cdlWorkingSpace_toCdlWorkingSpace_hash(){return this->cdlWorkingSpace_toCdlWorkingSpace_hash;}

std::string ParseAMF::Get_cdlWorkingSpace_toCdlWorkingSpace_hash_algorithm()
{
    return this->cdlWorkingSpace_toCdlWorkingSpace_hash_algorithm;
}
std::string ParseAMF::Get_cdlWorkingSpace_toCdlWorkingSpace_uuid(){return this->cdlWorkingSpace_toCdlWorkingSpace_uuid;}
std::string ParseAMF::Get_cdlWorkingSpace_toCdlWorkingSpace_file(){return this->cdlWorkingSpace_toCdlWorkingSpace_file;}
std::string ParseAMF::Get_cdlWorkingSpace_toCdlWorkingSpace_transformId(){return this->cdlWorkingSpace_toCdlWorkingSpace_transformId;}
std::string ParseAMF::Get_cdlWorkingSpace_fromCdlWorkingSpace_description(){return this->cdlWorkingSpace_fromCdlWorkingSpace_description;}
std::string ParseAMF::Get_cdlWorkingSpace_fromCdlWorkingSpace_hash(){return this->cdlWorkingSpace_fromCdlWorkingSpace_hash;}

std::string ParseAMF::Get_cdlWorkingSpace_fromCdlWorkingSpace_hash_algorithm()
{
    return this->cdlWorkingSpace_fromCdlWorkingSpace_hash_algorithm;
}
std::string ParseAMF::Get_cdlWorkingSpace_fromCdlWorkingSpace_uuid(){return this->cdlWorkingSpace_fromCdlWorkingSpace_uuid;}
std::string ParseAMF::Get_cdlWorkingSpace_fromCdlWorkingSpace_file(){return this->cdlWorkingSpace_fromCdlWorkingSpace_file;}
std::string ParseAMF::Get_cdlWorkingSpace_fromCdlWorkingSpace_transformId(){return this->cdlWorkingSpace_fromCdlWorkingSpace_transformId;}

double ParseAMF::Get_ASC_SOP_Slope_R(){return this->ASC_SOP_Slope_R;}
double ParseAMF::Get_ASC_SOP_Slope_G(){return this->ASC_SOP_Slope_G;}
double ParseAMF::Get_ASC_SOP_Slope_B(){return this->ASC_SOP_Slope_B;}
double ParseAMF::Get_ASC_SOP_Offset_R(){return this->ASC_SOP_Offset_R;}
double ParseAMF::Get_ASC_SOP_Offset_G(){return this->ASC_SOP_Offset_G;}
double ParseAMF::Get_ASC_SOP_Offset_B(){return this->ASC_SOP_Offset_B;}
double ParseAMF::Get_ASC_SOP_Power_R(){return this->ASC_SOP_Power_R;}
double ParseAMF::Get_ASC_SOP_Power_G(){return this->ASC_SOP_Power_G;}
double ParseAMF::Get_ASC_SOP_Power_B(){return this->ASC_SOP_Power_B;}
double ParseAMF::Get_ASC_SAT_Saturation(){return this->ASC_SAT_Saturation;}
std::string ParseAMF::Get_ColorCorrectionRef_ref(){return this->ColorCorrectionRef_ref;}
std::string ParseAMF::Get_ColorCorrectionRef_Description(){return this->ColorCorrectionRef_Description;}

std::string ParseAMF::Get_ColorCorrectionRef_InputDescription()
{
    return this->ColorCorrectionRef_InputDescription;
}

std::string ParseAMF::Get_ColorCorrectionRef_ViewingDescription()
{
    return this->ColorCorrectionRef_ViewingDescription;
}

std::string ParseAMF::Get_outputTransform_description(){return this->outputTransform_description;}
std::string ParseAMF::Get_outputTransform_hash(){return this->outputTransform_hash;}

std::string ParseAMF::Get_outputTransform_hash_algorithm()
{
    return this->outputTransform_hash_algorithm;
}
std::string ParseAMF::Get_outputTransform_uuid(){return this->outputTransform_uuid;}
std::string ParseAMF::Get_outputTransform_file(){return this->outputTransform_file;}
std::string ParseAMF::Get_outputTransform_transformId(){return this->outputTransform_transformId;}

std::string ParseAMF::Get_outputDeviceTransform_description(){return this->outputDeviceTransform_description;}
std::string ParseAMF::Get_outputDeviceTransform_hash(){return this->outputDeviceTransform_hash;}

std::string ParseAMF::Get_outputDeviceTransform_hash_algorithm()
{
    return this->outputDeviceTransform_hash_algorithm;
}
std::string ParseAMF::Get_outputDeviceTransform_uuid(){return this->outputDeviceTransform_uuid;}
std::string ParseAMF::Get_outputDeviceTransform_file(){return this->outputDeviceTransform_file;}
std::string ParseAMF::Get_outputDeviceTransform_transformId(){return this->outputDeviceTransform_transformId;}

std::string ParseAMF::Get_referenceRenderingTransform_description(){return this->referenceRenderingTransform_description;}
std::string ParseAMF::Get_referenceRenderingTransform_hash(){return this->referenceRenderingTransform_hash;}

std::string ParseAMF::Get_referenceRenderingTransform_hash_algorithm()
{
    return this->referenceRenderingTransform_hash_algorithm;
}
std::string ParseAMF::Get_referenceRenderingTransform_uuid(){return this->referenceRenderingTransform_uuid;}
std::string ParseAMF::Get_referenceRenderingTransform_file(){return this->referenceRenderingTransform_file;}
std::string ParseAMF::Get_referenceRenderingTransform_transformId(){return this->referenceRenderingTransform_transformId;}

std::string ParseAMF::Get_CTLpath(){return CTLpath;}
std::string ParseAMF::Get_input_picpath(){return input_picpath;}
std::string ParseAMF::Get_input_picpath_exr(){return this->input_picpath_exr;}

std::string ParseAMF::Modification_input_picpath(std::string Modification_NewPicPath)
{
    this->input_picpath = Modification_NewPicPath;
    return this->input_picpath;
}

std::string ParseAMF::Modification_output_picpath(std::string Modification_NewPicPath)
{
    this->output_picpath = Modification_NewPicPath;
    return this->output_picpath;
}

std::string ParseAMF::Get_output_picpath(){return output_picpath;}

std::string ParseAMF::Get_LUT_Pic_16(){return this->LUT_Pic_16;}
std::string ParseAMF::Get_LUT_Pic_Origin(){return this->LUT_Pic_Origin;}
std::string ParseAMF::Get_LUT_Pic_64(){return this->LUT_Pic_64;}
std::string ParseAMF::Get_LUT_Pic_After(){return LUT_Pic_After;}

QString ParseAMF::Get_program(){return program;}
QString ParseAMF::Get_LUT_Location(){return LUT_Location;}


void ParseAMF::Reset_AMF_Value(){
    this->xmlns_aces="None";
    this->xmlns_cdl="None";
    this->xmlns_xsi="None";
    this->xsischema_Location="None";
    this->acesMetadataFile_Version="None";

    // AMF Info
    this->description="None";
    this->author_name="None";
    this->author_emailAdress="None";
    this->dateTime_creationDateTime="None";
    this->dateTime_modificationDateTime="None";
    this->uuid="None";

    // CilpID Info
    this->ClipID_description = "None";
    this->ClipID_cilpname = "None";
    this->ClipID_sequence = "None";
    this->ClipID_sequence_idx ="#";
    this->ClipID_sequence_min = 0;
    this->ClipID_sequence_max = 0;
    this->ClipID_path = "None";
    this->ClipID_uuid = "None";

    // Pipeline Info
    this->pipeline_description = "None";
    this->pipelineInfo_author_name = "None";
    this->pipelineInfo_author_emailAddress = "None";
    this->pipelineInfo_dateTime_creationDateTime = "None";
    this->pipelineInfo_dateTime_modificationDateTime = "None";
    this->pipelineInfo_uuid = "None";
    this->pipelineInfo_systemVersion_majorVersion = 0;
    this->pipelineInfo_systemVersion_minorVersion = 0;
    this->pipelineInfo_systemVersion_patchVersion = 0;

    // InputTransform Info
    this->inputTransform_applied = "None";                 // Get_inputTransform_applied返回值是bool
    this->inputTransform_description = "None";
    this->inputTransform_hash = "None";
    this->inputTransform_uuid = "None";
    this->inputTransform_file = "None";
    this->inputTransform_transformId = "None";

    this->inverseOutputTransform_description = "None";
    this->inverseOutputTransform_hash = "None";
    this->inverseOutputTransform_uuid = "None";
    this->inverseOutputTransform_file = "None";
    this->inverseOutputTransform_transformId = "None";

    this->inverseOutputDeviceTransform_description = "None";
    this->inverseOutputDeviceTransform_hash = "None";
    this->inverseOutputDeviceTransform_uuid = "None";
    this->inverseOutputDeviceTransform_file = "None";
    this->inverseOutputDeviceTransform_transformId = "None";

    this->inverseReferenceRenderingTransform_description = "None";
    this->inverseReferenceRenderingTransform_hash = "None";
    this->inverseReferenceRenderingTransform_uuid = "None";
    this->inverseReferenceRenderingTransform_file = "None";
    this->inverseReferenceRenderingTransform_transformId = "None";

    // LMT Info
    this->lookTransform_applied  = "None";                // Get_lookTransform_applied返回值是bool
    this->lookTransform_description  = "None";
    this->lookTransform_hash  = "None";
    this->lookTransform_uuid  = "None";
    this->lookTransform_file  = "None";
    this->lookTransform_transformId  = "None";

    this->cdlWorkingSpace_toCdlWorkingSpace_description  = "None";
    this->cdlWorkingSpace_toCdlWorkingSpace_hash  = "None";
    this->cdlWorkingSpace_toCdlWorkingSpace_uuid  = "None";
    this->cdlWorkingSpace_toCdlWorkingSpace_file  = "None";
    this->cdlWorkingSpace_toCdlWorkingSpace_transformId  = "None";

    this->cdlWorkingSpace_fromCdlWorkingSpace_description  = "None";
    this->cdlWorkingSpace_fromCdlWorkingSpace_hash  = "None";
    this->cdlWorkingSpace_fromCdlWorkingSpace_uuid  = "None";
    this->cdlWorkingSpace_fromCdlWorkingSpace_file  = "None";
    this->cdlWorkingSpace_fromCdlWorkingSpace_transformId  = "None";

    this->ASC_SOP_Slope_R = 1.000;
    this->ASC_SOP_Slope_G = 1.000;
    this->ASC_SOP_Slope_B = 1.000;
    this->ASC_SOP_Offset_R = 0;
    this->ASC_SOP_Offset_G = 0;
    this->ASC_SOP_Offset_B = 0;
    this->ASC_SOP_Power_R = 1.000;
    this->ASC_SOP_Power_G = 1.000;
    this->ASC_SOP_Power_B = 1.000;
    this->ASC_SAT_Saturation = 1.000;
    this->ColorCorrectionRef_ref = "None";
    this->ColorCorrectionRef_Description = "None";

    // OutputTransform Info
    this->outputTransform_description = "None";
    this->outputTransform_hash = "None";
    this->outputTransform_uuid = "None";
    this->outputTransform_file = "None";
    this->outputTransform_transformId = "None";

    this->outputDeviceTransform_description = "None";
    this->outputDeviceTransform_hash = "None";
    this->outputDeviceTransform_uuid = "None";
    this->outputDeviceTransform_file = "None";
    this->outputDeviceTransform_transformId = "None";

    this->referenceRenderingTransform_description = "None";
    this->referenceRenderingTransform_hash = "None";
    this->referenceRenderingTransform_uuid = "None";
    this->referenceRenderingTransform_file = "None";
    this->referenceRenderingTransform_transformId = "None";
}


