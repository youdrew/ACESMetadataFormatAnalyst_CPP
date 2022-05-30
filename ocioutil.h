#include <OpenColorIO.h>
#include <iostream>




class ocioutil
{
public:
    ocioutil();



    /// TODO：将这些命令行工具更换成源码的形式
    void ociolutimage_extract_CLI(int LUTsize, std::string input_LUTpic_after,std::string Output_LUTLoctioan);         //调用命令行提取图片里的LUT信息，并返回其输出的LUT的位置
    void ocioociobakelut_TransLutFormat_CLI(std::string input_lut, std::string LUT_format,int LUTsize, std::string Output_LUTLoctioan);




private:
    std::string ociolutimage_Cli_DIR="/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/apps/ociolutimage/ociolutimage";


};
