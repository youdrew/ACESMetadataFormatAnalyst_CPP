#include "ctlrendertool.h"
#include <QProcess>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QDialog>
#include <QDebug>
#include "parseamf.h"
#include "amfanalyst.h"
#include <stdlib.h>

ctlrendertool::ctlrendertool()
{
}

std::string  ctlrendertool::usingctlrender(ParseAMF amffile){
    QObject *parent=new QDialog;

    QStringList arguments;

    // 流程 IDT-> LMT -> RRT -> ODT -> outputDeviceTransform
    // IDT
    if (amffile.Get_inputTransform_transformId()!="" && amffile.Get_inputTransform_applied()){
        arguments << "-ctl" << QString::fromStdString(amffile.ParseAMF::GetCTLpath(amffile.Get_CTLpath(),amffile.Get_inputTransform_transformId()));
        std::cout << std::endl << std::endl << "IDT applied using: " << amffile.ParseAMF::GetCTLpath(amffile.Get_CTLpath(),amffile.Get_inputTransform_transformId()) << std::endl;
    }
    // LMT
    if (amffile.Get_lookTransform_transformId()!="" && amffile.Get_lookTransform_applied()){
        arguments << "-ctl" << QString::fromStdString(amffile.ParseAMF::GetCTLpath(amffile.Get_CTLpath(),amffile.Get_lookTransform_transformId()));
        std::cout << "LMT applied using: " << amffile.ParseAMF::GetCTLpath(amffile.Get_CTLpath(),amffile.Get_lookTransform_transformId()) << std::endl;
    }
    // RRT
    if (amffile.Get_referenceRenderingTransform_transformId()!=""){
        arguments << "-ctl" << QString::fromStdString(amffile.ParseAMF::GetCTLpath(amffile.Get_CTLpath(),amffile.Get_referenceRenderingTransform_transformId()));
        std::cout << "RRT applied using: " << amffile.ParseAMF::GetCTLpath(amffile.Get_CTLpath(),amffile.Get_referenceRenderingTransform_transformId()) << std::endl;
    }
    // ODT
    if (amffile.Get_outputTransform_transformId()!=""){
        arguments << "-ctl" << QString::fromStdString(amffile.ParseAMF::GetCTLpath(amffile.Get_CTLpath(),amffile.Get_outputTransform_transformId()));
        std::cout << "ODT applied using: " << amffile.ParseAMF::GetCTLpath(amffile.Get_CTLpath(),amffile.Get_outputTransform_transformId()) << std::endl;
    }
    // outputDeviceTransform
    if (amffile.Get_outputDeviceTransform_transformId()!=""){
        arguments << "-ctl" << QString::fromStdString(amffile.ParseAMF::GetCTLpath(amffile.Get_CTLpath(),amffile.Get_outputDeviceTransform_transformId()));
        std::cout << "ODT applied using: " << amffile.ParseAMF::GetCTLpath(amffile.Get_CTLpath(),amffile.Get_outputDeviceTransform_transformId()) << std::endl;
    }


    arguments << QString::fromStdString(amffile.Get_input_picpath()) << QString::fromStdString(amffile.Get_output_picpath());

    qDebug() <<"             \n        "<< arguments;

    QProcess *myProcess = new QProcess(parent);
    myProcess->start(amffile.Get_program(), arguments);
    qDebug() << "exec ctlrendertool::usingctlrender()";

    return amffile.Get_output_picpath();
}


void ctlrendertool::usingctlrender(QStringList CTLrender_paras, std::string PicLocation_input, std::string PicLocation_output)
{
    CTLrender_paras.push_back(QString::fromStdString(PicLocation_input));
    CTLrender_paras.push_back(QString::fromStdString(PicLocation_output));

    std::string paras;
    paras = this->CTLrenderCLI_Location;

    for(int i = 0; i< CTLrender_paras.size();++i)
    {
        std::string tmp = CTLrender_paras.at(i).toStdString();
         paras = paras +" " + tmp;
    }

    system(paras.c_str());
}


std::string ctlrendertool::usingctlrender(std::vector<std::string>){
    return "None";
}

QStringList ctlrendertool::usingctlrender_forLut(ParseAMF For_LUT_amf)
{
    QStringList arguments;

    // 流程 IDT-> LMT -> RRT -> ODT -> outputDeviceTransform
    // IDT
    if (For_LUT_amf.Get_inputTransform_transformId()!="" && For_LUT_amf.Get_inputTransform_applied()){
        arguments << "-ctl" << QString::fromStdString(For_LUT_amf.ParseAMF::GetCTLpath(For_LUT_amf.Get_CTLpath(),For_LUT_amf.Get_inputTransform_transformId())) << "-param1 aIn 1.0";
        std::cout << std::endl << std::endl << "IDT applied using: " << For_LUT_amf.ParseAMF::GetCTLpath(For_LUT_amf.Get_CTLpath(),For_LUT_amf.Get_inputTransform_transformId()) << std::endl;
    }
    // LMT
    if (For_LUT_amf.Get_lookTransform_transformId()!="" && For_LUT_amf.Get_lookTransform_applied()){
        arguments << "-ctl" << QString::fromStdString(For_LUT_amf.ParseAMF::GetCTLpath(For_LUT_amf.Get_CTLpath(),For_LUT_amf.Get_lookTransform_transformId())) << "-param1 aIn 1.0";
        std::cout << "LMT applied using: " << For_LUT_amf.ParseAMF::GetCTLpath(For_LUT_amf.Get_CTLpath(),For_LUT_amf.Get_lookTransform_transformId()) << std::endl;
    }
    // RRT
    if (For_LUT_amf.Get_referenceRenderingTransform_transformId()!=""){
        arguments << "-ctl" << QString::fromStdString(For_LUT_amf.ParseAMF::GetCTLpath(For_LUT_amf.Get_CTLpath(),For_LUT_amf.Get_referenceRenderingTransform_transformId())) << "-param1 aIn 1.0";
        std::cout << "RRT applied using: " << For_LUT_amf.ParseAMF::GetCTLpath(For_LUT_amf.Get_CTLpath(),For_LUT_amf.Get_referenceRenderingTransform_transformId()) << std::endl;
    }
    // ODT
    if (For_LUT_amf.Get_outputTransform_transformId()!=""){
        arguments << "-ctl" << QString::fromStdString(For_LUT_amf.ParseAMF::GetCTLpath(For_LUT_amf.Get_CTLpath(),For_LUT_amf.Get_outputTransform_transformId())) << "-param1 aIn 1.0";
        std::cout << "ODT applied using: " << For_LUT_amf.ParseAMF::GetCTLpath(For_LUT_amf.Get_CTLpath(),For_LUT_amf.Get_outputTransform_transformId()) << std::endl;
    }
    // outputDeviceTransform
    if (For_LUT_amf.Get_outputDeviceTransform_transformId()!=""){
        arguments << "-ctl" << QString::fromStdString(For_LUT_amf.ParseAMF::GetCTLpath(For_LUT_amf.Get_CTLpath(),For_LUT_amf.Get_outputDeviceTransform_transformId())) << "-param1 aIn 1.0";
        std::cout << "ODT applied using: " << For_LUT_amf.ParseAMF::GetCTLpath(For_LUT_amf.Get_CTLpath(),For_LUT_amf.Get_outputDeviceTransform_transformId()) << std::endl;
    }

    qDebug() <<"             \n        "<< arguments;

    return arguments;
}

std::string ctlrendertool::Get_PicLUT_Location(){return this->PicLUT_Location;};


