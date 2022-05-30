#ifndef AMFANALYST_H
#define AMFANALYST_H

#include <QMainWindow>
#include "ui_amfanalyst.h"
#include "ctlrendertool.h"
#include "parseamf.h"
#include <QPixmap>
#include <QPainter>
#include <QMessageBox>
#include <QDir>
#include <QDebug>
#include <cstdio>
#include "scatter.h"
#include "amfutil.h"
#include "ocioutil.h"


#include <QtCharts/QSplineSeries>
#include <QtWidgets/QGraphicsTextItem>
#include <QtCharts/QChart>
#include <QtCharts/QLineSeries>
#include <QtCharts/QSplineSeries>
#include "callout.h"


// 这里面主要的内容是主要用来显示具体部件内容的

namespace Ui {
class amfanalyst;
}

class amfanalyst : public QMainWindow
{
    Q_OBJECT

public:
    explicit amfanalyst(QWidget *parent = nullptr);
    ~amfanalyst();

    QString Get_newAMFpath();
    QString Get_OutputAMFPath();


private:
    Ui::amfanalyst *ui;

public slots:      //槽函数的声明 1 一些组件使用到的槽函数
    // ——————————————————————————————————————————————————————————————————————
    // Import Panel / lift
    void Getuser_AMFpath();

    // ——————————————————————————————————————————————————————————————————————
    // AnalysisModification Panel / Display
    void RefreshPic();  // 根据给定的amfrender对象 然后根据refresh按钮的状态 判断应该显示渲染后的还是渲染前的图
                                    // 如果要显示渲染后的图判断图片是否有渲染出来 如果没有 弹出窗口 让用户再等等
    void displayPic(std::string);   //给定一张图片 在分析页面的图片窗口显示出来
    bool isDirExist(QString fullPath);  // 用于判断某个绝对路径是否存在 用于判断图片是否生成
    void displayCube(Q3DScatter *graph);    // 将一个Q3DScatter形式的对象显示到Pipieline Cube页面
    void RefreshCube();     // 在parseamf中寻找cube的路径，然后执行displayCube(); 这个SLOT被刷新控件调用

    void displayLinechart(QString OriginPIc_LUT, QString AfterPIc_LUT);        // 在折线图窗口显示折线图，在外部主要调用的就是这个
    void SetSeries(QLineSeries *series1,QLineSeries *series2,QLineSeries *series3,QString path1,QString path2); // 对LUT_Pic数据进行解析的函数
    void keepCallout();
    void tooltip(QPointF point, bool state);

    bool Get_IfOpenAMF();   // 用于判断之前是否打开过amf文件
    bool Get_IfhadCUBE();   // 用于判断是否已经有Cube图像了

    void Refresh_Editable_state(int state);      // 判断一下Editable Checkbox的状态，如果是选中的状态，就是可以编辑所有的内容，如果不是选中状态，则关闭所有AMF内容的编辑权限，默认以关闭状态呈现

    // ——————————————————————————————————————————————————————————————————————
    // AnalysisModification Panel / Navigater
    void Navigater(int);    //用于给Navigater定位器使用 点击定位器 跳转到对应页面
    void Navigater0();
    void Navigater1();
    void Navigater2();
    void Navigater3();

    // ——————————————————————————————————————————————————————————————————————
    // AnalysisModification Panel / Pipeline / LMT
    void refreshCDLValueSendFrom_slotslider();
    void refreshCDLValueSendFrom_slotDoubleSpinbox();
    void slotslider_DoubleSpinBox(QSlider* a,QDoubleSpinBox* b);
    void slotDoubleSpinbox_slider(QSlider* a,QDoubleSpinBox* b);

    void grab_2_tiff();                 // 分析页面底下的导出tiff
    void grab_2_exr();                  // 分析页面底下导出为exr

    // ——————————————————————————————————————————————————————————————————————
    // Output Panel
    void Getuser_AMFOutputAMFPath();    // 弹出一个页面让用户选择输出路径，结果保存在this的私有变量OutputAMFPath=newAMFpath里面
    void Getuser_AMFOutputAMFName();    // 弹出一个界面要求获取用户输入新AMF文件的文件名，结果保存到用户界面上
    void Creation_OutputAMF();      // 根据界面上显示的内容创建一个新的xml
    //void XmlSchema_validation();        // 【这是一个待撰写的函数，使用XmlSchema来验证“待打开”和“待创建”的xml是否是正确的】

    void Creation_LUT();                  // 输出LUT按钮LUT_Output_pushButton,调用这个函数
                                          // 在这个函数里会判断具体输出什么样的LUT;
    void Creation_CombinedLUT();          // 点击输出LUT按钮，将根据界面上的管线内容首先创建一个AMF文件到制定位置；
                                          // 然后读取该AMF文件（ANF-util）获取到CTLrender paras
                                          // 然后根据界面上到管道设置，选择性地渲染到不同精度的LUT图片上
                                          // 最后调用ociolutimage提取LUT图片上的信息，输出为.3dl格式
                                          // 再调用ociobakelut将其转换为其他格式
                                          // 转换完成之后，将过程中产生的.amf .tif .spi3d全部删除

//    void Creation_IDTODT_LUT();           // 由于目前尚不支持LMT处理，所以LMT输出部分暂时删除
//    void Creation_ODT_LUT();
//    void Creation_IDT_LUT();

    void dispParsedAMF(ParseAMF* );     //将读到的parseAMF示例中的数据显示到用户界面上，并将该实例的位置储存到this->currentamf中
    QString Get_Time();

private:
    ParseAMF *currentamf = new ParseAMF();       //当前所使用的AMF文件

    bool IfOpenAMF=0;
    bool IfhadCUBE=0;
    QString newAMFpath = "/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/TestFile/LogCEI800-Rec.709100nitsdim.amf";    // 要打开的AMF的绝对路径
    QString OutputAMFPath;                                                                                                   // 要输出的AMF的绝对路径,默认和原始打开路径一致


    // for cruve grah
    QGraphicsSimpleTextItem *m_coordX;
    QGraphicsSimpleTextItem *m_coordY;
    QChart *m_chart;
    Callout *m_tooltip;
    QList<Callout *> m_callouts;

};

#endif // AMFANALYST_H
