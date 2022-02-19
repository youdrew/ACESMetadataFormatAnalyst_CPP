#ifndef PARSEAMF_H
#define PARSEAMF_H

#include <iostream>
#include <string>

class ParseAMF
{
public:
    ParseAMF();
    ParseAMF(std::string Path);

    void PrintAllAmfInfo();             //用于测试输出private里面amf的信息是否检测到了，用于debug用。

private:
    // the infomation of ACES Metadata Format
    // Root File
    std::string xmlns_aces="None";
    std::string xmlns_cdl="None";
    std::string xmlns_xsi="None";
    std::string xsischema_Location="None";
    std::string acesMetadataFile_Version="None";

    // AMF Info
    std::string description="None";
    std::string author_name="None";
    std::string  author_emailAdress="None";
    std::string  dateTime_creationDateTime="None";
    std::string  dateTime_modificationDateTime="None";
    std::string  uuid="None";
    char  systemVersion_majorVersion=0;
    char  systemVersion_minorVersion=0;
    char  systemVersion_patchVersion=0;

    // CilpID Info
    std::string ClipID_description = "None";
    std::string ClipID_cilpname = "None";
    std::string ClipID_sequence = "None";
    char ClipID_sequence_idx ='#';
    int ClipID_sequence_min = 0;
    int ClipID_sequence_max = 0;
    std::string ClipID_path = "None";
    std::string ClipID_uuid = "None";

    // Pipeline Info
    std::string pipeline_description = "None";
    std::string pipelineInfo_author_name = "None";
    std::string pipelineInfo_author_emailAdress = "None";
    std::string pipelineInfo_dateTime_creationDateTime = "None";
    std::string pipelineInfo_dateTime_modificationDateTime = "None";
    std::string pipelineInfo_uuid = "None";
    std::string pipelineInfo_systemVersion_majorVersion = "None";
    std::string pipelineInfo_systemVersion_minorVersion = "None";
    std::string pipelineInfo_systemVersion_patchVersion = "None";

    // InputTransform Info
    std::string inputTransform_applied = "None";
    std::string inputTransform_description = "None";
    std::string inputTransform_hash = "None";
    std::string inputTransform_uuid = "None";
    std::string inputTransform_file = "None";
    std::string inputTransform_transformId = "None";

    std::string inverseOutputTransform_description = "None";
    std::string inverseOutputTransform_hash = "None";
    std::string inverseOutputTransform_uuid = "None";
    std::string inverseOutputTransform_file = "None";
    std::string inverseOutputTransform_transformId = "None";

    std::string inverseOutputDeviceTransform_description = "None";
    std::string inverseOutputDeviceTransform_hash = "None";
    std::string inverseOutputDeviceTransform_uuid = "None";
    std::string inverseOutputDeviceTransform_file = "None";
    std::string inverseOutputDeviceTransform_transformId = "None";

    std::string inverseReferenceRenderingTransform_description = "None";
    std::string inverseReferenceRenderingTransform_hash = "None";
    std::string inverseReferenceRenderingTransform_uuid = "None";
    std::string inverseReferenceRenderingTransform_file = "None";
    std::string inverseReferenceRenderingTransform_transformId = "None";

    // LMT Info
    std::string lookTransform_applied  = "None";
    std::string lookTransform_description  = "None";
    std::string lookTransform_hash  = "None";
    std::string lookTransform_uuid  = "None";
    std::string lookTransform_file  = "None";
    std::string lookTransform_transformId  = "None";

    std::string cdlWorkingSpace_toCdlWorkingSpace_description  = "None";
    std::string cdlWorkingSpace_toCdlWorkingSpace_hash  = "None";
    std::string cdlWorkingSpace_toCdlWorkingSpace_uuid  = "None";
    std::string cdlWorkingSpace_toCdlWorkingSpace_file  = "None";
    std::string cdlWorkingSpace_toCdlWorkingSpace_transformId  = "None";

    std::string cdlWorkingSpace_fromCdlWorkingSpace_description  = "None";
    std::string cdlWorkingSpace_fromCdlWorkingSpace_hash  = "None";
    std::string cdlWorkingSpace_fromCdlWorkingSpace_uuid  = "None";
    std::string cdlWorkingSpace_fromCdlWorkingSpace_file  = "None";
    std::string cdlWorkingSpace_fromCdlWorkingSpace_transformId  = "None";

    std::string ASC_SOP_Slope = "None";
    std::string ASC_SOP_Offset = "None";
    std::string ASC_SOP_Power = "None";
    std::string ASC_SAT_Saturation = "None";
    std::string ColorCorrectionRef_ref = "None";
    std::string ColorCorrectionRef_Description = "None";

    // OutputTransform Info
    std::string outputTransform_description = "None";
    std::string outputTransform_hash = "None";
    std::string outputTransform_uuid = "None";
    std::string outputTransform_file = "None";
    std::string outputTransform_transformId = "None";

    std::string outputDeviceTransform_description = "None";
    std::string outputDeviceTransform_hash = "None";
    std::string outputDeviceTransform_uuid = "None";
    std::string outputDeviceTransform_file = "None";
    std::string outputDeviceTransform_transformId = "None";

    std::string referenceRenderingTransform_description = "None";
    std::string referenceRenderingTransform_hash = "None";
    std::string referenceRenderingTransform_uuid = "None";
    std::string referenceRenderingTransform_file = "None";
    std::string referenceRenderingTransform_transformId = "None";
};

#endif // PARSEAMF_H
