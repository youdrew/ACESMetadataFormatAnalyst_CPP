#include "amfanalyst.h"
#include "ui_amfanalyst.h"

amfanalyst::amfanalyst(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::amfanalyst)
{
    ui->setupUi(this);

    //信号和槽的机制

    //AnalysisModification Panel / Pipeline / LMT
    // Slope
    connect(ui->Pipeline_RSlope_horizontalSlider,&QAbstractSlider::valueChanged,this,&amfanalyst::refreshCDLValueSendFrom_slotslider);
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

}

amfanalyst::~amfanalyst()
{
    delete ui;
}


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


