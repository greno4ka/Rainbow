#include <QApplication>
#include <QLayout>

#include "mainwindow.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    int programMode=0;

    if (argc > 1)
        programMode=atoi(argv[1]);

    QTranslator *translator = nullptr;
    translator = new QTranslator();
    // translator.load(QString("rainbow_") + QString("ru"));
    a.installTranslator(translator);

    MainWindow m(programMode);
    // thin bounds around window please
    m.centralWidget()->layout()->setContentsMargins(1,1,1,1);
    m.setTranslator(translator);
    m.show();

    a.exec();
    delete translator;
    return 0;
}
