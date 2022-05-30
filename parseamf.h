#ifndef PARSEAMF_H
#define PARSEAMF_H

#include <iostream>
#include <string>
#include <QString>
#include <vector>
#include <QDomDocument>
#include <QApplication>
#include <dirent.h>
#include <regex>
#include <QDateTime>

// 这里面涉及到解析AMF里面的信息

class ParseAMF
{
public:
    ParseAMF(QString);
    void RenderPic_Whenconstruct(QString AMFPath);      //给构造函数用，在构造的一开始，就把图片渲染出来，使用的图片是默认LogC格式的，使用的管道是AMF-Util进行解析，然后用CTLrender进行渲染
    //void RenderPic_WhenRenderLUT(QString AMFPath,std::string LUTSize,std::string LUTpic_OUTputLocation);

    ParseAMF();



    void ParsePipeline(QDomDocument);   //单独用于解析某个单Pipeline，可以在之后的<aces:archivedPipeline>部分使用
    void PrintAllAmfInfo();             //用于测试输出private里面amf的信息是否检测到了，用于debug用。

    void Reset_AMF_Value();             //Reset所有的值

    std::vector<std::string>  split(const std::string& str,const std::string& delim);   //用于切分一个string，以特定的切分符号，返回的是一个储存string的vector，在给CDL数据切分的时候用到了

    std::string GetAllFiles(std::string path,std::string nfile); //遍历path文件夹下所有文件,如果有跟nfile同名的ctl,则打印
    std::string GetCTLpath(std::string path, std::string name);     //用于获取到CTL的解析路径，返回的是解析后找到的具体某一个CTL

    // 获取私有变量
    std::string Get_CTLpath();
    std::string Get_input_picpath();
    std::string Get_input_picpath_exr();
    std::string Modification_input_picpath(std::string);
    std::string Modification_output_picpath(std::string);
    std::string Get_output_picpath();

    std::string Get_LUT_Pic_16();
    std::string Get_LUT_Pic_Origin();
    std::string Get_LUT_Pic_64();
    std::string Get_LUT_Pic_After();
    QString Get_program();
    QString Get_LUT_Location();


public:     // 用于获取private里面的值的接口

    std::string Get_xmlns_aces();
    std::string Get_xmlns_cdl();
    std::string Get_xmlns_xsi();
    std::string Get_xsischema_Location();
    std::string Get_acesMetadataFile_Version();

    std::string Get_description();
    std::string Get_author_name();
    std::string Get_author_emailAdress();
    std::string Get_dateTime_creationDateTime();
    std::string Get_dateTime_modificationDateTime();
    std::string Get_uuid();

    std::string Get_ClipID_description();
    std::string Get_ClipID_cilpname();
    std::string Get_ClipID_sequence();
    std::string Get_ClipID_sequence_idx();
    int Get_ClipID_sequence_min();
    int Get_ClipID_sequence_max();
    std::string Get_ClipID_path();
    std::string Get_ClipID_uuid();

    std::string Get_pipeline_description();
    std::string Get_pipelineInfo_author_name();
    std::string Get_pipelineInfo_author_emailAdress();
    std::string Get_pipelineInfo_dateTime_creationDateTime();
    std::string Get_pipelineInfo_dateTime_modificationDateTime();
    std::string Get_pipelineInfo_uuid();
    int Get_pipelineInfo_systemVersion_majorVersion();
    int Get_pipelineInfo_systemVersion_minorVersion();
    int Get_pipelineInfo_systemVersion_patchVersion();

    bool Get_inputTransform_applied();
    std::string Get_inputTransform_description();
    std::string Get_inputTransform_hash();
    std::string Get_inputTransform_hash_algorithm();
    std::string Get_inputTransform_uuid();
    std::string Get_inputTransform_file();
    std::string Get_inputTransform_transformId();

    std::string Get_inverseOutputTransform_description();
    std::string Get_inverseOutputTransform_hash();
    std::string Get_inverseOutputTransform_hash_algorithm();
    std::string Get_inverseOutputTransform_uuid();
    std::string Get_inverseOutputTransform_file();
    std::string Get_inverseOutputTransform_transformId();

    std::string Get_inverseOutputDeviceTransform_description();
    std::string Get_inverseOutputDeviceTransform_hash();
    std::string Get_inverseOutputDeviceTransform_hash_algorithm();
    std::string Get_inverseOutputDeviceTransform_uuid();
    std::string Get_inverseOutputDeviceTransform_file();
    std::string Get_inverseOutputDeviceTransform_transformId();

    std::string Get_inverseReferenceRenderingTransform_description();
    std::string Get_inverseReferenceRenderingTransform_hash();
    std::string Get_inverseReferenceRenderingTransform_hash_algorithm();
    std::string Get_inverseReferenceRenderingTransform_uuid();
    std::string Get_inverseReferenceRenderingTransform_file();
    std::string Get_inverseReferenceRenderingTransform_transformId();

    bool Get_lookTransform_applied();
    std::string Get_lookTransform_description();
    std::string Get_lookTransform_hash();
    std::string Get_lookTransform_hash_algorithm();
    std::string Get_lookTransform_uuid();
    std::string Get_lookTransform_file();
    std::string Get_lookTransform_transformId();

    std::string Get_cdlWorkingSpace_toCdlWorkingSpace_description();
    std::string Get_cdlWorkingSpace_toCdlWorkingSpace_hash();
    std::string Get_cdlWorkingSpace_toCdlWorkingSpace_hash_algorithm();
    std::string Get_cdlWorkingSpace_toCdlWorkingSpace_uuid();
    std::string Get_cdlWorkingSpace_toCdlWorkingSpace_file();
    std::string Get_cdlWorkingSpace_toCdlWorkingSpace_transformId();
    std::string Get_cdlWorkingSpace_fromCdlWorkingSpace_description();
    std::string Get_cdlWorkingSpace_fromCdlWorkingSpace_hash();
    std::string Get_cdlWorkingSpace_fromCdlWorkingSpace_hash_algorithm();
    std::string Get_cdlWorkingSpace_fromCdlWorkingSpace_uuid();
    std::string Get_cdlWorkingSpace_fromCdlWorkingSpace_file();
    std::string Get_cdlWorkingSpace_fromCdlWorkingSpace_transformId();

    double Get_ASC_SOP_Slope_R();
    double Get_ASC_SOP_Slope_G();
    double Get_ASC_SOP_Slope_B();
    double Get_ASC_SOP_Offset_R();
    double Get_ASC_SOP_Offset_G();
    double Get_ASC_SOP_Offset_B();
    double Get_ASC_SOP_Power_R();
    double Get_ASC_SOP_Power_G();
    double Get_ASC_SOP_Power_B();
    double Get_ASC_SAT_Saturation();
    std::string Get_ColorCorrectionRef_ref();
    std::string Get_ColorCorrectionRef_Description();
    std::string Get_ColorCorrectionRef_InputDescription();
    std::string Get_ColorCorrectionRef_ViewingDescription();

    std::string Get_outputTransform_description();
    std::string Get_outputTransform_hash();
    std::string Get_outputTransform_hash_algorithm();
    std::string Get_outputTransform_uuid();
    std::string Get_outputTransform_file();
    std::string Get_outputTransform_transformId();

    std::string Get_outputDeviceTransform_description();
    std::string Get_outputDeviceTransform_hash();
    std::string Get_outputDeviceTransform_hash_algorithm();
    std::string Get_outputDeviceTransform_uuid();
    std::string Get_outputDeviceTransform_file();
    std::string Get_outputDeviceTransform_transformId();

    std::string Get_referenceRenderingTransform_description();
    std::string Get_referenceRenderingTransform_hash();
    std::string Get_referenceRenderingTransform_hash_algorithm();
    std::string Get_referenceRenderingTransform_uuid();
    std::string Get_referenceRenderingTransform_file();
    std::string Get_referenceRenderingTransform_transformId();


private:
    // 每一次重新配置工程的时候需要看一下这里
    // 寻找ctl的路径
    std::string CTLpath= "/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/TestFile/ctl";

    // 图片输入和输出的路径
    std::string input_picpath = "/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/GraduationTestClassify/TIFF/TIFF_RGBA_LogC.tif" ;
    std::string input_picpath_exr = "/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/GraduationTestClassify/Exr/EXR_RGB_LogC.exr" ;
    std::string output_picpath = "/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/GraduationTestClassify/Out/"+QDateTime::currentDateTime().toString("yyyy_MMdd_hhmmss").toStdString()+".tif";

    std::string LUT_Pic_16 = "/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/TestFile/Pic_LUT/lut_origin_16.tif";
    std::string LUT_Pic_Origin = "/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/TestFile/Pic_LUT/lut_origin.tif";
    std::string LUT_Pic_64 = "/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/TestFile/Pic_LUT/lut_origin_64.tif";
    std::string LUT_Pic_After;

    // ctlrender所在的位置
    QString program="/usr/local/Cellar/ctl/1.5.2_6/bin/ctlrender";

    // 生成的Cube LUT所在的位置
    QString LUT_Location = "/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/TestFile/LUT/5213.cube";

    // the infomation of ACES Metadata Format
    // Root File
    std::string xmlns_aces="None";
    std::string xmlns_cdl="None";
    std::string xmlns_xsi="None";
    std::string xsischema_Location="None";
    std::string acesMetadataFile_Version="None";

    // AMF Info
    std::string description;
    std::string author_name;
    std::string  author_emailAdress;
    std::string  dateTime_creationDateTime;
    std::string  dateTime_modificationDateTime;
    std::string  uuid;

    // CilpID Info
    std::string ClipID_description ;
    std::string ClipID_cilpname ;
    std::string ClipID_sequence;
    std::string ClipID_sequence_idx ="#";
    int ClipID_sequence_min = 0;
    int ClipID_sequence_max = 0;
    std::string ClipID_path;
    std::string ClipID_uuid;

    // Pipeline Info
    std::string pipeline_description;
    std::string pipelineInfo_author_name;
    std::string pipelineInfo_author_emailAddress;
    std::string pipelineInfo_dateTime_creationDateTime;
    std::string pipelineInfo_dateTime_modificationDateTime;
    std::string pipelineInfo_uuid;
    int pipelineInfo_systemVersion_majorVersion = 0;
    int pipelineInfo_systemVersion_minorVersion = 0;
    int pipelineInfo_systemVersion_patchVersion = 0;

    // InputTransform Info
    std::string inputTransform_applied ;
    std::string inputTransform_description;
    std::string inputTransform_hash;
    std::string inputTransform_hash_algorithm;
    std::string inputTransform_uuid;
    std::string inputTransform_file;
    std::string inputTransform_transformId;

    std::string inverseOutputTransform_description;
    std::string inverseOutputTransform_hash;
    std::string inverseOutputTransform_hash_algorithm;
    std::string inverseOutputTransform_uuid;
    std::string inverseOutputTransform_file;
    std::string inverseOutputTransform_transformId;

    std::string inverseOutputDeviceTransform_description;
    std::string inverseOutputDeviceTransform_hash;
    std::string inverseOutputDeviceTransform_hash_algorithm;
    std::string inverseOutputDeviceTransform_uuid;
    std::string inverseOutputDeviceTransform_file;
    std::string inverseOutputDeviceTransform_transformId;

    std::string inverseReferenceRenderingTransform_description;
    std::string inverseReferenceRenderingTransform_hash;
    std::string inverseReferenceRenderingTransform_hash_algorithm;
    std::string inverseReferenceRenderingTransform_uuid;
    std::string inverseReferenceRenderingTransform_file;
    std::string inverseReferenceRenderingTransform_transformId;

    // LMT Info
    std::string lookTransform_applied;
    std::string lookTransform_description;
    std::string lookTransform_hash;
    std::string lookTransform_hash_algorithm;
    std::string lookTransform_uuid;
    std::string lookTransform_file;
    std::string lookTransform_transformId;

    std::string cdlWorkingSpace_toCdlWorkingSpace_description;
    std::string cdlWorkingSpace_toCdlWorkingSpace_hash;
    std::string cdlWorkingSpace_toCdlWorkingSpace_hash_algorithm;
    std::string cdlWorkingSpace_toCdlWorkingSpace_uuid;
    std::string cdlWorkingSpace_toCdlWorkingSpace_file;
    std::string cdlWorkingSpace_toCdlWorkingSpace_transformId;

    std::string cdlWorkingSpace_fromCdlWorkingSpace_description;
    std::string cdlWorkingSpace_fromCdlWorkingSpace_hash;
    std::string cdlWorkingSpace_fromCdlWorkingSpace_hash_algorithm;
    std::string cdlWorkingSpace_fromCdlWorkingSpace_uuid;
    std::string cdlWorkingSpace_fromCdlWorkingSpace_file;
    std::string cdlWorkingSpace_fromCdlWorkingSpace_transformId;

    double ASC_SOP_Slope_R = 1.000;
    double ASC_SOP_Slope_G = 1.000;
    double ASC_SOP_Slope_B = 1.000;
    double ASC_SOP_Offset_R = 0;
    double ASC_SOP_Offset_G = 0;
    double ASC_SOP_Offset_B = 0;
    double ASC_SOP_Power_R = 1.000;
    double ASC_SOP_Power_G = 1.000;
    double ASC_SOP_Power_B = 1.000;
    double ASC_SAT_Saturation = 1.000;
    std::string ColorCorrectionRef_ref;
    std::string ColorCorrectionRef_Description;
    std::string ColorCorrectionRef_InputDescription;
    std::string ColorCorrectionRef_ViewingDescription;

    // OutputTransform Info
    std::string outputTransform_description;
    std::string outputTransform_hash;
    std::string outputTransform_hash_algorithm;
    std::string outputTransform_uuid;
    std::string outputTransform_file;
    std::string outputTransform_transformId;

    std::string outputDeviceTransform_description;
    std::string outputDeviceTransform_hash;
    std::string outputDeviceTransform_hash_algorithm;
    std::string outputDeviceTransform_uuid;
    std::string outputDeviceTransform_file;
    std::string outputDeviceTransform_transformId;

    std::string referenceRenderingTransform_description;
    std::string referenceRenderingTransform_hash;
    std::string referenceRenderingTransform_hash_algorithm;
    std::string referenceRenderingTransform_uuid;
    std::string referenceRenderingTransform_file;
    std::string referenceRenderingTransform_transformId;
};

#endif // PARSEAMF_H
