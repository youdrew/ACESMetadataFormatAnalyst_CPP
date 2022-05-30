#include "ui_amfanalyst.h"
#include <QApplication>
#include <iostream>
#include <QFileDialog>
#include <QInputDialog>
#include "amfanalyst.h"
#include "view.h"

amfanalyst::amfanalyst(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::amfanalyst)
{
    ui->setupUi(this);
    //信号和槽的机制

    // Import Panel / lift
    connect(ui->IMPORTPanel_Lift_ImportAMF_pushBotton,SIGNAL(clicked()),this,SLOT(Getuser_AMFpath()));//点击获取AMF导入路径

    // AnalysisModification Panel / Navigater
    connect(ui->Pipeline_Navigator_PipelineInfo_pushButton,SIGNAL(clicked()),this,SLOT(Navigater0()));  // 点击Navigater，面板随之跳动
    connect(ui->Pipeline_Navigator_IDT_pushButton,SIGNAL(clicked()),this,SLOT(Navigater1()));
    connect(ui->Pipeline_Navigator_LMT_pushButton,SIGNAL(clicked()),this,SLOT(Navigater2()));
    connect(ui->Pipeline_Navigator_ODT_pushButton,SIGNAL(clicked()),this,SLOT(Navigater3()));
    connect(ui->ANALYSISPanel_Pipeline_Lift_ToolBox,SIGNAL(currentChanged(int)),this,SLOT(Navigater(int))); // 面板状态改0变，Navigater随之标注为选中

    //AnalysisModification Panel / Pipeline / LMT
    // Slope
    connect(ui->Pipeline_RSlope_horizontalSlider,&QAbstractSlider::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotslider);      // LMT中CDL double选框和滑块的联动
    connect(ui->Pipeline_RSlope_DoublespinBox,&QDoubleSpinBox::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotDoubleSpinbox);
    connect(ui->Pipeline_GSlope_horizontalSlider,&QAbstractSlider::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotslider);
    connect(ui->Pipeline_GSlope_DoublespinBox,&QDoubleSpinBox::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotDoubleSpinbox);
    connect(ui->Pipeline_BSlope_horizontalSlider,&QAbstractSlider::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotslider);
    connect(ui->Pipeline_BSlope_DoublespinBox,&QDoubleSpinBox::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotDoubleSpinbox);
    // Offset
    connect(ui->Pipeline_ROffset_horizontalSlider,&QAbstractSlider::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotslider);
    connect(ui->Pipeline_ROffset_DoublespinBox,&QDoubleSpinBox::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotDoubleSpinbox);
    connect(ui->Pipeline_GOffset_horizontalSlider,&QAbstractSlider::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotslider);
    connect(ui->Pipeline_GOffset_DoublespinBox,&QDoubleSpinBox::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotDoubleSpinbox);
    connect(ui->Pipeline_BOffset_horizontalSlider,&QAbstractSlider::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotslider);
    connect(ui->Pipeline_BOffset_DoublespinBox,&QDoubleSpinBox::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotDoubleSpinbox);
    // Power
    connect(ui->Pipeline_RPower_horizontalSlider,&QAbstractSlider::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotslider);
    connect(ui->Pipeline_RPower_DoublespinBox,&QDoubleSpinBox::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotDoubleSpinbox);
    connect(ui->Pipeline_GPower_horizontalSlider,&QAbstractSlider::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotslider);
    connect(ui->Pipeline_GPower_DoublespinBox,&QDoubleSpinBox::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotDoubleSpinbox);
    connect(ui->Pipeline_BPower_horizontalSlider,&QAbstractSlider::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotslider);
    connect(ui->Pipeline_BPower_DoublespinBox,&QDoubleSpinBox::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotDoubleSpinbox);
    // Saturation
    connect(ui->Pipeline_Saturation_horizontalSlider,&QAbstractSlider::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotslider);
    connect(ui->Pipeline_Saturation_DoublespinBox,&QDoubleSpinBox::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotDoubleSpinbox);

    // display
    connect(ui->AnalysisPanel_SaveToTiff_pushButton,SIGNAL(clicked()),this,SLOT(grab_2_tiff()));
    connect(ui->AnalysisPanel_SaveToexr_pushButton,SIGNAL(clicked()),this,SLOT(grab_2_exr()));

    //Output Panel / Save as AMF
    connect(ui->OUTPUTPanel_PathSelect_PushButton,SIGNAL(clicked()),this,SLOT(Getuser_AMFOutputAMFPath()));
    connect(ui->OUTPUTPanel_AMF_SaveAs_pushButton,SIGNAL(clicked()),this,SLOT(Creation_OutputAMF()));

    ui->SaveFileName_label_lineEdit->setText(this->Get_Time());

    connect(ui->Pipeline_Editable_checkBox,SIGNAL(stateChanged(int)),this,SLOT(Refresh_Editable_state(int)));
    ui->Pipeline_Editable_checkBox->setChecked(false);
    Refresh_Editable_state(1);

    connect(ui->LUT_Output_pushButton,SIGNAL(clicked()),this,SLOT(Creation_LUT()));
}


QString amfanalyst::Get_Time(){
    QDateTime time = QDateTime::currentDateTime();
    QString time_str = time.toString("yyyy_MMdd_hhmmss");
    return time_str;
}

amfanalyst::~amfanalyst()
{
    delete ui;
}

QString amfanalyst::Get_newAMFpath()
{
    return this->newAMFpath;
}

QString amfanalyst::Get_OutputAMFPath()
{
    return this->OutputAMFPath;
}

void amfanalyst::Getuser_AMFpath()
{
    QString file_full, file_name, file_path;

    QFileInfo fileinfo;                         // 创建读取窗口
    file_full = QFileDialog::getOpenFileName(this,"choose a amf file","/",tr("ACES Related file (*.amf *.AMF)"));
    fileinfo = QFileInfo(file_full);
    file_path = fileinfo.absolutePath();
    file_name = fileinfo.fileName();

    this->newAMFpath=file_path+"/"+file_name;   // 把地址给到用户界面


    /// TODO:重复读取问题
    static ParseAMF amf_file1(this->Get_newAMFpath());     // 实例化一个“分析amf”，这里一定要用static关键字，不让这个函数结束了，对象amf_file1就死了，后面的调用取它里面的地址的操作都不行了
                                                           // amf_file1关键字让对象有了 生存期与程序运行期相同的寿命
    this->dispParsedAMF(&amf_file1);

     this->currentamf->RenderPic_Whenconstruct(this->Get_newAMFpath());  // amf-util管道，// 调用渲染管道渲染初始化的图片,双管道开关可以在这里使用，如果不想使用双管道，可以注释调这行，关闭amf-util



    ctlrendertool *c1=new ctlrendertool;            // 启用管线，在构造parseAMF的时候就调用了AMF-util（python）来执行渲染，但是考虑到兼容性问题，以及可能出现的渲染失误，我在外面又使用了我自己的解析渲染。
    c1->usingctlrender(amf_file1);                  // 在这里关闭双管道，注释调这行关闭调我自己的管道。

    this->IfOpenAMF=1;
}

void amfanalyst::Getuser_AMFOutputAMFPath()
{

     QString file_path;
     file_path = QFileDialog::getExistingDirectory(this,"choose a amf save path:");
     this->OutputAMFPath = file_path;
     ui->OUTPUTPanel_SaveAs_LineEdit->setText(this->OutputAMFPath);
}

void amfanalyst::Getuser_AMFOutputAMFName()
{

     QString file_name;
     bool ok;
     file_name = QInputDialog::getText(this, tr("Enter AMF file name"),
                                              tr("Since no default file name exists, please enter an AMF file name to save this AMF file:"), QLineEdit::Normal,
                                              QDir::home().dirName(), &ok);
     ui->SaveFileName_label_lineEdit->setText(file_name);
}

void amfanalyst::Creation_OutputAMF()
{
    if(ui->OUTPUTPanel_SaveAs_LineEdit->text().isEmpty()){                              // 如果没有识别到保存路径，要求获取一个保存路径
      this->Getuser_AMFOutputAMFPath();
    };


    if(!(this->Get_OutputAMFPath().isEmpty()))
    {
        QFile file("test.xml");                                                         //创建一个叫test.xml的文件，会存在工程的build路径里

        bool ret = file.open(QIODevice::WriteOnly | QIODevice::Truncate);
        if (!ret)
        {
            qDebug() << "打开文件失败 amfanalyst::Creation_OutputAMF()";
            return;
        }

        /// 如果有些文件不存在的部分，不会替代调初始化里面的"None",这个问题需要解决一下。
        //开始给xml中写入信息
        /// TODO: 属性乱序问题
        qSetGlobalQHashSeed(100);   // 不加这个，“setAttribute”功能会出现乱序排列

        QDomDocument doc;

        //xml头文件
        QDomProcessingInstruction instruction;
        instruction = doc.createProcessingInstruction("xml",
                                     "version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"");
        doc.appendChild(instruction);

        //根节点
        QDomElement root =doc.createElement(tr("aces:acesMetadataFile"));
        doc.appendChild(root);

        root.setAttribute("xmlns:aces",QString::fromStdString(this->currentamf->Get_xmlns_aces()));
        root.setAttribute("xmlns:cdl",QString::fromStdString(this->currentamf->Get_xmlns_cdl()));
        root.setAttribute("xmlns:xsi",QString::fromStdString(this->currentamf->Get_xmlns_xsi()));
        root.setAttribute("xsi:schemaLocation",QString::fromStdString(this->currentamf->Get_xsischema_Location()));
        root.setAttribute("version",QString::fromStdString(this->currentamf->Get_acesMetadataFile_Version()));

        //创建4大子节点
        QDomElement aces_amfInfo = doc.createElement(tr("aces:amfInfo"));
        QDomElement aces_clipId = doc.createElement(tr("aces:clipId"));
        QDomElement aces_pipeline = doc.createElement(tr("aces:pipeline"));

        root.appendChild(aces_amfInfo);
        root.appendChild(aces_clipId);
        root.appendChild(aces_pipeline);

        // 考虑到目前aces:archivedPipeline支持不是很好，所有这个功能暂时隐蔽
        // Due to current aces:archivedPipeline support is not very good
        // all this function is temporarily hidden

        //QDomElement aces_archivedPipeline = doc.createElement(tr("aces:archivedPipeline"));
        //root.appendChild(aces_archivedPipeline);


        // aces_amfInfo
        // 创建所有元素
        QDomElement aces_description = doc.createElement(tr("aces:description"));

        QDomElement aces_author = doc.createElement(tr("aces:author"));             //包含子元素
        QDomElement aces_name = doc.createElement(tr("aces:name"));
        QDomElement aces_emailAddress = doc.createElement(tr("aces:emailAddress"));

        QDomElement aces_dateTime = doc.createElement(tr("aces:dateTime"));         //包含子元素
        QDomElement aces_creationDateTime = doc.createElement(tr("aces:creationDateTime"));
        QDomElement aces_modificationDateTime = doc.createElement(tr("aces:modificationDateTime"));

        QDomElement aces_uuid = doc.createElement(tr("aces:uuid"));

        // 从界面上获取值
        QDomText aces_description_text;     //用于存储要临时往里面填写的值
        aces_description_text = doc.createTextNode(ui->ANALYSISPanel_Info_AMFInfo_TextEdit->toPlainText());
        QDomText aces_name_text;
        aces_name_text = doc.createTextNode(ui->ANALYSISPanel_Info_Name_LineEdit->text());
        QDomText aces_emailAddress_text;
        aces_emailAddress_text = doc.createTextNode(ui->ANALYSISPanel_Info_EmailAddress_LineEdit->text());
        QDomText aces_creationDateTime_text;
        aces_creationDateTime_text = doc.createTextNode(QString::fromStdString(this->currentamf->Get_dateTime_creationDateTime()));
        QDomText aces_modificationDateTime_text;
        aces_modificationDateTime_text = doc.createTextNode(QString::fromStdString(this->currentamf->Get_dateTime_modificationDateTime()));
        QDomText aces_uuid_text;
        aces_uuid_text = doc.createTextNode(ui->ANALYSISPanel_Info_AMFUUID_LineEdit->text());

        // 给元素赋值
        aces_description.appendChild(aces_description_text);
        aces_name.appendChild(aces_name_text);
        aces_emailAddress.appendChild(aces_emailAddress_text);
        aces_creationDateTime.appendChild(aces_creationDateTime_text);
        aces_modificationDateTime.appendChild(aces_modificationDateTime_text);
        aces_uuid.appendChild(aces_uuid_text);

        // 编辑元素从属逻辑
        if(!ui->ANALYSISPanel_Pipeline_Info_textEdit->document()->isEmpty())(aces_amfInfo.appendChild(aces_description));

        aces_amfInfo.appendChild(aces_author);      //Required
        aces_author.appendChild(aces_name);
        aces_author.appendChild(aces_emailAddress);

        aces_amfInfo.appendChild(aces_dateTime);    //Required
        aces_dateTime.appendChild(aces_creationDateTime);
        aces_dateTime.appendChild(aces_modificationDateTime);

        if(!ui->ANALYSISPanel_Info_AMFUUID_LineEdit->text().isEmpty())(aces_amfInfo.appendChild(aces_uuid));


        // CilpID Info
        // 创建所有元素
        QDomElement aces_clipName = doc.createElement(tr("aces:clipName"));
        QDomElement aces_file = doc.createElement(tr("aces:file"));             //Choice of aces:file, aces:sequence or aces:uuid is required
        QDomElement aces_sequence = doc.createElement(tr("aces:sequence"));     //idx, min, max
        QDomElement clipid_aces_uuid = doc.createElement(tr("aces:uuid"));

        // 获取界面上的值
        QDomText aces_clipName_text = doc.createTextNode(ui->ANALYSISPanel_Info_CilpName_LineEdit->text());
        QDomText aces_file_text = doc.createTextNode(ui->ANALYSISPanel_Info_FilePath_LineEdit->text());
        QDomText aces_sequence_text = doc.createTextNode(ui->ANALYSISPanel_Info_Sequence_LineEdit->text());
        QDomText clipid_aces_uuid_text = doc.createTextNode(ui->ANALYSISPanel_Info_CilpUUID_LineEdit->text());

        // 给元素赋值
        aces_clipName.appendChild(aces_clipName_text);
        aces_sequence.appendChild(aces_sequence_text);
        aces_sequence.setAttribute("idx",ui->idx_lineEdit->text());
        aces_sequence.setAttribute("min",ui->min_spinBox->text());
        aces_sequence.setAttribute("max",ui->max_spinBox->text());
        aces_file.appendChild(aces_file_text);
        clipid_aces_uuid.appendChild(clipid_aces_uuid_text);

        // 编辑从属
        if((!ui->ANALYSISPanel_Info_CilpName_LineEdit->text().isEmpty())
                ||(!ui->ANALYSISPanel_Info_FilePath_LineEdit->text().isEmpty())
                ||(!ui->ANALYSISPanel_Info_Sequence_LineEdit->text().isEmpty())
                ||(!ui->ANALYSISPanel_Info_CilpUUID_LineEdit->text().isEmpty()))
            {
                aces_clipId.appendChild(aces_clipName);      //Required

                if(!ui->ANALYSISPanel_Info_Sequence_LineEdit->text().isEmpty()){aces_clipId.appendChild(aces_sequence);}
                if(!ui->ANALYSISPanel_Info_FilePath_LineEdit->text().isEmpty()){aces_clipId.appendChild(aces_file);}
                if(!ui->ANALYSISPanel_Info_CilpUUID_LineEdit->text().isEmpty()){aces_clipId.appendChild(clipid_aces_uuid);}
            };


        // pipeline
        // 创建所有元素
        // info
        QDomElement pipeline_pipelineInfo = doc.createElement(tr("aces:pipelineInfo"));

        QDomElement pipelineInfo_aces_description = doc.createElement(tr("aces:description"));

        QDomElement pipelineInfo_aces_dateTime = doc.createElement(tr("aces:dateTime"));
        QDomElement pipelineInfo_aces_creationDateTime = doc.createElement(tr("aces:creationDateTime"));
        QDomElement pipelineInfo_aces_modificationDateTime = doc.createElement(tr("aces:modificationDateTime"));

        QDomElement pipelineInfo_aces_uuid =doc.createElement(tr("aces:uuid"));

        QDomElement pipelineInfo_aces_systemVersion =doc.createElement(tr("aces:systemVersion"));            //包含子元素
        QDomElement pipelineInfo_aces_majorVersion =doc.createElement(tr("aces:majorVersion"));
        QDomElement pipelineInfo_aces_minorVersion =doc.createElement(tr("aces:minorVersion"));
        QDomElement pipelineInfo_aces_patchVersion =doc.createElement(tr("aces:patchVersion"));

        QDomElement pipelineInfo_aces_author = doc.createElement(tr("aces:author"));             //包含子元素
        QDomElement pipelineInfo_aces_name = doc.createElement(tr("aces:name"));
        QDomElement pipelineInfo_aces_emailAddress = doc.createElement(tr("aces:emailAddress"));

        // inputTransform
        QDomElement pipeline_inputTransform = doc.createElement(tr("aces:inputTransform"));
        QDomElement inputTransform_aces_description = doc.createElement(tr("aces:description"));
        QDomElement inputTransform_aces_hash = doc.createElement(tr("aces:hash"));
        QDomElement inputTransform_aces_uuid = doc.createElement(tr("aces:uuid"));
        QDomElement inputTransform_aces_file = doc.createElement(tr("aces:file"));
        QDomElement inputTransform_aces_transformId= doc.createElement(tr("aces:transformId"));

        QDomElement inputTransform_inverseOutputTransform = doc.createElement(tr("aces:inverseOutputDeviceTransform"));           //包含子元素
        QDomElement inputTransform_inverseOutputTransform_aces_description = doc.createElement(tr("aces:description"));
        QDomElement inputTransform_inverseOutputTransform_aces_hash = doc.createElement(tr("aces:hash"));
        QDomElement inputTransform_inverseOutputTransform_aces_uuid = doc.createElement(tr("aces:uuid"));
        QDomElement inputTransform_inverseOutputTransform_aces_file = doc.createElement(tr("aces:file"));
        QDomElement inputTransform_inverseOutputTransform_aces_transformId= doc.createElement(tr("aces:transformId"));

        QDomElement inputTransform_inverseOutputDeviceTransform = doc.createElement(tr("aces:inverseOutputDeviceTransform"));           //包含子元素
        QDomElement inputTransform_inverseOutputDeviceTransform_aces_description = doc.createElement(tr("aces:description"));
        QDomElement inputTransform_inverseOutputDeviceTransform_aces_hash = doc.createElement(tr("aces:hash"));
        QDomElement inputTransform_inverseOutputDeviceTransform_aces_uuid = doc.createElement(tr("aces:uuid"));
        QDomElement inputTransform_inverseOutputDeviceTransform_aces_file = doc.createElement(tr("aces:file"));
        QDomElement inputTransform_inverseOutputDeviceTransform_aces_transformId= doc.createElement(tr("aces:transformId"));

        QDomElement inputTransform_inverseReferenceRenderingTransform = doc.createElement(tr("aces:inverseReferenceRenderingTransform"));           //包含子元素
        QDomElement inputTransform_inverseReferenceRenderingTransform_aces_description = doc.createElement(tr("aces:description"));
        QDomElement inputTransform_inverseReferenceRenderingTransform_aces_hash = doc.createElement(tr("aces:hash"));
        QDomElement inputTransform_inverseReferenceRenderingTransform_aces_uuid = doc.createElement(tr("aces:uuid"));
        QDomElement inputTransform_inverseReferenceRenderingTransform_aces_file = doc.createElement(tr("aces:file"));
        QDomElement inputTransform_inverseReferenceRenderingTransform_aces_transformId= doc.createElement(tr("aces:transformId"));

        // lookTransform
        QDomElement pipeline_lookTransform = doc.createElement(tr("aces:lookTransform"));
        QDomElement lookTransform_aces_description = doc.createElement(tr("aces:description"));
        QDomElement lookTransform_aces_hash = doc.createElement(tr("aces:hash"));
        QDomElement lookTransform_aces_uuid = doc.createElement(tr("aces:uuid"));
        QDomElement lookTransform_aces_file = doc.createElement(tr("aces:file"));
        QDomElement lookTransform_aces_transformId = doc.createElement(tr("aces:transformId"));

        QDomElement lookTransform_cdlWorkingSpace = doc.createElement(tr("aces:cdlWorkingSpace"));
        QDomElement lookTransform_toCdlWorkingSpace = doc.createElement(tr("aces:toCdlWorkingSpace"));
        QDomElement toCdlWorkingSpace_aces_description = doc.createElement(tr("aces:description"));
        QDomElement toCdlWorkingSpace_aces_hash = doc.createElement(tr("aces:hash"));
        QDomElement toCdlWorkingSpace_aces_uuid = doc.createElement(tr("aces:uuid"));
        QDomElement toCdlWorkingSpace_aces_file = doc.createElement(tr("aces:file"));
        QDomElement toCdlWorkingSpace_aces_transformId = doc.createElement(tr("aces:transformId"));
        QDomElement lookTransform_fromCdlWorkingSpace = doc.createElement(tr("aces:fromCdlWorkingSpace"));
        QDomElement fromCdlWorkingSpace_aces_description = doc.createElement(tr("aces:description"));
        QDomElement fromCdlWorkingSpace_aces_hash = doc.createElement(tr("aces:hash"));
        QDomElement fromCdlWorkingSpace_aces_uuid = doc.createElement(tr("aces:uuid"));
        QDomElement fromCdlWorkingSpace_aces_file = doc.createElement(tr("aces:file"));
        QDomElement fromCdlWorkingSpace_aces_transformId = doc.createElement(tr("aces:transformId"));

        QDomElement lookTransform_cdl_SOPNode = doc.createElement(tr("cdl:SOPNode"));
        QDomElement cdl_SOPNode_cdl_Slope = doc.createElement(tr("cdl:Slope"));
        QDomElement cdl_SOPNode_cdl_Offset = doc.createElement(tr("cdl:Offset"));
        QDomElement cdl_SOPNode_cdl_Power = doc.createElement(tr("cdl:Power"));

        QDomElement lookTransform_cdl_SATNode = doc.createElement(tr("cdl:SATNode"));
        QDomElement lookTransform_cdl_Saturation = doc.createElement(tr("cdl:Saturation"));

        QDomElement lookTransform_cdl_ColorCorrectionRef = doc.createElement(tr("cdl:ColorCorrectionRef"));
        QDomElement ColorCorrectionRef_cdl_Description = doc.createElement(tr("cdl:Description"));
        QDomElement ColorCorrectionRef_cdl_InputDescription = doc.createElement(tr("cdl:InputDescription"));
        QDomElement ColorCorrectionRef_cdl_ViewingDescription = doc.createElement(tr("cdl:ViewingDescription"));

        // outputTransform
        QDomElement pipeline_outputTransform = doc.createElement(tr("aces:outputTransform"));
        QDomElement outputTransform_aces_description = doc.createElement(tr("aces:description"));
        QDomElement outputTransform_aces_hash = doc.createElement(tr("aces:hash"));
        QDomElement outputTransform_aces_uuid = doc.createElement(tr("aces:uuid"));
        QDomElement outputTransform_aces_file = doc.createElement(tr("aces:file"));
        QDomElement outputTransform_aces_transformId = doc.createElement(tr("aces:transformId"));

        QDomElement outputTransform_outputDeviceTransform = doc.createElement(tr("aces:outputDeviceTransform"));
        QDomElement outputDeviceTransform_aces_description = doc.createElement(tr("aces:description"));
        QDomElement outputDeviceTransform_aces_hash = doc.createElement(tr("aces:hash"));
        QDomElement outputDeviceTransform_aces_uuid = doc.createElement(tr("aces:uuid"));
        QDomElement outputDeviceTransform_aces_file = doc.createElement(tr("aces:file"));
        QDomElement outputDeviceTransform_aces_transformId = doc.createElement(tr("aces:transformId"));

        QDomElement outputTransform_referenceRenderingTransform = doc.createElement(tr("aces:referenceRenderingTransform"));
        QDomElement referenceRenderingTransform_aces_description = doc.createElement(tr("aces:description"));
        QDomElement referenceRenderingTransform_aces_hash = doc.createElement(tr("aces:hash"));
        QDomElement referenceRenderingTransform_aces_uuid = doc.createElement(tr("aces:uuid"));
        QDomElement referenceRenderingTransform_aces_file = doc.createElement(tr("aces:file"));
        QDomElement referenceRenderingTransform_aces_transformId = doc.createElement(tr("aces:transformId"));

        // 获取界面上的值
        // info
        QDomText pipelineInfo_aces_description_text = doc.createTextNode(ui->ANALYSISPanel_Pipeline_Info_textEdit->toPlainText());

        QDomText pipelineInfo_aces_creationDateTime_text = doc.createTextNode(ui->ANALYSISPanel_Pipeline_Info_CreationDatetime_lineEdit->text());
        QDomText pipelineInfo_aces_modificationDateTime_text = doc.createTextNode(ui->ANALYSISPanel_Pipeline_Info_ModificationDatetime_lineEdit->text());

        QDomText pipelineInfo_aces_uuid_text = doc.createTextNode(ui->ANALYSISPanel_Pipeline_Info_UUID_lineEdit->text());

        QDomText pipelineInfo_aces_majorVersion_text = doc.createTextNode(ui->pipelineInfo_systemVersion_majorVersion_spinBox->text());
        QDomText pipelineInfo_aces_minorVersion_text = doc.createTextNode(ui->pipelineInfo_systemVersion_minorVersion_spinBox->text());
        QDomText pipelineInfo_aces_patchVersion_text = doc.createTextNode(ui->pipelineInfo_systemVersion_patchVersion_spinBox->text());

        QDomText pipelineInfo_aces_name_text = doc.createTextNode(ui->ANALYSISPanel_Pipeline_Info__AuthorName_lineEdit->text());
        QDomText pipelineInfo_aces_emailAddress_text = doc.createTextNode(ui->ANALYSISPanel_Pipeline_Info_AuthorEmail_lineEdit->text());

        // inputTransform
        QDomText inputTransform_aces_description_text = doc.createTextNode(ui->inputTransform_description_textEdit->toPlainText());
        QDomText inputTransform_aces_hash_text = doc.createTextNode(ui->inputTransform_hash_lineEdit->text());
        QDomText inputTransform_aces_uuid_text = doc.createTextNode(ui->inputTransform_uuid_lineEdit->text());
        QDomText inputTransform_aces_file_text = doc.createTextNode(ui->inputTransform_file_lineEdit->text());
        QDomText inputTransform_aces_transformId_text = doc.createTextNode(ui->inputTransform_transformId_lineEdit->text());

        QDomText inputTransform_inverseOutputTransform_aces_description_text = doc.createTextNode(ui->inverseOutputTransform_description_textEdit->toPlainText());
        QDomText inputTransform_inverseOutputTransform_aces_hash_text = doc.createTextNode(ui->inverseOutputTransform_hash_lineEdit->text());
        QDomText inputTransform_inverseOutputTransform_aces_uuid_text = doc.createTextNode(ui->inverseOutputTransform_uuid_lineEdit->text());
        QDomText inputTransform_inverseOutputTransform_aces_file_text = doc.createTextNode(ui->inverseOutputTransform_file_lineEdit->text());
        QDomText inputTransform_inverseOutputTransform_aces_transformId_text = doc.createTextNode(ui->inverseOutputTransform_transformId_lineEdit->text());

        QDomText inputTransform_inverseOutputDeviceTransform_aces_description_text = doc.createTextNode(ui->inverseOutputDeviceTransform_description_textEdit->toPlainText());
        QDomText inputTransform_inverseOutputDeviceTransform_aces_hash_text = doc.createTextNode(ui->inverseOutputDeviceTransform_hash_lineEdit->text());
        QDomText inputTransform_inverseOutputDeviceTransform_aces_uuid_text = doc.createTextNode(ui->inverseOutputDeviceTransform_uuid_lineEdit->text());
        QDomText inputTransform_inverseOutputDeviceTransform_aces_file_text = doc.createTextNode(ui->inverseOutputDeviceTransform_file_lineEdit->text());
        QDomText inputTransform_inverseOutputDeviceTransform_aces_transformId_text = doc.createTextNode(ui->inverseOutputDeviceTransform_transformId_lineEdit->text());

        QDomText inputTransform_inverseReferenceRenderingTransform_aces_description_text = doc.createTextNode(ui->inverseReferenceRenderingTransform_description_textEdit->toPlainText());
        QDomText inputTransform_inverseReferenceRenderingTransform_aces_hash_text = doc.createTextNode(ui->inverseReferenceRenderingTransform_hash_lineEdit->text());
        QDomText inputTransform_inverseReferenceRenderingTransform_aces_uuid_text = doc.createTextNode(ui->inverseReferenceRenderingTransform_uuid_lineEdit->text());
        QDomText inputTransform_inverseReferenceRenderingTransform_aces_file_text = doc.createTextNode(ui->inverseReferenceRenderingTransform_file_lineEdit->text());
        QDomText inputTransform_inverseReferenceRenderingTransform_aces_transformId_text = doc.createTextNode(ui->inverseReferenceRenderingTransform_transformId_lineEdit->text());

        // lookTransform
        QDomText lookTransform_aces_description_description_text = doc.createTextNode(ui->lookTransform_description_textEdit->toPlainText());
        QDomText lookTransform_aces_hash_text = doc.createTextNode(ui->lookTransform_hash_lineEdit->text());
        QDomText lookTransform_aces_uuid_text = doc.createTextNode(ui->lookTransform_uuid_lineEdit->text());
        QDomText lookTransform_aces_file_text = doc.createTextNode(ui->lookTransform_file_lineEdit->text());
        QDomText lookTransform_aces_transformId_text = doc.createTextNode(ui->lookTransform_transformId_lineEdit->text());

        QDomText toCdlWorkingSpace_aces_description_text = doc.createTextNode(ui->cdlWorkingSpace_toCdlWorkingSpace_description_textEdit->toPlainText());
        QDomText toCdlWorkingSpace_aces_hash_text = doc.createTextNode(ui->cdlWorkingSpace_toCdlWorkingSpace_hash_lineEdit->text());
        QDomText toCdlWorkingSpace_aces_uuid_text = doc.createTextNode(ui->cdlWorkingSpace_toCdlWorkingSpace_uuid_lineEdit->text());
        QDomText toCdlWorkingSpace_aces_file_text = doc.createTextNode(ui->cdlWorkingSpace_toCdlWorkingSpace_file_lineEdit->text());
        QDomText toCdlWorkingSpace_transformId_text = doc.createTextNode(ui->cdlWorkingSpace_toCdlWorkingSpace_transformId_lineEdit->text());

        QDomText fromCdlWorkingSpace_aces_description_text = doc.createTextNode(ui->cdlWorkingSpace_fromCdlWorkingSpace_description_textEdit->toPlainText());
        QDomText fromCdlWorkingSpace_aces_hash_text = doc.createTextNode(ui->cdlWorkingSpace_fromCdlWorkingSpace_hash_lineEdit->text());
        QDomText fromCdlWorkingSpace_aces_uuid_text = doc.createTextNode(ui->cdlWorkingSpace_fromCdlWorkingSpace_uuid_lineEdit->text());
        QDomText fromCdlWorkingSpace_aces_file_text = doc.createTextNode(ui->cdlWorkingSpace_fromCdlWorkingSpace_file_lineEdit->text());
        QDomText fromCdlWorkingSpace_transformId_text = doc.createTextNode(ui->cdlWorkingSpace_fromCdlWorkingSpace_transformId_lineEdit->text());

        QDomText cdl_SOPNode_cdl_Slope_text = doc.createTextNode(ui->Pipeline_RSlope_DoublespinBox->text()+" "+ui->Pipeline_GSlope_DoublespinBox->text()+" "+ui->Pipeline_BSlope_DoublespinBox->text());
        QDomText cdl_SOPNode_cdl_Offset_text = doc.createTextNode(ui->Pipeline_ROffset_DoublespinBox->text()+" "+ui->Pipeline_GOffset_DoublespinBox->text()+" "+ui->Pipeline_BOffset_DoublespinBox->text());
        QDomText cdl_SOPNode_cdl_Power_text = doc.createTextNode(ui->Pipeline_RPower_DoublespinBox->text()+" "+ui->Pipeline_GPower_DoublespinBox->text()+" "+ui->Pipeline_BPower_DoublespinBox->text());
        QDomText lookTransform_cdl_Saturation_text = doc.createTextNode(ui->Pipeline_Saturation_DoublespinBox->text());

        QDomText lookTransform_cdl_ColorCorrectionRef_text = doc.createTextNode(ui->Pipeline_ColorCorrectionRef_lineEdit->text());
        QDomText ColorCorrectionRef_cdl_Description_text = doc.createTextNode(ui->Pipeline_ColorCorrectionRef_description->text());
        QDomText ColorCorrectionRef_cdl_InputDescription_text = doc.createTextNode(ui->Pipeline_ColorCorrectionRef_Inputdescription->text());
        QDomText ColorCorrectionRef_cdl_ViewingDescription_text = doc.createTextNode(ui->Pipeline_ColorCorrectionRef_Viewingdescription->text());

        // outputTransform
        QDomText outputTransform_aces_description_text = doc.createTextNode(ui->outputTransform_description_textEdit->toPlainText());
        QDomText outputTransform_aces_hash_text = doc.createTextNode(ui->outputTransform_hash_lineEdit->text());
        QDomText outputTransform_aces_uuid_text = doc.createTextNode(ui->outputTransform_uuid_lineEdit->text());
        QDomText outputTransform_aces_file_text = doc.createTextNode(ui->outputTransform_file_lineEdit->text());
        QDomText outputTransform_aces_transformId_text = doc.createTextNode(ui->outputTransform_transformId_lineEdit->text());

        QDomText outputDeviceTransform_aces_description_text = doc.createTextNode(ui->outputDeviceTransform_description_textEdit->toPlainText());
        QDomText outputDeviceTransform_aces_hash_text = doc.createTextNode(ui->outputDeviceTransform_hash_lineEdit->text());
        QDomText outputDeviceTransform_aces_uuid_text = doc.createTextNode(ui->outputDeviceTransform_uuid_lineEdit->text());
        QDomText outputDeviceTransform_aces_file_text = doc.createTextNode(ui->outputDeviceTransform_file_lineEdit->text());
        QDomText outputDeviceTransform_aces_transformId_text = doc.createTextNode(ui->outputDeviceTransform_transformId_lineEdit->text());

        QDomText referenceRenderingTransform_aces_description_text = doc.createTextNode(ui->referenceRenderingTransform_description_textEdit->toPlainText());
        QDomText referenceRenderingTransform_aces_hash_text = doc.createTextNode(ui->referenceRenderingTransform_hash_lineEdit->text());
        QDomText referenceRenderingTransform_aces_uuid_text = doc.createTextNode(ui->referenceRenderingTransform_uuid_lineEdit->text());
        QDomText referenceRenderingTransform_aces_file_text = doc.createTextNode(ui->referenceRenderingTransform_file_lineEdit->text());
        QDomText referenceRenderingTransform_aces_transformId_text = doc.createTextNode(ui->referenceRenderingTransform_transformId_lineEdit->text());

        // 赋值与属性
        // info
        pipelineInfo_aces_description.appendChild(pipelineInfo_aces_description_text);

        pipelineInfo_aces_creationDateTime.appendChild(pipelineInfo_aces_creationDateTime_text);
        pipelineInfo_aces_modificationDateTime.appendChild(pipelineInfo_aces_modificationDateTime_text);

        pipelineInfo_aces_uuid.appendChild(pipelineInfo_aces_uuid_text);

        pipelineInfo_aces_majorVersion.appendChild(pipelineInfo_aces_majorVersion_text);
        pipelineInfo_aces_minorVersion.appendChild(pipelineInfo_aces_minorVersion_text);
        pipelineInfo_aces_patchVersion.appendChild(pipelineInfo_aces_patchVersion_text);

        pipelineInfo_aces_name.appendChild(pipelineInfo_aces_name_text);
        pipelineInfo_aces_emailAddress.appendChild(pipelineInfo_aces_emailAddress_text);

        /// TODO: 每一个转换都涉及到Hash值都问题 但是每一个转换都Hash值算法是不一样的
        /// 需要设计一个让用户自己选择的窗口，目前的做法是让新的AMF文件直接使用老的或是默认的Hash

        // inputTransform
        if(ui->inputTransform_applied_checkBox->isChecked())
        {
            pipeline_inputTransform.setAttribute("applied","true");
        }
        else if(!(ui->inputTransform_applied_checkBox->isChecked()))
        {
            pipeline_inputTransform.setAttribute("applied","false");
        }
        else
        {pipeline_inputTransform.setAttribute("applied","false");};

        inputTransform_aces_description.appendChild(inputTransform_aces_description_text);
        inputTransform_aces_hash.appendChild(inputTransform_aces_hash_text);
        inputTransform_aces_uuid.appendChild(inputTransform_aces_uuid_text);
        inputTransform_aces_file.appendChild(inputTransform_aces_file_text);
        inputTransform_aces_transformId.appendChild(inputTransform_aces_transformId_text);

        inputTransform_inverseOutputTransform_aces_description.appendChild(inputTransform_inverseOutputTransform_aces_description_text);
        inputTransform_inverseOutputTransform_aces_hash.appendChild(inputTransform_inverseOutputTransform_aces_hash_text);
        inputTransform_inverseOutputTransform_aces_uuid.appendChild(inputTransform_inverseOutputTransform_aces_uuid_text);
        inputTransform_inverseOutputTransform_aces_file.appendChild(inputTransform_inverseOutputTransform_aces_file_text);
        inputTransform_inverseOutputTransform_aces_transformId.appendChild(inputTransform_inverseOutputTransform_aces_transformId_text);

        inputTransform_inverseOutputDeviceTransform_aces_description.appendChild(inputTransform_inverseOutputDeviceTransform_aces_description_text);
        inputTransform_inverseOutputDeviceTransform_aces_hash.appendChild(inputTransform_inverseOutputDeviceTransform_aces_hash_text);
        inputTransform_inverseOutputDeviceTransform_aces_uuid.appendChild(inputTransform_inverseOutputDeviceTransform_aces_uuid_text);
        inputTransform_inverseOutputDeviceTransform_aces_file.appendChild(inputTransform_inverseOutputDeviceTransform_aces_file_text);
        inputTransform_inverseOutputDeviceTransform_aces_transformId.appendChild(inputTransform_inverseOutputDeviceTransform_aces_transformId_text);

        inputTransform_inverseReferenceRenderingTransform_aces_description.appendChild(inputTransform_inverseReferenceRenderingTransform_aces_description_text);
        inputTransform_inverseReferenceRenderingTransform_aces_hash.appendChild(inputTransform_inverseReferenceRenderingTransform_aces_hash_text);
        inputTransform_inverseReferenceRenderingTransform_aces_uuid.appendChild(inputTransform_inverseReferenceRenderingTransform_aces_uuid_text);
        inputTransform_inverseReferenceRenderingTransform_aces_file.appendChild(inputTransform_inverseReferenceRenderingTransform_aces_file_text);
        inputTransform_inverseReferenceRenderingTransform_aces_transformId.appendChild(inputTransform_inverseReferenceRenderingTransform_aces_transformId_text);

        // lookTransform
        if(ui->lookTransform_applied_checkBox->isChecked())
        {
            pipeline_lookTransform.setAttribute("applied","true");
        }
        else if(!(ui->lookTransform_applied_checkBox->isChecked()))
        {
            pipeline_lookTransform.setAttribute("applied","false");
        }
        else
        {pipeline_lookTransform.setAttribute("applied","false");};

        lookTransform_aces_description.appendChild(lookTransform_aces_description_description_text);
        lookTransform_aces_hash.appendChild(lookTransform_aces_hash_text);
        lookTransform_aces_uuid.appendChild(lookTransform_aces_uuid_text);
        lookTransform_aces_file.appendChild(lookTransform_aces_file_text);
        lookTransform_aces_transformId.appendChild(lookTransform_aces_transformId_text);

        toCdlWorkingSpace_aces_description.appendChild(toCdlWorkingSpace_aces_description_text);
        toCdlWorkingSpace_aces_hash.appendChild(toCdlWorkingSpace_aces_hash_text);
        toCdlWorkingSpace_aces_uuid.appendChild(toCdlWorkingSpace_aces_uuid_text);
        toCdlWorkingSpace_aces_file.appendChild(toCdlWorkingSpace_aces_file_text);
        toCdlWorkingSpace_aces_transformId.appendChild(toCdlWorkingSpace_transformId_text);

        fromCdlWorkingSpace_aces_description.appendChild(fromCdlWorkingSpace_aces_description_text);
        fromCdlWorkingSpace_aces_hash.appendChild(fromCdlWorkingSpace_aces_hash_text);
        fromCdlWorkingSpace_aces_uuid.appendChild(fromCdlWorkingSpace_aces_uuid_text);
        fromCdlWorkingSpace_aces_file.appendChild(fromCdlWorkingSpace_aces_file_text);
        fromCdlWorkingSpace_aces_transformId.appendChild(fromCdlWorkingSpace_transformId_text);

        cdl_SOPNode_cdl_Slope.appendChild(cdl_SOPNode_cdl_Slope_text);
        cdl_SOPNode_cdl_Offset.appendChild(cdl_SOPNode_cdl_Offset_text);
        cdl_SOPNode_cdl_Power.appendChild(cdl_SOPNode_cdl_Power_text);

        lookTransform_cdl_Saturation.appendChild(lookTransform_cdl_Saturation_text);

        /// TODO:See the ASC-CDL documentation for more information on the usage of this node.
        lookTransform_cdl_ColorCorrectionRef.appendChild(lookTransform_cdl_ColorCorrectionRef_text);
        ColorCorrectionRef_cdl_Description.appendChild(ColorCorrectionRef_cdl_Description_text);
        ColorCorrectionRef_cdl_InputDescription.appendChild(ColorCorrectionRef_cdl_InputDescription_text);
        ColorCorrectionRef_cdl_ViewingDescription.appendChild(ColorCorrectionRef_cdl_ViewingDescription_text);

        // outputTransform
        outputTransform_aces_description.appendChild(outputTransform_aces_description_text);
        outputTransform_aces_hash.appendChild(outputTransform_aces_hash_text);
        outputTransform_aces_uuid.appendChild(outputTransform_aces_uuid_text);
        outputTransform_aces_file.appendChild(outputTransform_aces_file_text);
        outputTransform_aces_transformId.appendChild(outputTransform_aces_transformId_text);

        outputDeviceTransform_aces_description.appendChild(outputDeviceTransform_aces_description_text);
        outputDeviceTransform_aces_hash.appendChild(outputDeviceTransform_aces_hash_text);
        outputDeviceTransform_aces_uuid.appendChild(outputDeviceTransform_aces_uuid_text);
        outputDeviceTransform_aces_file.appendChild(outputDeviceTransform_aces_file_text);
        outputDeviceTransform_aces_transformId.appendChild(outputDeviceTransform_aces_transformId_text);

        referenceRenderingTransform_aces_description.appendChild(referenceRenderingTransform_aces_description_text);
        referenceRenderingTransform_aces_hash.appendChild(referenceRenderingTransform_aces_hash_text);
        referenceRenderingTransform_aces_uuid.appendChild(referenceRenderingTransform_aces_uuid_text);
        referenceRenderingTransform_aces_file.appendChild(referenceRenderingTransform_aces_file_text);
        referenceRenderingTransform_aces_transformId.appendChild(referenceRenderingTransform_aces_transformId_text);

        // 编辑从属
        // pipelineInfo
        aces_pipeline.appendChild(pipeline_pipelineInfo);

        if(!ui->ANALYSISPanel_Pipeline_Info_textEdit->toPlainText().isEmpty()){pipeline_pipelineInfo.appendChild(pipelineInfo_aces_description);}

        pipeline_pipelineInfo.appendChild(pipelineInfo_aces_author);
        pipelineInfo_aces_author.appendChild(pipelineInfo_aces_name);
        pipelineInfo_aces_author.appendChild(pipelineInfo_aces_emailAddress);

        pipeline_pipelineInfo.appendChild(pipelineInfo_aces_dateTime);
        pipelineInfo_aces_dateTime.appendChild(pipelineInfo_aces_creationDateTime);
        pipelineInfo_aces_dateTime.appendChild(pipelineInfo_aces_modificationDateTime);

        if(!ui->ANALYSISPanel_Pipeline_Info_UUID_lineEdit->text().isEmpty()){pipeline_pipelineInfo.appendChild(pipelineInfo_aces_uuid);}
        pipeline_pipelineInfo.appendChild(pipelineInfo_aces_systemVersion);
        pipelineInfo_aces_systemVersion.appendChild(pipelineInfo_aces_majorVersion);
        pipelineInfo_aces_systemVersion.appendChild(pipelineInfo_aces_minorVersion);
        pipelineInfo_aces_systemVersion.appendChild(pipelineInfo_aces_patchVersion);

        // inputTransform
        aces_pipeline.appendChild(pipeline_inputTransform);
        pipeline_inputTransform.appendChild(inputTransform_aces_description);

        if(!ui->inputTransform_hash_lineEdit->text().isEmpty())
        {
            pipeline_inputTransform.appendChild(inputTransform_aces_hash);
            inputTransform_aces_hash.setAttribute("algorithm",QString::fromStdString(this->currentamf->Get_inputTransform_hash_algorithm()));
        }
        if(!ui->inputTransform_uuid_lineEdit->text().isEmpty()){pipeline_inputTransform.appendChild(inputTransform_aces_uuid);}
        if(!ui->inputTransform_file_lineEdit->text().isEmpty()){pipeline_inputTransform.appendChild(inputTransform_aces_file);}
        pipeline_inputTransform.appendChild(inputTransform_aces_transformId);

        // inverseOutputTransform
        if((!ui->inverseOutputTransform_description_textEdit->toPlainText().isEmpty())
                ||(!ui->inverseOutputTransform_hash_lineEdit->text().isEmpty())
                ||(!ui->inverseOutputTransform_transformId_lineEdit->text().isEmpty())
                ||(!ui->inverseOutputTransform_uuid_lineEdit->text().isEmpty())
                ||(!ui->inverseOutputTransform_file_lineEdit->text().isEmpty()))
            {
                pipeline_inputTransform.appendChild(inputTransform_inverseOutputTransform);
                inputTransform_inverseOutputTransform.appendChild(inputTransform_inverseOutputTransform_aces_description);

                if(!ui->inverseOutputTransform_hash_lineEdit->text().isEmpty())
                {
                    inputTransform_inverseOutputTransform.appendChild(inputTransform_inverseOutputTransform_aces_hash);
                    inputTransform_inverseOutputTransform_aces_hash.setAttribute("algorithm",QString::fromStdString(this->currentamf->Get_inverseOutputTransform_hash_algorithm()));
                }
                if(!ui->inverseOutputTransform_uuid_lineEdit->text().isEmpty()){inputTransform_inverseOutputTransform.appendChild(inputTransform_inverseOutputTransform_aces_uuid);}
                if(!ui->inverseOutputTransform_file_lineEdit->text().isEmpty()){inputTransform_inverseOutputTransform.appendChild(inputTransform_inverseOutputTransform_aces_file);}
                inputTransform_inverseOutputTransform.appendChild(inputTransform_inverseOutputTransform_aces_transformId);
            }


        // inverseOutputDeviceTransform
        if((!ui->inverseOutputDeviceTransform_description_textEdit->toPlainText().isEmpty())
                ||(!ui->inverseOutputDeviceTransform_hash_lineEdit->text().isEmpty())
                ||(!ui->inverseOutputDeviceTransform_transformId_lineEdit->text().isEmpty())
                ||(!ui->inverseOutputDeviceTransform_uuid_lineEdit->text().isEmpty())
                ||(!ui->inverseOutputDeviceTransform_file_lineEdit->text().isEmpty()))
            {
                pipeline_inputTransform.appendChild(inputTransform_inverseOutputDeviceTransform);
                inputTransform_inverseOutputDeviceTransform.appendChild(inputTransform_inverseOutputDeviceTransform_aces_description);

                if(!ui->inverseOutputDeviceTransform_hash_lineEdit->text().isEmpty())
                {
                    inputTransform_inverseOutputDeviceTransform.appendChild(inputTransform_inverseOutputDeviceTransform_aces_hash);
                    inputTransform_inverseOutputDeviceTransform_aces_hash.setAttribute("algorithm",QString::fromStdString(this->currentamf->Get_inverseOutputDeviceTransform_hash_algorithm()));
                }
                if(!ui->inverseOutputDeviceTransform_uuid_lineEdit->text().isEmpty()){inputTransform_inverseOutputDeviceTransform.appendChild(inputTransform_inverseOutputDeviceTransform_aces_uuid);}
                if(!ui->inverseOutputDeviceTransform_file_lineEdit->text().isEmpty()){inputTransform_inverseOutputDeviceTransform.appendChild(inputTransform_inverseOutputDeviceTransform_aces_file);}
                inputTransform_inverseOutputDeviceTransform.appendChild(inputTransform_inverseOutputDeviceTransform_aces_transformId);
            }


        // inverseReferenceRenderingTransform
        if((!ui->inverseReferenceRenderingTransform_description_textEdit->toPlainText().isEmpty())
                ||(!ui->inverseReferenceRenderingTransform_hash_lineEdit->text().isEmpty())
                ||(!ui->inverseReferenceRenderingTransform_transformId_lineEdit->text().isEmpty())
                ||(!ui->inverseReferenceRenderingTransform_uuid_lineEdit->text().isEmpty())
                ||(!ui->inverseReferenceRenderingTransform_file_lineEdit->text().isEmpty()))
            {
                pipeline_inputTransform.appendChild(inputTransform_inverseReferenceRenderingTransform);
                inputTransform_inverseReferenceRenderingTransform.appendChild(inputTransform_inverseReferenceRenderingTransform_aces_description);

                if(!ui->inverseReferenceRenderingTransform_hash_lineEdit->text().isEmpty())
                {
                    inputTransform_inverseReferenceRenderingTransform.appendChild(inputTransform_inverseReferenceRenderingTransform_aces_hash);
                    inputTransform_inverseReferenceRenderingTransform_aces_hash.setAttribute("algorithm",QString::fromStdString(this->currentamf->Get_inverseReferenceRenderingTransform_hash_algorithm()));
                }
                if(!ui->inverseReferenceRenderingTransform_uuid_lineEdit->text().isEmpty()){inputTransform_inverseReferenceRenderingTransform.appendChild(inputTransform_inverseReferenceRenderingTransform_aces_uuid);}
                if(!ui->inverseReferenceRenderingTransform_file_lineEdit->text().isEmpty()){inputTransform_inverseReferenceRenderingTransform.appendChild(inputTransform_inverseReferenceRenderingTransform_aces_file);}
                inputTransform_inverseReferenceRenderingTransform.appendChild(inputTransform_inverseReferenceRenderingTransform_aces_transformId);
            }


        // lookTransform
        if(ui->lookTransform_applied_checkBox->isChecked())                                         // 如果applied的值是true则会将LMT载入
        {
            if((!ui->lookTransform_description_textEdit->toPlainText().isEmpty())
                ||(!ui->lookTransform_hash_lineEdit->text().isEmpty())
                ||(!ui->lookTransform_transformId_lineEdit->text().isEmpty())
                ||(!ui->lookTransform_uuid_lineEdit->text().isEmpty())
                ||(!ui->lookTransform_file_lineEdit->text().isEmpty())
                ||ui->lookTransform_applied_checkBox->isChecked())
            {
                    aces_pipeline.appendChild(pipeline_lookTransform);
                    pipeline_lookTransform.appendChild(lookTransform_aces_description);

                    if(!ui->lookTransform_hash_lineEdit->text().isEmpty())
                    {
                        pipeline_lookTransform.appendChild(lookTransform_aces_hash);
                        lookTransform_aces_hash.setAttribute("algorithm",QString::fromStdString(this->currentamf->Get_lookTransform_hash_algorithm()));
                    }
                    if(!ui->lookTransform_uuid_lineEdit->text().isEmpty()){pipeline_lookTransform.appendChild(lookTransform_aces_uuid);}
                    if(!ui->lookTransform_file_lineEdit->text().isEmpty()){pipeline_lookTransform.appendChild(lookTransform_aces_file);}
                    pipeline_lookTransform.appendChild(lookTransform_aces_transformId);
            }

            // toCdlWorkingSpace
            if((!ui->cdlWorkingSpace_toCdlWorkingSpace_description_textEdit->toPlainText().isEmpty())
                ||(!ui->cdlWorkingSpace_toCdlWorkingSpace_hash_lineEdit->text().isEmpty())
                ||(!ui->cdlWorkingSpace_toCdlWorkingSpace_transformId_lineEdit->text().isEmpty())
                ||(!ui->cdlWorkingSpace_toCdlWorkingSpace_uuid_lineEdit->text().isEmpty())
                ||(!ui->cdlWorkingSpace_toCdlWorkingSpace_file_lineEdit->text().isEmpty()))
            {
                pipeline_lookTransform.appendChild(lookTransform_toCdlWorkingSpace);
                lookTransform_toCdlWorkingSpace.appendChild(toCdlWorkingSpace_aces_description);

                if(!ui->cdlWorkingSpace_toCdlWorkingSpace_hash_lineEdit->text().isEmpty())
                {
                    lookTransform_toCdlWorkingSpace.appendChild(toCdlWorkingSpace_aces_hash);
                    toCdlWorkingSpace_aces_hash.setAttribute("algorithm",QString::fromStdString(this->currentamf->Get_lookTransform_hash_algorithm()));
                }
                if(!ui->cdlWorkingSpace_toCdlWorkingSpace_uuid_lineEdit->text().isEmpty()){lookTransform_toCdlWorkingSpace.appendChild(toCdlWorkingSpace_aces_uuid);}
                if(!ui->cdlWorkingSpace_toCdlWorkingSpace_file_lineEdit->text().isEmpty()){lookTransform_toCdlWorkingSpace.appendChild(toCdlWorkingSpace_aces_file);}
                lookTransform_toCdlWorkingSpace.appendChild(toCdlWorkingSpace_aces_transformId);
            }

            // fromCdlWorkingSpace
            if((!ui->cdlWorkingSpace_fromCdlWorkingSpace_description_textEdit->toPlainText().isEmpty())
                ||(!ui->cdlWorkingSpace_fromCdlWorkingSpace_hash_lineEdit->text().isEmpty())
                ||(!ui->cdlWorkingSpace_fromCdlWorkingSpace_transformId_lineEdit->text().isEmpty())
                ||(!ui->cdlWorkingSpace_fromCdlWorkingSpace_uuid_lineEdit->text().isEmpty())
                ||(!ui->cdlWorkingSpace_fromCdlWorkingSpace_file_lineEdit->text().isEmpty()))
            {
                pipeline_lookTransform.appendChild(lookTransform_fromCdlWorkingSpace);
                lookTransform_fromCdlWorkingSpace.appendChild(fromCdlWorkingSpace_aces_description);

                if(!ui->cdlWorkingSpace_fromCdlWorkingSpace_hash_lineEdit->text().isEmpty())
                {
                    lookTransform_fromCdlWorkingSpace.appendChild(fromCdlWorkingSpace_aces_hash);
                    fromCdlWorkingSpace_aces_hash.setAttribute("algorithm",QString::fromStdString(this->currentamf->Get_lookTransform_hash_algorithm()));
                }
                if(!ui->cdlWorkingSpace_fromCdlWorkingSpace_uuid_lineEdit->text().isEmpty()){lookTransform_fromCdlWorkingSpace.appendChild(fromCdlWorkingSpace_aces_uuid);}
                if(!ui->cdlWorkingSpace_fromCdlWorkingSpace_file_lineEdit->text().isEmpty()){lookTransform_fromCdlWorkingSpace.appendChild(fromCdlWorkingSpace_aces_file);}
                lookTransform_fromCdlWorkingSpace.appendChild(fromCdlWorkingSpace_aces_transformId);
            }

            // cdl part
            if((!ui->cdlWorkingSpace_toCdlWorkingSpace_description_textEdit->toPlainText().isEmpty())               // 如果toCdlWorkingSpace或是fromCdlWorkingSpace或是CDL中任何个一个值不等于起始值
                    ||(!ui->cdlWorkingSpace_toCdlWorkingSpace_hash_lineEdit->text().isEmpty())                      // 就会把cdl:SOPNode和cdl:SatNode以及cdl:ColorCorrectionRef（选择性）节点添加到lookTransform的后面
                    ||(!ui->cdlWorkingSpace_toCdlWorkingSpace_transformId_lineEdit->text().isEmpty())
                    ||(!ui->cdlWorkingSpace_toCdlWorkingSpace_uuid_lineEdit->text().isEmpty())
                    ||(!ui->cdlWorkingSpace_toCdlWorkingSpace_file_lineEdit->text().isEmpty())

                    ||(!ui->cdlWorkingSpace_fromCdlWorkingSpace_description_textEdit->toPlainText().isEmpty())
                    ||(!ui->cdlWorkingSpace_fromCdlWorkingSpace_hash_lineEdit->text().isEmpty())
                    ||(!ui->cdlWorkingSpace_fromCdlWorkingSpace_transformId_lineEdit->text().isEmpty())
                    ||(!ui->cdlWorkingSpace_fromCdlWorkingSpace_uuid_lineEdit->text().isEmpty())
                    ||(!ui->cdlWorkingSpace_fromCdlWorkingSpace_file_lineEdit->text().isEmpty())

                    ||(ui->Pipeline_RSlope_horizontalSlider->value()!=1000)
                    ||(ui->Pipeline_GSlope_horizontalSlider->value()!=1000)
                    ||(ui->Pipeline_BSlope_horizontalSlider->value()!=1000)
                    ||(ui->Pipeline_ROffset_horizontalSlider->value()!=0)
                    ||(ui->Pipeline_GOffset_horizontalSlider->value()!=0)
                    ||(ui->Pipeline_BOffset_horizontalSlider->value()!=0)
                    ||(ui->Pipeline_RPower_horizontalSlider->value()!=1000)
                    ||(ui->Pipeline_GPower_horizontalSlider->value()!=1000)
                    ||(ui->Pipeline_BPower_horizontalSlider->value()!=1000)
                    ||(ui->Pipeline_Saturation_horizontalSlider->value()!=1000)

                    ||(!ui->Pipeline_ColorCorrectionRef_lineEdit->text().isEmpty()))
            {
                pipeline_lookTransform.appendChild(lookTransform_cdl_SOPNode);
                lookTransform_cdl_SOPNode.appendChild(cdl_SOPNode_cdl_Slope);
                lookTransform_cdl_SOPNode.appendChild(cdl_SOPNode_cdl_Offset);
                lookTransform_cdl_SOPNode.appendChild(cdl_SOPNode_cdl_Power);

                pipeline_lookTransform.appendChild(lookTransform_cdl_SATNode);
                lookTransform_cdl_SATNode.appendChild(lookTransform_cdl_Saturation);

                if((!ui->Pipeline_ColorCorrectionRef_description->text().isEmpty())
                        ||(!ui->Pipeline_ColorCorrectionRef_description->text().isEmpty())
                        ||(!ui->Pipeline_ColorCorrectionRef_Inputdescription->text().isEmpty())
                        ||(!ui->Pipeline_ColorCorrectionRef_Viewingdescription->text().isEmpty())){
                    pipeline_lookTransform.appendChild(lookTransform_cdl_ColorCorrectionRef);
                    lookTransform_cdl_ColorCorrectionRef.setAttribute("ref",QString::fromStdString(this->currentamf->Get_ColorCorrectionRef_ref()));

                    if(!ui->Pipeline_ColorCorrectionRef_description->text().isEmpty()){lookTransform_cdl_ColorCorrectionRef.appendChild(ColorCorrectionRef_cdl_Description);}
                    if(!ui->Pipeline_ColorCorrectionRef_Inputdescription->text().isEmpty()){lookTransform_cdl_ColorCorrectionRef.appendChild(ColorCorrectionRef_cdl_InputDescription);}
                    if(!ui->Pipeline_ColorCorrectionRef_Viewingdescription->text().isEmpty()){lookTransform_cdl_ColorCorrectionRef.appendChild(ColorCorrectionRef_cdl_ViewingDescription);}
                }
            }
        }

        // OutputTransform
        aces_pipeline.appendChild(pipeline_outputTransform);

        if((!ui->outputTransform_description_textEdit->toPlainText().isEmpty())
                ||(!ui->outputTransform_hash_lineEdit->text().isEmpty())
                ||(!ui->outputTransform_transformId_lineEdit->text().isEmpty())
                ||(!ui->outputTransform_uuid_lineEdit->text().isEmpty())
                ||(!ui->outputTransform_file_lineEdit->text().isEmpty()))
        {
        pipeline_outputTransform.appendChild(outputTransform_aces_description);
        if(!ui->outputTransform_hash_lineEdit->text().isEmpty())
        {
            pipeline_outputTransform.appendChild(outputTransform_aces_hash);
            outputTransform_aces_hash.setAttribute("algorithm",QString::fromStdString(this->currentamf->Get_outputTransform_hash_algorithm()));
        }
        if(!ui->outputTransform_uuid_lineEdit->text().isEmpty()){pipeline_outputTransform.appendChild(outputTransform_aces_uuid);}
        if(!ui->outputTransform_file_lineEdit->text().isEmpty()){pipeline_outputTransform.appendChild(outputTransform_aces_file);}
        pipeline_outputTransform.appendChild(outputTransform_aces_transformId);
        }

        // referenceRenderingTransform
        if((!ui->referenceRenderingTransform_description_textEdit->toPlainText().isEmpty())
                ||(!ui->referenceRenderingTransform_hash_lineEdit->text().isEmpty())
                ||(!ui->referenceRenderingTransform_transformId_lineEdit->text().isEmpty())
                ||(!ui->referenceRenderingTransform_uuid_lineEdit->text().isEmpty())
                ||(!ui->referenceRenderingTransform_file_lineEdit->text().isEmpty()))
            {
                pipeline_outputTransform.appendChild(outputTransform_referenceRenderingTransform);
                outputTransform_referenceRenderingTransform.appendChild(referenceRenderingTransform_aces_description);

                if(!ui->referenceRenderingTransform_hash_lineEdit->text().isEmpty())
                {
                    outputTransform_referenceRenderingTransform.appendChild(referenceRenderingTransform_aces_hash);
                    outputTransform_referenceRenderingTransform.setAttribute("algorithm",QString::fromStdString(this->currentamf->Get_referenceRenderingTransform_hash_algorithm()));
                }
                if(!ui->referenceRenderingTransform_uuid_lineEdit->text().isEmpty()){outputTransform_referenceRenderingTransform.appendChild(referenceRenderingTransform_aces_uuid);}
                if(!ui->referenceRenderingTransform_file_lineEdit->text().isEmpty()){outputTransform_referenceRenderingTransform.appendChild(referenceRenderingTransform_aces_file);}
                outputTransform_referenceRenderingTransform.appendChild(referenceRenderingTransform_aces_transformId);
            };

        // outputDeviceTransform
        if((!ui->outputDeviceTransform_description_textEdit->toPlainText().isEmpty())
                ||(!ui->outputDeviceTransform_hash_lineEdit->text().isEmpty())
                ||(!ui->outputDeviceTransform_transformId_lineEdit->text().isEmpty())
                ||(!ui->outputDeviceTransform_uuid_lineEdit->text().isEmpty())
                ||(!ui->outputDeviceTransform_file_lineEdit->text().isEmpty()))
            {
                pipeline_outputTransform.appendChild(outputTransform_outputDeviceTransform);
                outputTransform_outputDeviceTransform.appendChild(outputDeviceTransform_aces_description);

                if(!ui->outputDeviceTransform_hash_lineEdit->text().isEmpty())
                {
                    outputTransform_outputDeviceTransform.appendChild(outputDeviceTransform_aces_hash);
                    outputDeviceTransform_aces_hash.setAttribute("algorithm",QString::fromStdString(this->currentamf->Get_outputDeviceTransform_hash_algorithm()));
                }
                if(!ui->outputDeviceTransform_uuid_lineEdit->text().isEmpty()){outputTransform_outputDeviceTransform.appendChild(outputDeviceTransform_aces_uuid);}
                if(!ui->outputDeviceTransform_file_lineEdit->text().isEmpty()){outputTransform_outputDeviceTransform.appendChild(outputDeviceTransform_aces_file);}
                outputTransform_outputDeviceTransform.appendChild(outputDeviceTransform_aces_transformId);
            };

        // 输出部分 获取名字
        if(ui->SaveFileName_label_lineEdit->text().isEmpty()){
            Getuser_AMFOutputAMFName();
        }

        QString newpath = this->Get_OutputAMFPath() + "/" + ui->SaveFileName_label_lineEdit->text() +".amf";

        QTextStream out(&file);
        doc.save(out, 4);                   // 将文档保存到文件，4为子元素缩进字符数
        file.copy("test.xml",newpath);
        file.close();
        qSetGlobalQHashSeed(-1);
    }
}

void amfanalyst::Creation_LUT()
{
    if(ui->OutputPanel_IDT_pushButton->isChecked()&&ui->OutputPanel_LMT_pushButton->isChecked()&&ui->OutputPanel_ODT_pushButton->isChecked()){
        this->Creation_CombinedLUT();
    }
}

void amfanalyst::Creation_CombinedLUT()
{

    std::cout << "调用Creation_CombinedLUT()" << std::endl << std::endl;

    this->Creation_OutputAMF();         // 先根据输出路径输出一个AMF文件
    QString outputamf_temporary;
    outputamf_temporary = this->Get_OutputAMFPath() + "/" + ui->SaveFileName_label_lineEdit->text() +".amf";            // 获取到输出文件的路径
    QString outputLutPic_temporary;
    outputLutPic_temporary = this->Get_OutputAMFPath() + "/" + ui->SaveFileName_label_lineEdit->text() +".tif";

    QStringList LUTPic_Render_paras;                                                                                    // 获取CTLrender的参数
    ParseAMF ParseAMF_Creation_CombinedLUT_Invoking(outputamf_temporary);
    ctlrendertool ctlrendertool_Creation_CombinedLUT_Invoking;
    LUTPic_Render_paras=ctlrendertool_Creation_CombinedLUT_Invoking.usingctlrender_forLut(ParseAMF_Creation_CombinedLUT_Invoking);

    for(int i = 0; i< LUTPic_Render_paras.size();++i)                                                                   // 输出渲染参数
    {
        std::string tmp = LUTPic_Render_paras.at(i).toStdString();
        std::cout <<i<<":"<<tmp << std::endl;
    }

    int LUT_size;
    std::string User_selected_LUT_size_2_Loction;
    if(ui->OUTPUTPanel_LUTSize_ComboBox->currentText()=="Small(16*16*16)"){
        User_selected_LUT_size_2_Loction = ParseAMF_Creation_CombinedLUT_Invoking.Get_LUT_Pic_16();
        LUT_size = 16;
    }
    else if(ui->OUTPUTPanel_LUTSize_ComboBox->currentText()=="Normal(32*32*32)"){
        User_selected_LUT_size_2_Loction = ParseAMF_Creation_CombinedLUT_Invoking.Get_LUT_Pic_Origin();
        LUT_size = 32;
    }
    else if(ui->OUTPUTPanel_LUTSize_ComboBox->currentText()=="Great(64*64*64)"){
        User_selected_LUT_size_2_Loction = ParseAMF_Creation_CombinedLUT_Invoking.Get_LUT_Pic_64();
        LUT_size = 64;
    }
    else{
        User_selected_LUT_size_2_Loction = ParseAMF_Creation_CombinedLUT_Invoking.Get_LUT_Pic_Origin();
        LUT_size = 32;
    }


    ctlrendertool_Creation_CombinedLUT_Invoking.usingctlrender(LUTPic_Render_paras,User_selected_LUT_size_2_Loction,outputLutPic_temporary.toStdString());    // 使用参数输出LUT图
    std::cout << "输出Pic_Lut图片完成" << std::endl << std::endl;

    ocioutil ocioutil_Creation_CombinedLUT_Invoking;                                                                                                                //调用OCIO对LUT中的内容进行提取
    QString Output_LUT_Location = this->Get_OutputAMFPath() + "/" + ui->SaveFileName_label_lineEdit->text();                                                        // 使用OCIOutil提取LUT图上的信息,并输出到界面上选择的输出路径
    ocioutil_Creation_CombinedLUT_Invoking.ociolutimage_extract_CLI(LUT_size,outputLutPic_temporary.toStdString(),Output_LUT_Location.toStdString());


    if(ui->OUTPUTPanel_LUTFormat_ComboBox->currentText()=="flame (.3dl)"){                                                                                          // 输出为不同的格式
        QString Output_LUT_Location_trans = this->Get_OutputAMFPath() + "/" + ui->SaveFileName_label_lineEdit->text() +".3dl";
        ocioutil_Creation_CombinedLUT_Invoking.ocioociobakelut_TransLutFormat_CLI(Output_LUT_Location.toStdString(), "flame",LUT_size, Output_LUT_Location_trans.toStdString());
        std::string spi3dlut_Location =Output_LUT_Location.toStdString()+"_LUT.spi3d";
        remove(spi3dlut_Location.c_str());
    }
    if(ui->OUTPUTPanel_LUTFormat_ComboBox->currentText()=="lustre (.3dl)"){
        QString Output_LUT_Location_trans = this->Get_OutputAMFPath() + "/" + ui->SaveFileName_label_lineEdit->text() +".3dl";
        ocioutil_Creation_CombinedLUT_Invoking.ocioociobakelut_TransLutFormat_CLI(Output_LUT_Location.toStdString(), "lustre",LUT_size, Output_LUT_Location_trans.toStdString());
        std::string spi3dlut_Location =Output_LUT_Location.toStdString()+"_LUT.spi3d";
        remove(spi3dlut_Location.c_str());
    }
    if(ui->OUTPUTPanel_LUTFormat_ComboBox->currentText()=="Academy/ASC Common LUT Format (.clf)"){
        QString Output_LUT_Location_trans = this->Get_OutputAMFPath() + "/" + ui->SaveFileName_label_lineEdit->text() +".clf";
        ocioutil_Creation_CombinedLUT_Invoking.ocioociobakelut_TransLutFormat_CLI(Output_LUT_Location.toStdString(), "Academy/ASC' 'Common' 'LUT' 'Format",LUT_size, Output_LUT_Location_trans.toStdString());
        std::string spi3dlut_Location =Output_LUT_Location.toStdString()+"_LUT.spi3d";
        remove(spi3dlut_Location.c_str());
    }
    if(ui->OUTPUTPanel_LUTFormat_ComboBox->currentText()=="Color Transform Format (.ctf)"){
        QString Output_LUT_Location_trans = this->Get_OutputAMFPath() + "/" + ui->SaveFileName_label_lineEdit->text() +".ctf";
        ocioutil_Creation_CombinedLUT_Invoking.ocioociobakelut_TransLutFormat_CLI(Output_LUT_Location.toStdString(), "Color' 'Transform' 'Format",LUT_size, Output_LUT_Location_trans.toStdString());
        std::string spi3dlut_Location =Output_LUT_Location.toStdString()+"_LUT.spi3d";
        remove(spi3dlut_Location.c_str());
    }
    if(ui->OUTPUTPanel_LUTFormat_ComboBox->currentText()=="cinespace (.csp)"){
        QString Output_LUT_Location_trans = this->Get_OutputAMFPath() + "/" + ui->SaveFileName_label_lineEdit->text() +".csp";
        ocioutil_Creation_CombinedLUT_Invoking.ocioociobakelut_TransLutFormat_CLI(Output_LUT_Location.toStdString(), "cinespace",LUT_size, Output_LUT_Location_trans.toStdString());
        std::string spi3dlut_Location =Output_LUT_Location.toStdString()+"_LUT.spi3d";
        remove(spi3dlut_Location.c_str());
    }
    if(ui->OUTPUTPanel_LUTFormat_ComboBox->currentText()=="houdini (.lut)"){
        QString Output_LUT_Location_trans = this->Get_OutputAMFPath() + "/" + ui->SaveFileName_label_lineEdit->text() +".lut";
        ocioutil_Creation_CombinedLUT_Invoking.ocioociobakelut_TransLutFormat_CLI(Output_LUT_Location.toStdString(), "houdini",LUT_size, Output_LUT_Location_trans.toStdString());
        std::string spi3dlut_Location =Output_LUT_Location.toStdString()+"_LUT.spi3d";
        remove(spi3dlut_Location.c_str());
    }
    if(ui->OUTPUTPanel_LUTFormat_ComboBox->currentText()=="iridas_cube (.cube)"){
        QString Output_LUT_Location_trans = this->Get_OutputAMFPath() + "/" + ui->SaveFileName_label_lineEdit->text() +".cube";
        ocioutil_Creation_CombinedLUT_Invoking.ocioociobakelut_TransLutFormat_CLI(Output_LUT_Location.toStdString(), "iridas_cube",LUT_size, Output_LUT_Location_trans.toStdString());
        std::string spi3dlut_Location =Output_LUT_Location.toStdString()+"_LUT.spi3d";
        remove(spi3dlut_Location.c_str());
    }
    if(ui->OUTPUTPanel_LUTFormat_ComboBox->currentText()=="iridas_itx (.itx)"){
        QString Output_LUT_Location_trans = this->Get_OutputAMFPath() + "/" + ui->SaveFileName_label_lineEdit->text() +".itx";
        ocioutil_Creation_CombinedLUT_Invoking.ocioociobakelut_TransLutFormat_CLI(Output_LUT_Location.toStdString(), "iridas_itx",LUT_size, Output_LUT_Location_trans.toStdString());
        std::string spi3dlut_Location =Output_LUT_Location.toStdString()+"_LUT.spi3d";
        remove(spi3dlut_Location.c_str());
    }
    if(ui->OUTPUTPanel_LUTFormat_ComboBox->currentText()=="resolve_cube (.cube)"){
        QString Output_LUT_Location_trans = this->Get_OutputAMFPath() + "/" + ui->SaveFileName_label_lineEdit->text() +".cube";
        ocioutil_Creation_CombinedLUT_Invoking.ocioociobakelut_TransLutFormat_CLI(Output_LUT_Location.toStdString(), "resolve_cube",LUT_size, Output_LUT_Location_trans.toStdString());
        std::string spi3dlut_Location =Output_LUT_Location.toStdString()+"_LUT.spi3d";
        remove(spi3dlut_Location.c_str());
    }
    if(ui->OUTPUTPanel_LUTFormat_ComboBox->currentText()=="truelight (.cub)"){
        QString Output_LUT_Location_trans = this->Get_OutputAMFPath() + "/" + ui->SaveFileName_label_lineEdit->text() +".cub";
        ocioutil_Creation_CombinedLUT_Invoking.ocioociobakelut_TransLutFormat_CLI(Output_LUT_Location.toStdString(), "truelight",LUT_size, Output_LUT_Location_trans.toStdString());
        std::string spi3dlut_Location =Output_LUT_Location.toStdString()+"_LUT.spi3d";
        remove(spi3dlut_Location.c_str());
    }
    if(ui->OUTPUTPanel_LUTFormat_ComboBox->currentText()=="icc (.icc)"){
        QString Output_LUT_Location_trans = this->Get_OutputAMFPath() + "/" + ui->SaveFileName_label_lineEdit->text() +".icc";
        ocioutil_Creation_CombinedLUT_Invoking.ocioociobakelut_TransLutFormat_CLI(Output_LUT_Location.toStdString(), "icc",LUT_size, Output_LUT_Location_trans.toStdString());
        std::string spi3dlut_Location =Output_LUT_Location.toStdString()+"_LUT.spi3d";
        remove(spi3dlut_Location.c_str());
    }

    std::string tiffpic_location = this->Get_OutputAMFPath().toStdString() + "/" + ui->SaveFileName_label_lineEdit->text().toStdString() + ".tif";
    remove(tiffpic_location.c_str());
    remove(outputamf_temporary.toStdString().c_str());
}


void amfanalyst::RefreshPic()
{
    std::cout << "amfanalyst::RefreshPic 被执行" << std::endl;

    std::cout << "第90行读取到的AMF:"<<this->currentamf<< std::endl;
    std::cout << "第91行读取到的路径:"<< this->currentamf->Get_input_picpath() << std::endl;

    QPixmap PictestConnect;

    //如果refresh 按钮的标志是Original的话 需要把它变成渲染后的图片
    if(ui->ANALYSISPanel_Pipeline_Right_Display_Refresh_pushButton->text()=="Original"){
        // 如果图片存在且能够正常读取
        if(isDirExist(QString::fromStdString(this->currentamf->Get_output_picpath())) && PictestConnect.load(QString::fromStdString(this->currentamf->Get_output_picpath()))){
            this->displayPic(this->currentamf->Get_output_picpath());
            ui->ANALYSISPanel_Pipeline_Right_Display_Refresh_pushButton->setText(QString::fromStdString("Rendered"));
        }
        else{
            QMessageBox::information(this,
                                     tr("Failed to read image"),
                                     tr("Failed to read image! \n If you want to display the image of after render, please wait..."));
            return;
        }
    }

    //如果refresh 按钮的标志是Rendered的话 需要把它变成渲染前的图片
    else if(ui->ANALYSISPanel_Pipeline_Right_Display_Refresh_pushButton->text()=="Rendered"){
        if(isDirExist(QString::fromStdString(this->currentamf->Get_input_picpath())) && PictestConnect.load((QString::fromStdString(this->currentamf->Get_input_picpath())))){
            this->displayPic(this->currentamf->Get_input_picpath());
            ui->ANALYSISPanel_Pipeline_Right_Display_Refresh_pushButton->setText(QString::fromStdString("Original"));
        }
        else{
            QMessageBox::information(this,
                                     tr("Failed to read image"),
                                     tr("Failed to read original image!"));
            return;
        }
    }
}

void amfanalyst::displayPic(std::string PicPath)
{

    std::cout << "在显示函数内的路径： "<<PicPath << std::endl;
     QPixmap DispPic;
     if(! ( DispPic.load((QString::fromStdString(PicPath))) )) //加载图像
     {
         QMessageBox::information(this,
                                  tr("Failed to read image"),
                                  tr("Failed to read image! If you want to display the image of after render, please wait..."));
         return;
     }
     DispPic.load((QString::fromStdString(PicPath)));
     QPixmap dest=DispPic.scaled((ui->ANALYSISPanel_Pipeline_Right_Display_Image_tab->size()*0.88),Qt::KeepAspectRatio);
     ui->ANALYSISPanel_Pipeline_Right_Display_Image_QLabel->setPixmap(dest);
}

bool amfanalyst::isDirExist(QString fullPath)
{
    QDir dir(fullPath);
    if(dir.exists(fullPath))
    {
      return true;
    }
    return false;
}

void amfanalyst::displayCube(Q3DScatter *graph)
{
    //实例化Q3DScatter,加入container
//    scatter *modifier = new scatter(graph);
    QWidget *container = QWidget::createWindowContainer(graph);

    //设置窗体大小和布局
    ui->ANALYSISPanel_Pipeline_Right_Display_Cube_tab->layout()->addWidget(container);

    QSlider *pSlider = new QSlider(container);
    pSlider->setOrientation(Qt::Horizontal);  // 水平方向
    pSlider->setMinimum(1);  // 最小值
    pSlider->setMaximum(100);  // 最大值
    pSlider->setSingleStep(1);  // 步长
    ui->ANALYSISPanel_Pipeline_Right_Display_Cube_tab->layout()->addWidget(pSlider);

    scatter *modifier = new scatter(graph,pSlider);

    //输入spi3d,数据不超过1500行,且没有头文件
    qDebug() << "path1" << QString::fromStdString(this->currentamf->Get_LUT_Pic_Origin());
    qDebug() << "path2" << QString::fromStdString(this->currentamf->Get_LUT_Pic_After());


    modifier->getlut2(QString::fromStdString(this->currentamf->Get_LUT_Pic_Origin()),QString::fromStdString(this->currentamf->Get_LUT_Pic_After()));
    //槽函数，滑块拖动的时候将传递给repaint
    QObject::connect(pSlider, &QSlider::valueChanged, modifier,
                     &scatter::repaint);

//    测试多种颜色，需要创建多个scatter，一个scatter一个颜色
//    Q3DScatter *graph2=new Q3DScatter();
//    ui->ANALYSISPanel_Pipeline_Right_Display_Cube_tab->layout()->addWidget(container);
//    QSlider *pSlider2 = new QSlider(container);
//    ui->ANALYSISPanel_Pipeline_Right_Display_Cube_tab->layout()->addWidget(pSlider2);
//    scatter *modifier2 = new scatter(graph,pSlider2);
//    modifier2->getlut("/Users/youdrew/Library/Containers/com.tencent.xinWeChat/Data/Library/Application Support/com.tencent.xinWeChat/2.0b4.0.9/2a8893281ade3e572f48b7cc30a7f2d4/Message/MessageTemp/2a986369e335bcc685896ab36de505c4/File/origin(1).spi3d","/Users/youdrew/Library/Containers/com.tencent.xinWeChat/Data/Library/Application Support/com.tencent.xinWeChat/2.0b4.0.9/2a8893281ade3e572f48b7cc30a7f2d4/Message/MessageTemp/2a986369e335bcc685896ab36de505c4/File/test+2000.spi3d");
//    graph2->seriesList().at(0)->setBaseColor(Qt::blue);
//    graph2->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyle::ColorStyleUniform);


    graph->show();
}

void amfanalyst::RefreshCube()
{
    if (!this->Get_IfhadCUBE()){
    std::cout <<"被调用的LUT是："<< this->currentamf->Get_LUT_Location().toStdString() << std::endl;

    // 显示3D图
    Q3DScatter *graph = new Q3DScatter();

    this->displayCube(graph);

    graph->seriesList().at(0)->setBaseColor(Qt::red);
    graph->seriesList().at(0)->setColorStyle(Q3DTheme::ColorStyle::ColorStyleUniform);


    /// 这里没拿到输出
    displayLinechart(QString::fromStdString(this->currentamf->Get_LUT_Pic_Origin()),QString::fromStdString(this->currentamf->Get_LUT_Pic_After()));

    this->IfhadCUBE=1;
    }
}

void amfanalyst::displayLinechart(QString OriginPIc_LUT, QString AfterPIc_LUT)
{
    m_coordX=0;
    m_coordY=0;
    m_chart=0;
    m_tooltip=0;

    QGraphicsView(new QGraphicsScene, ui->ANALYSISPanel_Pipeline_Right_Display_tabWidget);

    QString path1 = OriginPIc_LUT;
    QString path2 = AfterPIc_LUT;

    // chart
    m_chart = new QChart;
    m_chart->setTitle("This chart is mainly used to show the color tendency of the 1D LUT.");

    QSplineSeries *series = new QSplineSeries;
    QSplineSeries *series2 = new QSplineSeries;
    QSplineSeries *series3 = new QSplineSeries;
    SetSeries(series,series2,series3,path1,path2);

    m_chart->addSeries(series);
    m_chart->addSeries(series2);
    m_chart->addSeries(series3);
    series->setName("R");
    series2->setName("B");
    series3->setName("G");
    series->setColor(QColor(255,0,0));
    series2->setColor(QColor(0,255,0));
    series3->setColor(QColor(0,0,255));

    ui->ANALYSISPANEL_Curve_graphicsView->chart();
    ui->ANALYSISPANEL_Curve_graphicsView->setChart(m_chart);
    ui->ANALYSISPANEL_Curve_graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->ANALYSISPANEL_Curve_graphicsView->show();

    m_coordX = new QGraphicsSimpleTextItem(m_chart);
    m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height());
    m_coordX->setText("X: ");
    m_coordY = new QGraphicsSimpleTextItem(m_chart);
    m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height());
    m_coordY->setText("Y: ");

    connect(series, &QLineSeries::clicked, this, &amfanalyst::keepCallout);
    connect(series, &QLineSeries::hovered, this, &amfanalyst::tooltip);

    connect(series2, &QSplineSeries::clicked, this, &amfanalyst::keepCallout);
    connect(series2, &QSplineSeries::hovered, this, &amfanalyst::tooltip);

    connect(series3, &QSplineSeries::clicked, this, &amfanalyst::keepCallout);
    connect(series3, &QSplineSeries::hovered, this, &amfanalyst::tooltip);


    this->setMouseTracking(true);

}

void amfanalyst::SetSeries(QLineSeries *series1, QLineSeries *series2, QLineSeries *series3, QString path1, QString path2)
{
    cv::Mat img1 =  imread(path1.toStdString(), cv::IMREAD_UNCHANGED);
    cv::Mat img2 =  imread(path2.toStdString(), cv::IMREAD_UNCHANGED);

    for (size_t i = 0; i < 32; i++)
    {
        cv::Vec3f redx = img1.at<cv::Vec3f>(0, i);          //R
        float redxvalue = float(redx[2]);

        cv::Vec4f redy = img2.at<cv::Vec4f>(0, i);
        float redyvalue = float(redy[2]);

        cv::Vec3f bluex = img1.at<cv::Vec3f>(i, 0);         //B
        float bluexvalue = float(bluex[0]);

        cv::Vec4f bluey = img2.at<cv::Vec4f>(i, 0);
        float blueyvalue = float(bluey[0]);

        cv::Vec3f greenx = img1.at<cv::Vec3f>(0, i*32);     //G
        float greenxvalue = float(greenx[1]);

        cv::Vec4f greeny = img2.at<cv::Vec4f>(0, i * 32);
        float greenyvalue = float(greeny[1]);

        series1->append(bluexvalue, blueyvalue);
        series2->append(greenxvalue, greenyvalue);
        series3->append(redxvalue, redyvalue);
    }
}

void amfanalyst::keepCallout()
{
    m_callouts.append(m_tooltip);
    m_tooltip = new Callout(m_chart);
}

void amfanalyst::tooltip(QPointF point, bool state)
{
    if (m_tooltip == 0)
        m_tooltip = new Callout(m_chart);

    if (state) {
        m_tooltip->setText(QString("X: %1 \nY: %2 ").arg(point.x()).arg(point.y()));
        m_tooltip->setAnchor(point);
        m_tooltip->setZValue(11);
        m_tooltip->updateGeometry();
        m_tooltip->show();
    } else {
        m_tooltip->hide();
    }
}

bool amfanalyst::Get_IfOpenAMF()
{
    return IfOpenAMF;
}

bool amfanalyst::Get_IfhadCUBE()
{
    return IfhadCUBE;
}

void amfanalyst::Refresh_Editable_state(int state)
{
    if(ui->Pipeline_Editable_checkBox->isChecked()){
        ui->Pipeline_Editable_checkBox->setText("Eidtable");
        // AMF/Cilps INFO
        ui->ANALYSISPanel_Info_AMFInfo_TextEdit->setEnabled(true);
        ui->ANALYSISPanel_Info_Name_LineEdit->setEnabled(true);
        ui->ANALYSISPanel_Info_EmailAddress_LineEdit->setEnabled(true);
        ui->ANALYSISPanel_Info_AMFUUID_LineEdit->setEnabled(true);

        ui->ANALYSISPanel_Info_CilpID_TextEdit->setEnabled(true);
        ui->ANALYSISPanel_Info_CilpName_LineEdit->setEnabled(true);
        ui->ANALYSISPanel_Info_CilpUUID_LineEdit->setEnabled(true);
        ui->ANALYSISPanel_Info_FilePath_LineEdit->setEnabled(true);
        ui->ANALYSISPanel_Info_Sequence_LineEdit->setEnabled(true);
        ui->idx_lineEdit->setEnabled(true);
        ui->min_spinBox->setEnabled(true);
        ui->max_spinBox->setEnabled(true);

        ui->ANALYSISPanel_Pipeline_Info_textEdit->setEnabled(true);
        ui->ANALYSISPanel_Pipeline_Info__AuthorName_lineEdit->setEnabled(true);
        ui->ANALYSISPanel_Pipeline_Info_AuthorEmail_lineEdit->setEnabled(true);
        ui->ANALYSISPanel_Pipeline_Info_UUID_lineEdit->setEnabled(true);
        ui->ANALYSISPanel_Pipeline_Info_CreationDatetime_lineEdit->setEnabled(true);
        ui->ANALYSISPanel_Pipeline_Info_ModificationDatetime_lineEdit->setEnabled(true);
        ui->pipelineInfo_systemVersion_majorVersion_spinBox->setEnabled(true);
        ui->pipelineInfo_systemVersion_minorVersion_spinBox->setEnabled(true);
        ui->pipelineInfo_systemVersion_patchVersion_spinBox->setEnabled(true);

        ui->inputTransform_description_textEdit->setEnabled(true);
        ui->inputTransform_transformId_lineEdit->setEnabled(true);
        ui->inputTransform_hash_lineEdit->setEnabled(true);
        ui->inputTransform_uuid_lineEdit->setEnabled(true);
        ui->inputTransform_file_lineEdit->setEnabled(true);

        ui->inverseOutputTransform_description_textEdit->setEnabled(true);
        ui->inverseOutputTransform_hash_lineEdit->setEnabled(true);
        ui->inverseOutputTransform_uuid_lineEdit->setEnabled(true);
        ui->inverseOutputTransform_file_lineEdit->setEnabled(true);
        ui->inverseOutputTransform_transformId_lineEdit->setEnabled(true);

        ui->inverseOutputDeviceTransform_description_textEdit->setEnabled(true);
        ui->inverseOutputDeviceTransform_hash_lineEdit->setEnabled(true);
        ui->inverseOutputDeviceTransform_uuid_lineEdit->setEnabled(true);
        ui->inverseOutputDeviceTransform_file_lineEdit->setEnabled(true);
        ui->inverseOutputDeviceTransform_transformId_lineEdit->setEnabled(true);

        ui->inverseReferenceRenderingTransform_description_textEdit->setEnabled(true);
        ui->inverseReferenceRenderingTransform_hash_lineEdit->setEnabled(true);
        ui->inverseReferenceRenderingTransform_uuid_lineEdit->setEnabled(true);
        ui->inverseReferenceRenderingTransform_file_lineEdit->setEnabled(true);
        ui->inverseReferenceRenderingTransform_transformId_lineEdit->setEnabled(true);

        ui->lookTransform_description_textEdit->setEnabled(true);
        ui->lookTransform_hash_lineEdit->setEnabled(true);
        ui->lookTransform_uuid_lineEdit->setEnabled(true);
        ui->lookTransform_file_lineEdit->setEnabled(true);
        ui->lookTransform_transformId_lineEdit->setEnabled(true);

        ui->cdlWorkingSpace_toCdlWorkingSpace_description_textEdit->setEnabled(true);
        ui->cdlWorkingSpace_toCdlWorkingSpace_hash_lineEdit->setEnabled(true);
        ui->cdlWorkingSpace_toCdlWorkingSpace_uuid_lineEdit->setEnabled(true);
        ui->cdlWorkingSpace_toCdlWorkingSpace_file_lineEdit->setEnabled(true);
        ui->cdlWorkingSpace_toCdlWorkingSpace_transformId_lineEdit->setEnabled(true);

        ui->cdlWorkingSpace_fromCdlWorkingSpace_description_textEdit->setEnabled(true);
        ui->cdlWorkingSpace_fromCdlWorkingSpace_hash_lineEdit->setEnabled(true);
        ui->cdlWorkingSpace_fromCdlWorkingSpace_uuid_lineEdit->setEnabled(true);
        ui->cdlWorkingSpace_fromCdlWorkingSpace_file_lineEdit->setEnabled(true);
        ui->cdlWorkingSpace_fromCdlWorkingSpace_transformId_lineEdit->setEnabled(true);

        ui->Pipeline_RSlope_horizontalSlider->setEnabled(true);
        ui->Pipeline_GSlope_horizontalSlider->setEnabled(true);
        ui->Pipeline_BSlope_horizontalSlider->setEnabled(true);
        ui->Pipeline_ROffset_horizontalSlider->setEnabled(true);
        ui->Pipeline_GOffset_horizontalSlider->setEnabled(true);
        ui->Pipeline_BOffset_horizontalSlider->setEnabled(true);
        ui->Pipeline_RPower_horizontalSlider->setEnabled(true);
        ui->Pipeline_GPower_horizontalSlider->setEnabled(true);
        ui->Pipeline_BPower_horizontalSlider->setEnabled(true);
        ui->Pipeline_Saturation_horizontalSlider->setEnabled(true);

        ui->Pipeline_RSlope_DoublespinBox->setEnabled(true);
        ui->Pipeline_GSlope_DoublespinBox->setEnabled(true);
        ui->Pipeline_BSlope_DoublespinBox->setEnabled(true);
        ui->Pipeline_ROffset_DoublespinBox->setEnabled(true);
        ui->Pipeline_GOffset_DoublespinBox->setEnabled(true);
        ui->Pipeline_BOffset_DoublespinBox->setEnabled(true);
        ui->Pipeline_RPower_DoublespinBox->setEnabled(true);
        ui->Pipeline_GPower_DoublespinBox->setEnabled(true);
        ui->Pipeline_BPower_DoublespinBox->setEnabled(true);
        ui->Pipeline_Saturation_DoublespinBox->setEnabled(true);
        ui->Pipeline_ColorCorrectionRef_lineEdit->setEnabled(true);
        ui->Pipeline_ColorCorrectionRef_description->setEnabled(true);
        ui->Pipeline_ColorCorrectionRef_Inputdescription->setEnabled(true);
        ui->Pipeline_ColorCorrectionRef_Viewingdescription->setEnabled(true);

        ui->outputTransform_description_textEdit->setEnabled(true);
        ui->outputTransform_hash_lineEdit->setEnabled(true);
        ui->outputTransform_uuid_lineEdit->setEnabled(true);
        ui->outputTransform_file_lineEdit->setEnabled(true);
        ui->outputTransform_transformId_lineEdit->setEnabled(true);

        ui->outputDeviceTransform_description_textEdit->setEnabled(true);
        ui->outputDeviceTransform_hash_lineEdit->setEnabled(true);
        ui->outputDeviceTransform_uuid_lineEdit->setEnabled(true);
        ui->outputDeviceTransform_file_lineEdit->setEnabled(true);
        ui->outputDeviceTransform_transformId_lineEdit->setEnabled(true);

        ui->referenceRenderingTransform_description_textEdit->setEnabled(true);
        ui->referenceRenderingTransform_hash_lineEdit->setEnabled(true);
        ui->referenceRenderingTransform_uuid_lineEdit->setEnabled(true);
        ui->referenceRenderingTransform_file_lineEdit->setEnabled(true);
        ui->referenceRenderingTransform_transformId_lineEdit->setEnabled(true);
    }
    else if(!(ui->Pipeline_Editable_checkBox->isChecked())){
        ui->Pipeline_Editable_checkBox->setText("Not Editable");

        ui->ANALYSISPanel_Info_AMFInfo_TextEdit->setEnabled(false);
        ui->ANALYSISPanel_Info_Name_LineEdit->setEnabled(false);
        ui->ANALYSISPanel_Info_EmailAddress_LineEdit->setEnabled(false);
        ui->ANALYSISPanel_Info_AMFUUID_LineEdit->setEnabled(false);

        ui->ANALYSISPanel_Info_CilpID_TextEdit->setEnabled(false);
        ui->ANALYSISPanel_Info_CilpName_LineEdit->setEnabled(false);
        ui->ANALYSISPanel_Info_CilpUUID_LineEdit->setEnabled(false);
        ui->ANALYSISPanel_Info_FilePath_LineEdit->setEnabled(false);
        ui->ANALYSISPanel_Info_Sequence_LineEdit->setEnabled(false);
        ui->idx_lineEdit->setEnabled(false);
        ui->min_spinBox->setEnabled(false);
        ui->max_spinBox->setEnabled(false);

        ui->ANALYSISPanel_Pipeline_Info_textEdit->setEnabled(false);
        ui->ANALYSISPanel_Pipeline_Info__AuthorName_lineEdit->setEnabled(false);
        ui->ANALYSISPanel_Pipeline_Info_AuthorEmail_lineEdit->setEnabled(false);
        ui->ANALYSISPanel_Pipeline_Info_UUID_lineEdit->setEnabled(false);
        ui->ANALYSISPanel_Pipeline_Info_CreationDatetime_lineEdit->setEnabled(false);
        ui->ANALYSISPanel_Pipeline_Info_ModificationDatetime_lineEdit->setEnabled(false);
        ui->pipelineInfo_systemVersion_majorVersion_spinBox->setEnabled(false);
        ui->pipelineInfo_systemVersion_minorVersion_spinBox->setEnabled(false);
        ui->pipelineInfo_systemVersion_patchVersion_spinBox->setEnabled(false);

        ui->inputTransform_description_textEdit->setEnabled(false);
        ui->inputTransform_transformId_lineEdit->setEnabled(false);
        ui->inputTransform_hash_lineEdit->setEnabled(false);
        ui->inputTransform_uuid_lineEdit->setEnabled(false);
        ui->inputTransform_file_lineEdit->setEnabled(false);

        ui->inverseOutputTransform_description_textEdit->setEnabled(false);
        ui->inverseOutputTransform_hash_lineEdit->setEnabled(false);
        ui->inverseOutputTransform_uuid_lineEdit->setEnabled(false);
        ui->inverseOutputTransform_file_lineEdit->setEnabled(false);
        ui->inverseOutputTransform_transformId_lineEdit->setEnabled(false);

        ui->inverseOutputDeviceTransform_description_textEdit->setEnabled(false);
        ui->inverseOutputDeviceTransform_hash_lineEdit->setEnabled(false);
        ui->inverseOutputDeviceTransform_uuid_lineEdit->setEnabled(false);
        ui->inverseOutputDeviceTransform_file_lineEdit->setEnabled(false);
        ui->inverseOutputDeviceTransform_transformId_lineEdit->setEnabled(false);

        ui->inverseReferenceRenderingTransform_description_textEdit->setEnabled(false);
        ui->inverseReferenceRenderingTransform_hash_lineEdit->setEnabled(false);
        ui->inverseReferenceRenderingTransform_uuid_lineEdit->setEnabled(false);
        ui->inverseReferenceRenderingTransform_file_lineEdit->setEnabled(false);
        ui->inverseReferenceRenderingTransform_transformId_lineEdit->setEnabled(false);

        ui->lookTransform_description_textEdit->setEnabled(false);
        ui->lookTransform_hash_lineEdit->setEnabled(false);
        ui->lookTransform_uuid_lineEdit->setEnabled(false);
        ui->lookTransform_file_lineEdit->setEnabled(false);
        ui->lookTransform_transformId_lineEdit->setEnabled(false);

        ui->cdlWorkingSpace_toCdlWorkingSpace_description_textEdit->setEnabled(false);
        ui->cdlWorkingSpace_toCdlWorkingSpace_hash_lineEdit->setEnabled(false);
        ui->cdlWorkingSpace_toCdlWorkingSpace_uuid_lineEdit->setEnabled(false);
        ui->cdlWorkingSpace_toCdlWorkingSpace_file_lineEdit->setEnabled(false);
        ui->cdlWorkingSpace_toCdlWorkingSpace_transformId_lineEdit->setEnabled(false);

        ui->cdlWorkingSpace_fromCdlWorkingSpace_description_textEdit->setEnabled(false);
        ui->cdlWorkingSpace_fromCdlWorkingSpace_hash_lineEdit->setEnabled(false);
        ui->cdlWorkingSpace_fromCdlWorkingSpace_uuid_lineEdit->setEnabled(false);
        ui->cdlWorkingSpace_fromCdlWorkingSpace_file_lineEdit->setEnabled(false);
        ui->cdlWorkingSpace_fromCdlWorkingSpace_transformId_lineEdit->setEnabled(false);

        ui->Pipeline_RSlope_horizontalSlider->setEnabled(false);
        ui->Pipeline_GSlope_horizontalSlider->setEnabled(false);
        ui->Pipeline_BSlope_horizontalSlider->setEnabled(false);
        ui->Pipeline_ROffset_horizontalSlider->setEnabled(false);
        ui->Pipeline_GOffset_horizontalSlider->setEnabled(false);
        ui->Pipeline_BOffset_horizontalSlider->setEnabled(false);
        ui->Pipeline_RPower_horizontalSlider->setEnabled(false);
        ui->Pipeline_GPower_horizontalSlider->setEnabled(false);
        ui->Pipeline_BPower_horizontalSlider->setEnabled(false);
        ui->Pipeline_Saturation_horizontalSlider->setEnabled(false);

        ui->Pipeline_RSlope_DoublespinBox->setEnabled(false);
        ui->Pipeline_GSlope_DoublespinBox->setEnabled(false);
        ui->Pipeline_BSlope_DoublespinBox->setEnabled(false);
        ui->Pipeline_ROffset_DoublespinBox->setEnabled(false);
        ui->Pipeline_GOffset_DoublespinBox->setEnabled(false);
        ui->Pipeline_BOffset_DoublespinBox->setEnabled(false);
        ui->Pipeline_RPower_DoublespinBox->setEnabled(false);
        ui->Pipeline_GPower_DoublespinBox->setEnabled(false);
        ui->Pipeline_BPower_DoublespinBox->setEnabled(false);
        ui->Pipeline_Saturation_DoublespinBox->setEnabled(false);
        ui->Pipeline_ColorCorrectionRef_lineEdit->setEnabled(false);
        ui->Pipeline_ColorCorrectionRef_description->setEnabled(false);
        ui->Pipeline_ColorCorrectionRef_Inputdescription->setEnabled(false);
        ui->Pipeline_ColorCorrectionRef_Viewingdescription->setEnabled(false);

        ui->outputTransform_description_textEdit->setEnabled(false);
        ui->outputTransform_hash_lineEdit->setEnabled(false);
        ui->outputTransform_uuid_lineEdit->setEnabled(false);
        ui->outputTransform_file_lineEdit->setEnabled(false);
        ui->outputTransform_transformId_lineEdit->setEnabled(false);

        ui->outputDeviceTransform_description_textEdit->setEnabled(false);
        ui->outputDeviceTransform_hash_lineEdit->setEnabled(false);
        ui->outputDeviceTransform_uuid_lineEdit->setEnabled(false);
        ui->outputDeviceTransform_file_lineEdit->setEnabled(false);
        ui->outputDeviceTransform_transformId_lineEdit->setEnabled(false);

        ui->referenceRenderingTransform_description_textEdit->setEnabled(false);
        ui->referenceRenderingTransform_hash_lineEdit->setEnabled(false);
        ui->referenceRenderingTransform_uuid_lineEdit->setEnabled(false);
        ui->referenceRenderingTransform_file_lineEdit->setEnabled(false);
        ui->referenceRenderingTransform_transformId_lineEdit->setEnabled(false);
    }
}

void amfanalyst::Navigater(int value)
{
    if (value==0){
        ui->Pipeline_Navigator_PipelineInfo_pushButton->setDefault(true);
        ui->Pipeline_Navigator_IDT_pushButton->setDefault(false);
        ui->Pipeline_Navigator_LMT_pushButton->setDefault(false);
        ui->Pipeline_Navigator_ODT_pushButton->setDefault(false);}
    else if (value==1){
        ui->Pipeline_Navigator_PipelineInfo_pushButton->setDefault(false);
        ui->Pipeline_Navigator_IDT_pushButton->setDefault(true);
        ui->Pipeline_Navigator_LMT_pushButton->setDefault(false);
        ui->Pipeline_Navigator_ODT_pushButton->setDefault(false);}
    if (value==2){
        ui->Pipeline_Navigator_PipelineInfo_pushButton->setDefault(false);
        ui->Pipeline_Navigator_IDT_pushButton->setDefault(false);
        ui->Pipeline_Navigator_LMT_pushButton->setDefault(true);
        ui->Pipeline_Navigator_ODT_pushButton->setDefault(false);}
    if (value==3){
        ui->Pipeline_Navigator_PipelineInfo_pushButton->setDefault(false);
        ui->Pipeline_Navigator_IDT_pushButton->setDefault(false);
        ui->Pipeline_Navigator_LMT_pushButton->setDefault(false);
        ui->Pipeline_Navigator_ODT_pushButton->setDefault(true);}
}

void amfanalyst::Navigater0(){ui->ANALYSISPanel_Pipeline_Lift_ToolBox->setCurrentIndex(0);}
void amfanalyst::Navigater1(){ui->ANALYSISPanel_Pipeline_Lift_ToolBox->setCurrentIndex(1);}
void amfanalyst::Navigater2(){ui->ANALYSISPanel_Pipeline_Lift_ToolBox->setCurrentIndex(2);}
void amfanalyst::Navigater3(){ui->ANALYSISPanel_Pipeline_Lift_ToolBox->setCurrentIndex(3);}

void amfanalyst::refreshCDLValueSendFrom_slotslider()
{
    slotslider_DoubleSpinBox(ui->Pipeline_RSlope_horizontalSlider,ui->Pipeline_RSlope_DoublespinBox);
    slotslider_DoubleSpinBox(ui->Pipeline_GSlope_horizontalSlider,ui->Pipeline_GSlope_DoublespinBox);
    slotslider_DoubleSpinBox(ui->Pipeline_BSlope_horizontalSlider,ui->Pipeline_BSlope_DoublespinBox);
    slotslider_DoubleSpinBox(ui->Pipeline_ROffset_horizontalSlider,ui->Pipeline_ROffset_DoublespinBox);
    slotslider_DoubleSpinBox(ui->Pipeline_GOffset_horizontalSlider,ui->Pipeline_GOffset_DoublespinBox);
    slotslider_DoubleSpinBox(ui->Pipeline_BOffset_horizontalSlider,ui->Pipeline_BOffset_DoublespinBox);
    slotslider_DoubleSpinBox(ui->Pipeline_RPower_horizontalSlider,ui->Pipeline_RPower_DoublespinBox);
    slotslider_DoubleSpinBox(ui->Pipeline_GPower_horizontalSlider,ui->Pipeline_GPower_DoublespinBox);
    slotslider_DoubleSpinBox(ui->Pipeline_BPower_horizontalSlider,ui->Pipeline_BPower_DoublespinBox);
    slotslider_DoubleSpinBox(ui->Pipeline_Saturation_horizontalSlider,ui->Pipeline_Saturation_DoublespinBox);

}

void amfanalyst::refreshCDLValueSendFrom_slotDoubleSpinbox()
{
    slotDoubleSpinbox_slider(ui->Pipeline_RSlope_horizontalSlider,ui->Pipeline_RSlope_DoublespinBox);
    slotDoubleSpinbox_slider(ui->Pipeline_GSlope_horizontalSlider,ui->Pipeline_GSlope_DoublespinBox);
    slotDoubleSpinbox_slider(ui->Pipeline_BSlope_horizontalSlider,ui->Pipeline_BSlope_DoublespinBox);
    slotDoubleSpinbox_slider(ui->Pipeline_ROffset_horizontalSlider,ui->Pipeline_ROffset_DoublespinBox);
    slotDoubleSpinbox_slider(ui->Pipeline_GOffset_horizontalSlider,ui->Pipeline_GOffset_DoublespinBox);
    slotDoubleSpinbox_slider(ui->Pipeline_BOffset_horizontalSlider,ui->Pipeline_BOffset_DoublespinBox);
    slotDoubleSpinbox_slider(ui->Pipeline_RPower_horizontalSlider,ui->Pipeline_RPower_DoublespinBox);
    slotDoubleSpinbox_slider(ui->Pipeline_GPower_horizontalSlider,ui->Pipeline_GPower_DoublespinBox);
    slotDoubleSpinbox_slider(ui->Pipeline_BPower_horizontalSlider,ui->Pipeline_BPower_DoublespinBox);
    slotDoubleSpinbox_slider(ui->Pipeline_Saturation_horizontalSlider,ui->Pipeline_Saturation_DoublespinBox);


}


void amfanalyst::slotslider_DoubleSpinBox(QSlider *a, QDoubleSpinBox *b)    //Slider的值给SpinBox
{
    b->setValue((static_cast<double>(a->value())/1000));
}

void amfanalyst::slotDoubleSpinbox_slider(QSlider *a, QDoubleSpinBox *b)    //SpinBox的值给Slider
{
    a->setValue(static_cast<int>(b->value()*1000));
}

void amfanalyst::grab_2_tiff()
{
    if(this->IfOpenAMF)
    {
        if(ui->OUTPUTPanel_SaveAs_LineEdit->text().isEmpty()){                              // 如果没有识别到保存路径，要求获取一个保存路径
          this->Getuser_AMFOutputAMFPath();
        };

        if(ui->SaveFileName_label_lineEdit->text().isEmpty()){
            this->Getuser_AMFOutputAMFName();
        }

        QString toDir = this->OutputAMFPath + "/" + ui->SaveFileName_label_lineEdit->text() + ".tif";

        qDebug() << "原始路径" << QString::fromStdString(this->currentamf->Get_output_picpath());
        qDebug() << "目标路径" << toDir;

        QFile file(QString::fromStdString(this->currentamf->Get_output_picpath())); // 创建一个QFile 变量
        file.copy(toDir);                                                           // 使用copy()完成文件的拷贝
    }
}

void amfanalyst::grab_2_exr()
{
    {
        if(this->IfOpenAMF)
        {
            if(ui->OUTPUTPanel_SaveAs_LineEdit->text().isEmpty()){                              // 如果没有识别到保存路径，要求获取一个保存路径
              this->Getuser_AMFOutputAMFPath();
            };

            if(ui->SaveFileName_label_lineEdit->text().isEmpty()){
                this->Getuser_AMFOutputAMFName();
            }

            QString fromDir = QString::fromStdString(this->currentamf->Get_input_picpath_exr());
            QString toDir = this->OutputAMFPath + "/" + ui->SaveFileName_label_lineEdit->text() + ".exr";

            qDebug() << "原始路径" << fromDir;
            qDebug() << "目标路径" << toDir;

            this->currentamf->Modification_input_picpath(fromDir.toStdString());
            this->currentamf->Modification_output_picpath(toDir.toStdString());

            ctlrendertool *c2=new ctlrendertool;            // 启用管线，在构造parseAMF的时候就调用了AMF-util（python）来执行渲染，但是考虑到兼容性问题，以及可能出现的渲染失误，我在外面又使用了我自己的解析渲染。
            c2->usingctlrender(*this->currentamf);                  // 在这里关闭双管道，注释调这行关闭调我自己的管道。

            //QDialog *dlg = new QDialog(this);

        }
    }
}

void amfanalyst::dispParsedAMF(ParseAMF* this_amf)
{
    // 将指针对象的值赋给导入的this_amf对象
    this->currentamf = this_amf;
    std::cout << "第271行读取到的AMF实例地址:"<<this_amf<< std::endl;
    std::cout << "第272行读取到的储存的值:"<<this->currentamf << std::endl;
    std::cout << "第273行的调用尝试:"<< this->currentamf->Get_input_picpath() << std::endl;

    ui->ANALYSISPanel_Info_CreationTime_Label->setText(QString::fromStdString("CreationTime："+this_amf->Get_dateTime_creationDateTime()));
    ui->ANALYSISPanel_Info_ModificationTime_Label->setText(QString::fromStdString("ModificationTime："+this_amf->Get_dateTime_modificationDateTime()));

    ui->ANALYSISPanel_Info_AMFInfo_TextEdit->setText(QString::fromStdString(this_amf->Get_description()));
    ui->ANALYSISPanel_Info_Name_LineEdit->setText(QString::fromStdString(this_amf->Get_author_name()));
    ui->ANALYSISPanel_Info_EmailAddress_LineEdit->setText(QString::fromStdString(this_amf->Get_author_emailAdress()));
    ui->ANALYSISPanel_Info_AMFUUID_LineEdit->setText(QString::fromStdString(this_amf->Get_uuid()));

    ui->ANALYSISPanel_Info_CilpID_TextEdit->setText(QString::fromStdString(this_amf->Get_ClipID_description()));
    ui->ANALYSISPanel_Info_CilpName_LineEdit->setText(QString::fromStdString(this_amf->Get_ClipID_cilpname()));
    ui->ANALYSISPanel_Info_CilpUUID_LineEdit->setText(QString::fromStdString(this_amf->Get_ClipID_uuid()));
    ui->ANALYSISPanel_Info_FilePath_LineEdit->setText(QString::fromStdString(this_amf->Get_ClipID_path()));
    ui->ANALYSISPanel_Info_Sequence_LineEdit->setText(QString::fromStdString(this_amf->Get_ClipID_sequence()));
    ui->idx_lineEdit->setText(QString::fromStdString(this_amf->Get_ClipID_sequence_idx()));
    ui->min_spinBox->setValue(this_amf->Get_ClipID_sequence_min());
    ui->max_spinBox->setValue(this_amf->Get_ClipID_sequence_max());

    ui->ANALYSISPanel_Pipeline_Info_textEdit->setText(QString::fromStdString(this_amf->Get_pipeline_description()));
    ui->ANALYSISPanel_Pipeline_Info__AuthorName_lineEdit->setText(QString::fromStdString(this_amf->Get_pipelineInfo_author_name()));
    ui->ANALYSISPanel_Pipeline_Info_AuthorEmail_lineEdit->setText(QString::fromStdString(this_amf->Get_pipelineInfo_author_emailAdress()));
    ui->ANALYSISPanel_Pipeline_Info_UUID_lineEdit->setText(QString::fromStdString(this_amf->Get_pipelineInfo_uuid()));
    ui->ANALYSISPanel_Pipeline_Info_CreationDatetime_lineEdit->setText(QString::fromStdString(this_amf->Get_pipelineInfo_dateTime_creationDateTime()));
    ui->ANALYSISPanel_Pipeline_Info_ModificationDatetime_lineEdit->setText(QString::fromStdString(this_amf->Get_pipelineInfo_dateTime_modificationDateTime()));
    ui->pipelineInfo_systemVersion_majorVersion_spinBox->setValue(this_amf->Get_pipelineInfo_systemVersion_majorVersion());
    ui->pipelineInfo_systemVersion_minorVersion_spinBox->setValue(this_amf->Get_pipelineInfo_systemVersion_minorVersion());
    ui->pipelineInfo_systemVersion_patchVersion_spinBox->setValue(this_amf->Get_pipelineInfo_systemVersion_patchVersion());

    // inputTransform applied status
    if((!ui->inputTransform_applied_checkBox->isChecked())&&this_amf->Get_inputTransform_applied()){
        ui->inputTransform_applied_checkBox->setChecked(true);
    };
    if((ui->inputTransform_applied_checkBox->isChecked())&&this_amf->Get_inputTransform_applied()){
        ui->inputTransform_applied_checkBox->setChecked(true);
    }
    if((!ui->inputTransform_applied_checkBox->isChecked())&&!this_amf->Get_inputTransform_applied()){
        ui->inputTransform_applied_checkBox->setChecked(false);
    }
    if((ui->inputTransform_applied_checkBox->isChecked())&&!this_amf->Get_inputTransform_applied()){
        ui->inputTransform_applied_checkBox->setChecked(false);
    }

    ui->inputTransform_description_textEdit->setText(QString::fromStdString(this_amf->Get_inputTransform_description()));
    ui->inputTransform_transformId_lineEdit->setText(QString::fromStdString(this_amf->Get_inputTransform_transformId()));
    ui->inputTransform_hash_lineEdit->setText(QString::fromStdString(this_amf->Get_inputTransform_hash()));
    ui->inputTransform_uuid_lineEdit->setText(QString::fromStdString(this_amf->Get_inputTransform_uuid()));
    ui->inputTransform_file_lineEdit->setText(QString::fromStdString(this_amf->Get_inputTransform_file()));

    ui->inverseOutputTransform_description_textEdit->setText(QString::fromStdString(this_amf->Get_inverseOutputTransform_description()));
    ui->inverseOutputTransform_hash_lineEdit->setText(QString::fromStdString(this_amf->Get_inverseOutputTransform_hash()));
    ui->inverseOutputTransform_uuid_lineEdit->setText(QString::fromStdString(this_amf->Get_inverseOutputTransform_uuid()));
    ui->inverseOutputTransform_file_lineEdit->setText(QString::fromStdString(this_amf->Get_inverseOutputTransform_file()));
    ui->inverseOutputTransform_transformId_lineEdit->setText(QString::fromStdString(this_amf->Get_inverseOutputTransform_transformId()));

    ui->inverseOutputDeviceTransform_description_textEdit->setText(QString::fromStdString(this_amf->Get_inverseOutputDeviceTransform_description()));
    ui->inverseOutputDeviceTransform_hash_lineEdit->setText(QString::fromStdString(this_amf->Get_inverseOutputDeviceTransform_hash()));
    ui->inverseOutputDeviceTransform_uuid_lineEdit->setText(QString::fromStdString(this_amf->Get_inverseOutputDeviceTransform_uuid()));
    ui->inverseOutputDeviceTransform_file_lineEdit->setText(QString::fromStdString(this_amf->Get_inverseOutputDeviceTransform_file()));
    ui->inverseOutputDeviceTransform_transformId_lineEdit->setText(QString::fromStdString(this_amf->Get_inverseOutputDeviceTransform_transformId()));

    ui->inverseReferenceRenderingTransform_description_textEdit->setText(QString::fromStdString(this_amf->Get_inverseReferenceRenderingTransform_description()));
    ui->inverseReferenceRenderingTransform_hash_lineEdit->setText(QString::fromStdString(this_amf->Get_inverseReferenceRenderingTransform_hash()));
    ui->inverseReferenceRenderingTransform_uuid_lineEdit->setText(QString::fromStdString(this_amf->Get_inverseReferenceRenderingTransform_uuid()));
    ui->inverseReferenceRenderingTransform_file_lineEdit->setText(QString::fromStdString(this_amf->Get_inverseReferenceRenderingTransform_file()));
    ui->inverseReferenceRenderingTransform_transformId_lineEdit->setText(QString::fromStdString(this_amf->Get_inverseReferenceRenderingTransform_transformId()));

    // lookTransform applied status
    if((!ui->lookTransform_applied_checkBox->isChecked())&&this_amf->Get_lookTransform_applied()){
        ui->lookTransform_applied_checkBox->setChecked(true);
    };
    if((ui->lookTransform_applied_checkBox->isChecked())&&this_amf->Get_lookTransform_applied()){
        ui->lookTransform_applied_checkBox->setChecked(true);
    }
    if((!ui->lookTransform_applied_checkBox->isChecked())&&!this_amf->Get_lookTransform_applied()){
        ui->lookTransform_applied_checkBox->setChecked(false);
    }
    if((ui->lookTransform_applied_checkBox->isChecked())&&!this_amf->Get_lookTransform_applied()){
        ui->lookTransform_applied_checkBox->setChecked(false);
    }

    ui->lookTransform_description_textEdit->setText(QString::fromStdString(this_amf->Get_lookTransform_description()));
    ui->lookTransform_hash_lineEdit->setText(QString::fromStdString(this_amf->Get_lookTransform_hash()));
    ui->lookTransform_uuid_lineEdit->setText(QString::fromStdString(this_amf->Get_lookTransform_uuid()));
    ui->lookTransform_file_lineEdit->setText(QString::fromStdString(this_amf->Get_lookTransform_file()));
    ui->lookTransform_transformId_lineEdit->setText(QString::fromStdString(this_amf->Get_lookTransform_transformId()));

    ui->cdlWorkingSpace_toCdlWorkingSpace_description_textEdit->setText(QString::fromStdString(this_amf->Get_cdlWorkingSpace_toCdlWorkingSpace_description()));
    ui->cdlWorkingSpace_toCdlWorkingSpace_hash_lineEdit->setText(QString::fromStdString(this_amf->Get_cdlWorkingSpace_toCdlWorkingSpace_hash()));
    ui->cdlWorkingSpace_toCdlWorkingSpace_uuid_lineEdit->setText(QString::fromStdString(this_amf->Get_cdlWorkingSpace_toCdlWorkingSpace_uuid()));
    ui->cdlWorkingSpace_toCdlWorkingSpace_file_lineEdit->setText(QString::fromStdString(this_amf->Get_cdlWorkingSpace_toCdlWorkingSpace_file()));
    ui->cdlWorkingSpace_toCdlWorkingSpace_transformId_lineEdit->setText(QString::fromStdString(this_amf->Get_cdlWorkingSpace_toCdlWorkingSpace_transformId()));

    ui->cdlWorkingSpace_fromCdlWorkingSpace_description_textEdit->setText(QString::fromStdString(this_amf->Get_cdlWorkingSpace_fromCdlWorkingSpace_description()));
    ui->cdlWorkingSpace_fromCdlWorkingSpace_hash_lineEdit->setText(QString::fromStdString(this_amf->Get_cdlWorkingSpace_fromCdlWorkingSpace_hash()));
    ui->cdlWorkingSpace_fromCdlWorkingSpace_uuid_lineEdit->setText(QString::fromStdString(this_amf->Get_cdlWorkingSpace_fromCdlWorkingSpace_uuid()));
    ui->cdlWorkingSpace_fromCdlWorkingSpace_file_lineEdit->setText(QString::fromStdString(this_amf->Get_cdlWorkingSpace_fromCdlWorkingSpace_file()));
    ui->cdlWorkingSpace_fromCdlWorkingSpace_transformId_lineEdit->setText(QString::fromStdString(this_amf->Get_cdlWorkingSpace_fromCdlWorkingSpace_transformId()));

    ui->Pipeline_RSlope_DoublespinBox->setValue(this_amf->Get_ASC_SOP_Slope_R());
    ui->Pipeline_GSlope_DoublespinBox->setValue(this_amf->Get_ASC_SOP_Slope_G());
    ui->Pipeline_BSlope_DoublespinBox->setValue(this_amf->Get_ASC_SOP_Slope_B());
    ui->Pipeline_ROffset_DoublespinBox->setValue(this_amf->Get_ASC_SOP_Offset_R());
    ui->Pipeline_GOffset_DoublespinBox->setValue(this_amf->Get_ASC_SOP_Offset_G());
    ui->Pipeline_BOffset_DoublespinBox->setValue(this_amf->Get_ASC_SOP_Offset_B());
    ui->Pipeline_RPower_DoublespinBox->setValue(this_amf->Get_ASC_SOP_Power_R());
    ui->Pipeline_GPower_DoublespinBox->setValue(this_amf->Get_ASC_SOP_Power_G());
    ui->Pipeline_BPower_DoublespinBox->setValue(this_amf->Get_ASC_SOP_Power_B());
    ui->Pipeline_Saturation_DoublespinBox->setValue(this_amf->Get_ASC_SAT_Saturation());
    ui->Pipeline_ColorCorrectionRef_lineEdit->setText(QString::fromStdString(this->currentamf->Get_ColorCorrectionRef_ref()));
    ui->Pipeline_ColorCorrectionRef_description->setText(QString::fromStdString(this->currentamf->Get_ColorCorrectionRef_Description()));
    ui->Pipeline_ColorCorrectionRef_Inputdescription->setText(QString::fromStdString(this->currentamf->Get_ColorCorrectionRef_InputDescription()));
    ui->Pipeline_ColorCorrectionRef_Viewingdescription->setText(QString::fromStdString(this->currentamf->Get_ColorCorrectionRef_ViewingDescription()));

    ui->outputTransform_description_textEdit->setText(QString::fromStdString(this_amf->Get_outputTransform_description()));
    ui->outputTransform_hash_lineEdit->setText(QString::fromStdString(this_amf->Get_outputTransform_hash()));
    ui->outputTransform_uuid_lineEdit->setText(QString::fromStdString(this_amf->Get_outputTransform_uuid()));
    ui->outputTransform_file_lineEdit->setText(QString::fromStdString(this_amf->Get_outputTransform_file()));
    ui->outputTransform_transformId_lineEdit->setText(QString::fromStdString(this_amf->Get_outputTransform_transformId()));

    ui->outputDeviceTransform_description_textEdit->setText(QString::fromStdString(this_amf->Get_outputDeviceTransform_description()));
    ui->outputDeviceTransform_hash_lineEdit->setText(QString::fromStdString(this_amf->Get_outputDeviceTransform_hash()));
    ui->outputDeviceTransform_uuid_lineEdit->setText(QString::fromStdString(this_amf->Get_outputDeviceTransform_uuid()));
    ui->outputDeviceTransform_file_lineEdit->setText(QString::fromStdString(this_amf->Get_outputDeviceTransform_file()));
    ui->outputDeviceTransform_transformId_lineEdit->setText(QString::fromStdString(this_amf->Get_outputDeviceTransform_transformId()));

    ui->referenceRenderingTransform_description_textEdit->setText(QString::fromStdString(this_amf->Get_referenceRenderingTransform_description()));
    ui->referenceRenderingTransform_hash_lineEdit->setText(QString::fromStdString(this_amf->Get_referenceRenderingTransform_hash()));
    ui->referenceRenderingTransform_uuid_lineEdit->setText(QString::fromStdString(this_amf->Get_referenceRenderingTransform_uuid()));
    ui->referenceRenderingTransform_file_lineEdit->setText(QString::fromStdString(this_amf->Get_referenceRenderingTransform_file()));
    ui->referenceRenderingTransform_transformId_lineEdit->setText(QString::fromStdString(this_amf->Get_referenceRenderingTransform_transformId()));

    //AnalysisModification Panel / Pipeline / Display
    connect(ui->ANALYSISPanel_Pipeline_Right_Display_Refresh_pushButton,SIGNAL(clicked()),this,SLOT(RefreshPic()));
    connect(ui->ANALYSISPanel_Pipeline_Right_Display_Refresh_pushButton,SIGNAL(clicked()),this,SLOT(RefreshCube()));
};


