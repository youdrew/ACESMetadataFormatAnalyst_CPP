#include "analyst.h"
#include "ui_analyst2.h"

Analyst_Test::Analyst_Test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Analyst_Test)
{
    ui->setupUi(this);
}

Analyst_Test::~Analyst_Test()
{
    delete ui;
}
