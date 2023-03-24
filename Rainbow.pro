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
    settingswindow.cpp \
    wavelength.cpp \
    \
    scenes/scene0.cpp \
    scenes/scene1.cpp \
    scenes/scene2.cpp \
    scenes/scene3.cpp \
    scenes/scene4.cpp \
    scenes/scenebase.cpp

HEADERS  += \
    beam.h \
    glwidget.h \
    \
    mainwindow.h \
    settingswindow.h \
    wavelength.h \
    \
    scenes/scene0.h \
    scenes/scene1.h \
    scenes/scene2.h \
    scenes/scene3.h \
    scenes/scene4.h \
    scenes/scenebase.h

FORMS    += \
    mainwindow.ui \
    settingswindow.ui

TRANSLATIONS = rainbow_ru.ts \
               rainbow_fr.ts
