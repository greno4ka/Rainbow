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
    forms/form0.cpp \
    forms/form1.cpp \
    forms/form2.cpp \
    forms/form3.cpp \
    forms/form4.cpp \
    mainwindow.cpp \
    scenes/scene0.cpp \
    scenes/scene3.cpp

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
    wavelength.h \
    \
    forms/form0.h \
    forms/form1.h \
    forms/form2.h \
    forms/form3.h \
    forms/form4.h \
    scenes/scene0.h \
    scenes/scene3.h

FORMS    += \
    forms/form0.ui \
    forms/form1.ui \
    forms/form2.ui \
    forms/form3.ui \
    forms/form4.ui \
    mainwindow.ui
