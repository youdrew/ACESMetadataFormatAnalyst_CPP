#include "ocioutil.h"
#include <OpenColorIO.h>
#include <OpenColorTransforms.h>
#include <QStringList>

ocioutil::ocioutil()
{

}

void ocioutil::ociolutimage_extract_CLI(int LUTsize, std::string input_LUTpic_after,std::string Output_LUTLoctioan)
{
    QStringList ociolutimage_paras;

    ociolutimage_paras.push_back(this->ociolutimage_Cli_DIR.c_str());

    ociolutimage_paras.push_back("--extract");
    ociolutimage_paras.push_back("--cubesize");
    ociolutimage_paras.push_back(std::to_string(LUTsize).c_str());
    ociolutimage_paras.push_back("--input");
    ociolutimage_paras.push_back(input_LUTpic_after.c_str());
    ociolutimage_paras.push_back("--output");
    Output_LUTLoctioan=Output_LUTLoctioan+"_LUT.spi3d";
    ociolutimage_paras.push_back(Output_LUTLoctioan.c_str());

    std::string paras;

    for(int i = 0; i< ociolutimage_paras.size();++i)
    {
        std::string tmp = ociolutimage_paras.at(i).toStdString();
         paras = paras +" " + tmp;
    }

    std::cout << paras << std::endl;

    system(paras.c_str());
}

void ocioutil::ocioociobakelut_TransLutFormat_CLI(std::string input_lut, std::string LUT_format, int LUTsize, std::string Output_LUTLoctioan)
{
    QStringList ocioociobakelut_paras;

    ocioociobakelut_paras.push_back("/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/apps/builded/ociobakelut");

    ocioociobakelut_paras.push_back("--lut");
    std::string input_LUT_paras = input_lut+"_LUT.spi3d";
    ocioociobakelut_paras.push_back(input_LUT_paras.c_str());

    ocioociobakelut_paras.push_back("--format");
    ocioociobakelut_paras.push_back(LUT_format.c_str());

    ocioociobakelut_paras.push_back("--cubesize");
    ocioociobakelut_paras.push_back(std::to_string(LUTsize).c_str());

    ocioociobakelut_paras.push_back(Output_LUTLoctioan.c_str());

    std::string paras;
    for(int i = 0; i< ocioociobakelut_paras.size();++i)
    {
        std::string tmp = ocioociobakelut_paras.at(i).toStdString();
         paras = paras +" " + tmp;
    }

    std::cout << paras << std::endl;
    system(paras.c_str());
}
