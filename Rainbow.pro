#-------------------------------------------------
#
# Project created by QtCreator 2014-01-01T15:56:37
#
#-------------------------------------------------

QT       += core gui widgets opengl openglwidgets

win32: LIBS += -lopengl32

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

TARGET = Rainbow
TEMPLATE = app


SOURCES += main.cpp \
    beam.cpp \
    glwidget.cpp \
    \
    mainwindow.cpp \
    scenes/scene0.cpp \
    scenes/scene1.cpp \
    scenes/scene2.cpp \
    scenes/scene3.cpp \
    scenes/scenebase.cpp \
    wavelength.cpp

HEADERS  += \
    beam.h \
    display.h \
    glwidget.h \
    draw_beam.h \
    k.h \
    mainwindow.h \
    recalc.h \
    questions.h \
    mode3.h \
    draw3sc.h \
    scenes/scenebase.h \
    wavelength.h \
    \
    scenes/scene0.h \
    scenes/scene1.h \
    scenes/scene2.h \
    scenes/scene3.h

FORMS    += \
    mainwindow.ui
