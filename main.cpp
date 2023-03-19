#include <QApplication>

#include "mainwindow.h"

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

    MainWindow m(programMode);
    m.show();

    //w->setWindowFlags(Qt::FramelessWindowHint);
    //w->centralWidget()->layout()->setContentsMargins(1,1,1,1);
    // 1,1,1,1 because somewhere 0,0,0,0 doesn't work
    // this code cuts frame around program

    return a.exec();
}
