#ifndef SCATTER_H
#define SCATTER_H

#include <QObject>
#include <QtDataVisualization/q3dscatter.h>
#include <QtDataVisualization/qabstract3dseries.h>
#include <QSlider>

#include <opencv4/opencv2/opencv.hpp>
#include <opencv4/opencv2/imgproc/imgproc.hpp>
#include <opencv4/opencv2/highgui/highgui.hpp>


//using namespace QtDataVisualization;

class scatter : public QObject
{
    Q_OBJECT

public:
    explicit scatter(Q3DScatter *scatter,QSlider *m_slider);
    void parse_lut(QScatterDataItem *ptrToDataArray,QString path,float *pdata);
    void getlut(QString path1,QString path2);
    void repaint();
    int undersampling(QString path,float *pdata);       //下采样的函数

    void getlut2(QString path1,QString path2);          // new one


signals:

public slots:

private:
    Q3DScatter *m_graph;            // 坐标系的。
    QSlider *m_slider;              // 与之关联的滑块
    QScatterDataArray *m_dataArray1;// 第一份点的坐标数据
    QScatterDataArray *m_dataArray2;// 第二份点的坐标数据
    int m_fontSize;                 // 字体大小
    bool m_smooth;                  // 数据是否要平滑
    int m_itemCount;                // 预设的点的数量
    int m_count;
    float m_curveDivider;
    float m_pdata1[1500][3];         // 储存的是，被下采样之后的三维点数据
    float m_pdata2[1500][3];         //路径2被下采样后的三维点数据
    QScatter3DSeries *item;

};

#endif // SCATTER_H
