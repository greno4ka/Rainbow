#-------------------------------------------------
#
# Project created by QtCreator 2014-01-01T15:56:37
#
#-------------------------------------------------

QT       += core gui widgets opengl openglwidgets
win32: QT += openglwidgets

LIBS +=  -lGLU -lqwt-qt6
win32: LIBS += -lopengl32

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

TARGET = Rainbow
TEMPLATE = app

SOURCES += main.cpp \
    beam.cpp \
    glwidget.cpp \
    glwidget3d.cpp \
    plotFactory.cpp \
    windows/MainWindow/mainwindow.cpp \
    windows/MainWindow/mainwindow_interface.cpp \
    windows/MainWindow/mainwindow_page0.cpp \
    windows/MainWindow/mainwindow_page1.cpp \
    windows/MainWindow/mainwindow_page2.cpp \
    windows/MainWindow/mainwindow_page3.cpp \
    windows/MainWindow/mainwindow_page4.cpp \
    windows/MainWindow/mainwindow_page5.cpp \
    scenes/scene1.cpp \
    scenes/scene2.cpp \
    scenes/scene3.cpp \
    scenes/scene4.cpp \
    scenes/scene5.cpp \
    scenes/scene6.cpp \
    scenes/scene7.cpp \
    scenes/scenex.cpp \
    scenes/scenebase.cpp \
    settingswindow.cpp \
    wavelength.cpp

HEADERS  += \
    beam.h \
    config.h \
    glwidget.h \
    glwidget3d.h \
    plotFactory.h \
    windows/MainWindow/mainwindow.h \
    scenes/scene1.h \
    scenes/scene2.h \
    scenes/scene3.h \
    scenes/scene4.h \
    scenes/scene5.h \
    scenes/scene6.h \
    scenes/scene7.h \
    scenes/scenex.h \
    scenes/scenebase.h \
    settingswindow.h \
    wavelength.h

FORMS    += \
    windows/MainWindow/mainwindow.ui \
    settingswindow.ui

TRANSLATIONS = rainbow_en.ts \
               rainbow_ru.ts \
               rainbow_fr.ts

RESOURCES += \
    styles/styles.qrc \
    images/images.qrc

INCLUDEPATH += \
    $$PWD \
    /usr/include/qt6/qwt \
