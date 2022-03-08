#ifndef ANALYST2_H
#define ANALYST2_H

#include <QMainWindow>

namespace Ui {
class Analyst_Test;
}

class Analyst_Test : public QMainWindow
{
    Q_OBJECT

public:
    explicit Analyst_Test(QWidget *parent = nullptr);
    ~Analyst_Test();

private:
    Ui::Analyst_Test *ui;
};

#endif // ANALYST2_H
