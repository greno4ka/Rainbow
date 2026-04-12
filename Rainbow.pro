#-------------------------------------------------
#
# Project created by QtCreator 2014-01-01T15:56:37
#
#-------------------------------------------------

QT       += core gui widgets opengl openglwidgets
win32: QT += openglwidgets

LIBS +=  -lGLU
win32: LIBS += -lopengl32

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

TARGET = Rainbow
TEMPLATE = app

SOURCES += main.cpp \
    beam.cpp \
    glwidget.cpp \
    \
    glwidget3d.cpp \
    mainwindow.cpp \
    scenes/scene1.cpp \
    scenes/scene2.cpp \
    scenes/scene3.cpp \
    scenes/scene4.cpp \
    scenes/scene5.cpp \
    scenes/scenex.cpp \
    settingswindow.cpp \
    wavelength.cpp \
    \
    scenes/scenebase.cpp

HEADERS  += \
    beam.h \
    glwidget.h \
    \
    glwidget3d.h \
    mainwindow.h \
    scenes/scene1.h \
    scenes/scene2.h \
    scenes/scene3.h \
    scenes/scene4.h \
    scenes/scene5.h \
    scenes/scenex.h \
    settingswindow.h \
    wavelength.h \
    \
    scenes/scenebase.h

FORMS    += \
    mainwindow.ui \
    settingswindow.ui

TRANSLATIONS = rainbow_en.ts \
               rainbow_ru.ts \
               rainbow_fr.ts

RESOURCES += \
    images/images.qrc \
    styles/styles.qrc

DISTFILES += \
    images/double-rainbow-1000.jpg
