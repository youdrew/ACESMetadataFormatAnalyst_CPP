/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt Charts module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "view.h"
#include "ui_amfanalyst.h"


void View::SetSeries(QLineSeries *series1,QLineSeries *series2,QLineSeries *series3,QString path1,QString path2)
{
    cv::Mat img1 =  imread(path1.toStdString(), cv::IMREAD_UNCHANGED);
    cv::Mat img2 =  imread(path2.toStdString(), cv::IMREAD_UNCHANGED);

    for (size_t i = 0; i < 32; i++)
    {
        cv::Vec3f redx = img1.at<cv::Vec3f>(0, i);
        float redxvalue = float(redx[2]);

        cv::Vec4f redy = img2.at<cv::Vec4f>(0, i);
        float redyvalue = float(redy[2]);

        cv::Vec3f bluex = img1.at<cv::Vec3f>(i, 0);
        float bluexvalue = float(bluex[0]);

        cv::Vec4f bluey = img2.at<cv::Vec4f>(i, 0);
        float blueyvalue = float(bluey[0]);

        cv::Vec3f greenx = img1.at<cv::Vec3f>(0, i*32);
        float greenxvalue = float(greenx[1]);

        cv::Vec4f greeny = img2.at<cv::Vec4f>(0, i * 32);
        float greenyvalue = float(greeny[1]);

        series1->append(bluexvalue, blueyvalue);
        series2->append(greenxvalue, greenyvalue);
        series3->append(redxvalue, redyvalue);
    }
}

View::View(QWidget *parent)
    : QGraphicsView(new QGraphicsScene, parent),
      m_coordX(0),
      m_coordY(0),
      m_chart(0)
{
    setDragMode(QGraphicsView::NoDrag);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    QString path1 = "/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/TestFile/Pic_LUT/lut_origin.tif";
    QString path2 = "/Volumes/Youdrew_WorkDisk_01/ACESMetadataFormatAnalyst_CPP/GraduationTestClassify/Out/LUT_2022_0516_000819.tif";

    // chart
    m_chart = new QChart;
    m_chart->maximumSize();
    m_chart->setTitle("Hover the line to show callout. Click the line to make it stay");
    m_chart->legend()->hide();

    QSplineSeries *series = new QSplineSeries;
    QSplineSeries *series2 = new QSplineSeries;
    QSplineSeries *series3 = new QSplineSeries;
    SetSeries(series,series2,series3,path1,path2);

    m_chart->addSeries(series);
    m_chart->addSeries(series2);
    m_chart->addSeries(series3);

    m_chart->createDefaultAxes();
    m_chart->setAcceptHoverEvents(true);

    setRenderHint(QPainter::Antialiasing);
    scene()->addItem(m_chart);

    m_coordX = new QGraphicsSimpleTextItem(m_chart);
    m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height());
    m_coordX->setText("X: ");
    m_coordY = new QGraphicsSimpleTextItem(m_chart);
    m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height());
    m_coordY->setText("Y: ");

    connect(series, &QLineSeries::clicked, this, &View::keepCallout);
    connect(series, &QLineSeries::hovered, this, &View::tooltip);

    connect(series2, &QSplineSeries::clicked, this, &View::keepCallout);
    connect(series2, &QSplineSeries::hovered, this, &View::tooltip);

    connect(series3, &QSplineSeries::clicked, this, &View::keepCallout);
    connect(series3, &QSplineSeries::hovered, this, &View::tooltip);


    this->setMouseTracking(true);
}

void View::resizeEvent(QResizeEvent *event)
{
    if (scene()) {
        scene()->setSceneRect(QRect(QPoint(0, 0), event->size()));
         m_chart->resize(event->size());
         m_coordX->setPos(m_chart->size().width()/2 - 50, m_chart->size().height() - 20);
         m_coordY->setPos(m_chart->size().width()/2 + 50, m_chart->size().height() - 20);
         const auto callouts = m_callouts;
         for (Callout *callout : callouts)
             callout->updateGeometry();
    }
    QGraphicsView::resizeEvent(event);
}

void View::mouseMoveEvent(QMouseEvent *event)
{
    m_coordX->setText(QString("X: %1").arg(m_chart->mapToValue(event->pos()).x()));
    m_coordY->setText(QString("Y: %1").arg(m_chart->mapToValue(event->pos()).y()));
    QGraphicsView::mouseMoveEvent(event);
}

void View::keepCallout()
{
    m_callouts.append(m_tooltip);
    m_tooltip = new Callout(m_chart);
}

void View::tooltip(QPointF point, bool state)
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
