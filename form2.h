#ifndef FORM2_H
#define FORM2_H

#include <QMainWindow>
#include "ui_form2.h"
#include "line.h"

typedef QList<Line> Lst;
extern Lst Beams;
extern int radio_ch,
m2beams,
angle,
brbeams;
extern double wl;

extern bool
one, // flags for 2nd scene
two; // flags for 2nd scene
inline void reset_flags() // using in button "add beams"
{
    one=true; two=true;
}

namespace Ui {
class Form2;
}

class Form2 : public QMainWindow
{
    Q_OBJECT

public:
    explicit Form2(QWidget *parent = 0);
    ~Form2();

public slots:

    void radio1(){
        radio_ch=0;
    }
    void radio2(){
        radio_ch=1;
    }

    void show_angle()
    {
        angle=!angle;
    }
    void show_brbeams()
    {
        brbeams=!brbeams;
    }
    void wave_spin_d(int id)
    {
        wl=id;
        this->ui->slider1->setValue(id);
        for (Lst::iterator i=Beams.begin(); i!=Beams.end(); i++)
                        if (i->w()==false) i->setwl(wl);
    }
    void wave_slider_d(int id)
    {
    wl=id;
    this->ui->spin1->setValue(id);
    for (Lst::iterator i=Beams.begin(); i!=Beams.end(); i++)
                    if (i->w()==false) i->setwl(wl);
    }

    void add_beams()
    {
        Beams.clear();
                    double h=1.0/m2beams;
                    //printf("%f\n",h);
                    for (float r=0.01; r<0.99; r+=h)
                    {
                        Line Input(0,1,-r*R,wl);
                        if (angle && radio_ch==1)
                        {
                            Input.invertz();
                            reset_flags();
                        }
                        Beams.push_back (Input);
                    }
    }

    void m2beams_change(int id)
    {
        m2beams=id;
        add_beams();
    }

    void clearscr(){
        Beams.clear();
    }

private:
    Ui::Form2 *ui;
};

#endif // FORM2_H
