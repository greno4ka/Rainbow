#include <QApplication>
#include <QMessageBox>
#include <QLayout>
#include <iostream>
#include <ctime>
#include "beam.h"
#include "forms/form0.h"
#include "forms/form1.h"
#include "forms/form2.h"
#include "forms/form3.h"
#include "forms/form4.h"

#include "mainwindow.h"

using namespace std;
Beams beams;                  // STL list of our beams

float *rnd=NULL;            // array for random values in 3rd scene
int N=0,    // number of reformations and reflections of beam in the drop
    radio_ch=0,                 // universal variable for selection something
    angle=0,                    // show only bright beams in 2nd scene
    brbeams=0,                  // show angle in 2nd scene
    dynamic=1,                  // dynamic image or not in 3rd scene
    rainbows=0,                 // 1 if we draw only rainbow beams
    m2beams=30,                 // number of beams in 2nd scene [dynamic]
    m3beams=150,                // number of beams in 3rd scene [constant]
    X=1335, Y=768;              // Screen resolution IN CURRENT moment
double R=10,                // Drop's radius
       wl=600,              // current WaveLength
       d=0.8;               // current distance
bool  // I wanted to put it in "display.h", but ups...
// These two flags are required to display degree in 2nd scene only one time
one=false, // flags for 2nd scene
two=false; // flags for 2nd scene

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    int programMode=0;  // program mode
                        // 0 - XOD JIY4A B KAnJIE
                        // 1 - PACXODUMOCTb JIY4EU
                        // 2 - PADYrA DEKAPTA
                        // 3 - AHCAM6JIb KAnEJIb (CTOXACTU4ECKUU)
                        // 4 - nOJIOCA AJIEKCAHDPA

    if (argc > 1)
        programMode=atoi(argv[1]);

    R=5;
    /// Initialization code here
//    switch (mode)
//    {
//    case 3: R=3;
//        srand(time(NULL));
//        if(!(rnd = (float*)malloc((m3beams) * sizeof(float))))
//        {
//            QMessageBox::critical(0,"Ошибка 0x0001", "Не удалось выделить память. Работа программы будет завершена");
//            a.quit();
//        }
//        break;
//    case 4: R=5;
//        {
//        double r=-0.95;
//        while (r<0.95)
//            {
//                for (int w=400; w<=760; w+=(760-400)/7)
//                {
//                    Beam Input(0,1,-r*R,w);
//                    beams.push_back (Input);
//                }
//                r+=0.01;
//            }
//        }
//        break;
//    }

    MainWindow m(programMode);
    m.show();

    //w->setWindowFlags(Qt::FramelessWindowHint);
    //w->show();
    // w->showFullScreen();
    //w->centralWidget()->layout()->setContentsMargins(1,1,1,1);
    // 1,1,1,1 because somewhere 0,0,0,0 doesn't work
    // this code cuts frame around program

    return a.exec();
}
