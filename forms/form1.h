#ifndef FORM1_H
#define FORM1_H

#include <QMainWindow>
#include "ui_form1.h"
#include "beam.h"

extern int radio_ch;
extern double d,wl;

extern Lst Beams;

namespace Ui {
class Form1;
}

class Form1 : public QMainWindow
{
    Q_OBJECT
private:

    void initializeBeams(double d, double R, double wl)
    {
        Beams.clear();
        Beam Input1(0,1,-(d+0.02)*R,wl),
             Input2(0,1,-(d-0.03)*R,wl);
        Beams.push_back (Input1);
        Beams.push_back (Input2);
    }

public:
    explicit Form1(QWidget *parent = 0);
    ~Form1();

public slots:
    void radio1(){
        radio_ch=0;
    }
    void radio2(){
        radio_ch=1;
    }

    void dist_slider_d(int id)
    {
    d=id/100.0;
    this->ui->spin1->setValue(d);
    initializeBeams(d,R,wl);
    }

    void dist_spin_d(double id)
    {
        d=id;
        this->ui->slider1->setValue(id*100);
        initializeBeams(d,R,wl);
    }
    void wave_spin_d(int id)
    {
        wl=id;
        this->ui->slider2->setValue(id);
        initializeBeams(d,R,wl);
    }
    void wave_slider_d(int id)
    {
    wl=id;
    this->ui->spin2->setValue(id);
    initializeBeams(d,R,wl);
    }

    void reset()
    {
        d=0.86; // good inception with parallel beams
        wl=600;
        this->ui->spin1->setValue(d);
        this->ui->spin2->setValue(wl);
        this->ui->slider1->setValue(d*100);
        this->ui->slider2->setValue(wl);
        initializeBeams(d,R,wl);
    }

private:
    Ui::Form1 *ui;
};

#endif // FORM1_H
