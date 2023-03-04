#ifndef FORM3_H
#define FORM3_H

#include <QMainWindow>
#include "ui_form3.h"

extern int dynamic,
radio_ch;

namespace Ui {
class Form3;
}

class Form3 : public QMainWindow
{
    Q_OBJECT

public:
    explicit Form3(QWidget *parent = 0);
    ~Form3();

public slots:
    void radio1(){
        radio_ch=0;
    }
    void radio2(){
        radio_ch=1;
    }
    void radio3(){
        radio_ch=2;
    }
    void start_pause()
    {
        dynamic=!dynamic;
        if (dynamic)
            this->ui->dynbut->setText("Пауза");
        else
            this->ui->dynbut->setText("Старт");
    }

private slots:

private:
    Ui::Form3 *ui;
};

#endif // FORM3_H
