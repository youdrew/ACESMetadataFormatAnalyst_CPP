#ifndef AMFANALYST_H
#define AMFANALYST_H

#include <QMainWindow>
#include "ui_amfanalyst.h"

namespace Ui {
class amfanalyst;
}

class amfanalyst : public QMainWindow
{
    Q_OBJECT

public:
    explicit amfanalyst(QWidget *parent = nullptr);
    ~amfanalyst();

private:
    Ui::amfanalyst *ui;


public slots:      //槽函数的声明 1 一些组件使用到的槽函数

    // AnalysisModification Panel / Pipeline / LMT
    void refreshCDLValueSendFrom_slotslider();
    void refreshCDLValueSendFrom_slotDoubleSpinbox();
    void slotslider_DoubleSpinBox(QSlider* a,QDoubleSpinBox* b);
    void slotDoubleSpinbox_slider(QSlider* a,QDoubleSpinBox* b);




};

#endif // AMFANALYST_H
