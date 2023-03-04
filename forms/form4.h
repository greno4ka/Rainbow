#ifndef FORM4_H
#define FORM4_H

#include <QMainWindow>

extern int radio_ch;

namespace Ui {
class Form4;
}

class Form4 : public QMainWindow
{
    Q_OBJECT

public:
    explicit Form4(QWidget *parent = 0);
    ~Form4();

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
private:
    Ui::Form4 *ui;
};

#endif // FORM4_H
