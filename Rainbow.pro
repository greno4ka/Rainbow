#-------------------------------------------------
#
# Project created by QtCreator 2014-01-01T15:56:37
#
#-------------------------------------------------

QT       += core gui opengl
LIBS += -lglut
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Rainbow
TEMPLATE = app


SOURCES += main.cpp\
        glwidget.cpp \
    form0.cpp \
    form1.cpp \
    form3.cpp \
    form4.cpp \
    form2.cpp

HEADERS  += \
    display.h \
    glwidget.h \
    line.h \
    draw_beam.h \
    recalc.h \
    questions.h \
    mode3.h \
    draw3sc.h \
    wavelength.h \
    form0.h \
    form1.h \
    form3.h \
    form4.h \
    form2.h

FORMS    += \
    form0.ui \
    form1.ui \
    form3.ui \
    form4.ui \
    form2.ui
