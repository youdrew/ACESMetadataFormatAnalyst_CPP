#include "scatter.h"
#include <QtDebug>
#include<sstream>
#include <QtCore/qmath.h>
#include <iostream>
#include <QFile>
#include <fstream>
#include <QObject>

const int lowerNumberOfItems = 5000;
const float lowerCurveDivider = 0.75f;

//构造函数，修改坐标系的背景和标签，最后把点加载进来
scatter::scatter(Q3DScatter *scatter,QSlider *pslider) : m_graph(scatter),
    m_slider(pslider),
    m_dataArray1(new QScatterDataArray),
    m_dataArray2(new QScatterDataArray),
    m_fontSize(40.0f),
    m_smooth(true),
    m_itemCount(lowerNumberOfItems),
    m_count(0),
    m_curveDivider(lowerCurveDivider),
    item(new QScatter3DSeries)
{
    //设置三维坐标系的各种参数
    m_graph->activeTheme()->setType(Q3DTheme::ThemeEbony);
    QFont font = m_graph->activeTheme()->font();
    font.setPointSize(m_fontSize);
    m_graph->activeTheme()->setFont(font);
    m_graph->setShadowQuality(QAbstract3DGraph::ShadowQualitySoftLow);
    m_graph->scene()->activeCamera()->setCameraPreset(Q3DCamera::CameraPresetFront);

}


void scatter::parse_lut(QScatterDataItem *ptrToDataArray,QString path,float *pdata)
{

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {

        qDebug()<<"Can't open the cube file!";

    }
    int count = 0;
    while(!file.atEnd())

    {

        QByteArray line = file.readLine();

        QString str(line);
        std::string str2(line);

        if(str[0].isDigit())
        {
            //qDebug()<<str2.c_str();
            std::vector<std::string> res;
            //暂存从word中读取的字符串
            std::string result;
            //将字符串读到input中
            std::stringstream input(str2);
            //依次输出到result中，并存入res中
            while(input>>result)
                res.push_back(result);
            //输出res
            QVector3D ret;
            for(unsigned int i=0;i<res.size();i++){
                float n = atof ( res[i].c_str() );

                if (i==3)
                {
                    ret.setX(n);
                    *(pdata + count*3 + i-3) = n;

                }
                else if(i==4)
                {
                    ret.setY(n);
                    *(pdata + count*3 + i-3) = n;
                }
                else if(i==5)
                {
                    ret.setZ(n);
                    *(pdata + count*3 + i-3) = n;
                }
            }

            //qDebug()<<ret.x()<<ret.y()<<ret.z();
            ptrToDataArray->setPosition(ret);
            ptrToDataArray++;
            count++;
        }

    }
    m_count= count;
}


void scatter::getlut(QString path1,QString path2)
{
    m_dataArray1->resize(m_itemCount);
    QScatterDataItem *ptrToDataArray1 = &m_dataArray1->first();
    int num1;
    num1 = undersampling(path1,&m_pdata1[0][0]);
    for(int i = 0; i < num1; i++)
    {
        QVector3D ret;
        ret.setX(*(&m_pdata1[0][0] + i*3));
        ret.setY(*(&m_pdata1[0][0] + i*3 + 1));
        ret.setZ(*(&m_pdata1[0][0] + i*3 + 2));
        ptrToDataArray1->setPosition(ret);
        ptrToDataArray1++;
    }

    int num2;
    num2 = undersampling(path2,&m_pdata2[0][0]);

    if(num1 == num2)
        m_count = num1;
    else
        qDebug()<<"Two LUT file size not equal!!";

    QScatterDataProxy *proxy = new QScatterDataProxy;
    QScatter3DSeries *series = new QScatter3DSeries(proxy);
    series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    series->setMeshSmooth(m_smooth);
    m_graph->addSeries(item);
    item->dataProxy()->resetArray(m_dataArray1);
}


//刷新Lut数据
void scatter::repaint()
{
    //删除原有数据
    m_graph->removeSeries(item);

    //新建数据
    item = new QScatter3DSeries();
    QScatterDataProxy *proxy = new QScatterDataProxy;
    QScatter3DSeries *series = new QScatter3DSeries(proxy);
    series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    series->setMeshSmooth(m_smooth);
    m_graph->addSeries(item);
    QScatterDataArray *dataArray = new QScatterDataArray;
    dataArray->resize(lowerNumberOfItems);
    QScatterDataItem *ptrToDataArray = &dataArray->first();

    //遍历所有数据，求新的点
    //pd1_X : 第一个lut中的点
    //pd2_X : 第二个lut中的点
    if(m_count == 0)
        return;

    for(int i = 0; i < m_count; i++)
    {
        float pd1_x = *(&m_pdata1[0][0] + i*3);
        float pd1_y = *(&m_pdata1[0][0] + i*3 + 1);
        float pd1_z = *(&m_pdata1[0][0] + i*3 + 2);
        float pd2_x = *(&m_pdata2[0][0] + i*3);
        float pd2_y = *(&m_pdata2[0][0] + i*3 + 1);
        float pd2_z = *(&m_pdata2[0][0] + i*3 + 2);
        float percent = (float)m_slider->value()/(float)100;

        float new_x = pd1_x+(pd2_x-pd1_x)*percent;
        float new_y = pd1_y+(pd2_y-pd1_y)*percent;
        float new_z = pd1_z+(pd2_z-pd1_z)*percent;

        ptrToDataArray->setPosition(QVector3D(new_x,new_y,new_z));
        ptrToDataArray++;

    }

    //更新数据
    item->dataProxy()->resetArray(dataArray);


}



int scatter::undersampling(QString path,float *pdata)
{
    int t_count =0;
    cv::Mat img =  cv::imread(path.toStdString(), cv::IMREAD_UNCHANGED);
    for (int nrow = 0; nrow < img.rows; nrow++)
    {
        if(nrow%8 == 0 || nrow==0){
        for (int ncol = 0; ncol < img.cols; ncol++)
        {
            if(ncol%4 ==0 || ncol == 0){
            cv::Vec3f color_value = img.at<cv::Vec3f>(nrow, ncol);                      // 取像素点
            std::cout << float(color_value[0]) << " " << float(color_value[1]) << " " << float(color_value[2]) << std::endl;        // 打印RGB
            *(pdata + t_count*3 + 0) = float(color_value[0]);
            *(pdata + t_count*3 + 1) = float(color_value[1]);
            *(pdata + t_count*3 + 2) = float(color_value[2]);
            t_count++;
            }
        }

        }
    }
    return t_count;
}

void scatter::getlut2(QString path1, QString path2)
{



    m_dataArray1->resize(m_itemCount);
    QScatterDataItem *ptrToDataArray1 = &m_dataArray1->first();
    int num1;
//    num1 = undersampling(path1,&m_pdata1[0][0]);
    num1 = undersampling(path1,&m_pdata1[0][0]);

    std::cout << num1 << std::endl;


    for(int i = 0; i < num1; i++)
    {
        QVector3D ret;
        ret.setX(*(&m_pdata1[0][0] + i*3));
        ret.setY(*(&m_pdata1[0][0] + i*3 + 1));
        ret.setZ(*(&m_pdata1[0][0] + i*3 + 2));
        ptrToDataArray1->setPosition(ret);
        ptrToDataArray1++;
    }

    m_dataArray2->resize(m_itemCount);
    QScatterDataItem *ptrToDataArray2 = &m_dataArray2->first();
    int num2;
    num2 = undersampling(path2,&m_pdata2[0][0]);

    std::cout << num2 << std::endl;


    if(num1 == num2)
        m_count = num1;
    else
        qDebug()<<"Two LUT file size not equal!!";

    QScatterDataProxy *proxy = new QScatterDataProxy;
    QScatter3DSeries *series = new QScatter3DSeries(proxy);
    series->setItemLabelFormat(QStringLiteral("@xTitle: @xLabel @yTitle: @yLabel @zTitle: @zLabel"));
    series->setMeshSmooth(m_smooth);
    m_graph->addSeries(item);
    item->dataProxy()->resetArray(m_dataArray1);
}
