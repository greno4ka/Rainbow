#include <QApplication>
#include <QMessageBox>
#include <QLayout>
#include <ctime>
#include "mainwindow.h"

float *rnd=NULL;            // array for random values in 3rd scene
int dynamic=1;                  // dynamic image or not in 3rd scene

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
