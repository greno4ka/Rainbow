#ifndef FORM0_H
#define FORM0_H

#include <QMainWindow>
#include "ui_form0.h"
#include "line.h"

extern int N,
radio_ch;

typedef QList<Line> Lst;
extern Lst Beams;
extern double R,d,wl;

namespace Ui {
class Form0;
}

class Form0 : public QMainWindow
{
    Q_OBJECT

public:
    explicit Form0(QWidget *parent = 0);
    ~Form0();

public slots:

    void incN(){
        if (N<=4) N++; // after 4 beam dissapears
    }
    void decN(){
        if (N>=-1) N--; // -1 is critical minimum
    }

    void radio1(){
        radio_ch=0;
    }
    void radio2(){
        radio_ch=1;
    }
    void radio3(){
        radio_ch=2;
    }

    void dist_slider(int id)
    {
    d=id/100.0;
    this->ui->spin1->setValue(d);
    }
    void wave_slider(int id)
    {
    wl=id;
    this->ui->spin2->setValue(id);
    }

    void dist_spin(double id)
    {
        d=id;
        this->ui->slider1->setValue(id*100);
    }
    void wave_spin(int id)
    {
        wl=id;
        this->ui->slider2->setValue(id);
    }

    void add_beam(){
        Line Input(0,1,-d*R,wl);
        Beams.push_back (Input);
    }

    void clearscr(){
        N=-1;
        Beams.clear();
    }

private:
    Ui::Form0 *ui;
};

#endif // FORM0_H
