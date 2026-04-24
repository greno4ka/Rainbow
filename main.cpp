#include <QApplication>
#include <QGuiApplication>
#include <QLayout>

#include "windows/MainWindow/mainwindow.h"

int main(int argc, char **argv)
{
    QApplication a(argc, argv);
    int programMode=0;

    if (argc > 1)
        programMode=atoi(argv[1]);

    a.setWindowIcon(QIcon(":/rainbow.ico"));

    QTranslator translator;
    a.installTranslator(&translator);

    MainWindow m(programMode, &translator);
    // thin bounds around window please
    m.centralWidget()->layout()->setContentsMargins(1,1,1,1);
    m.show();

    a.exec();
    return 0;
}
