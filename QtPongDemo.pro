#-------------------------------------------------
#
# Project created by QtCreator 2012-09-11T13:40:10
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = QtPongDemo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gamewindow.cpp \
    glwidget.cpp

HEADERS  += mainwindow.h \
    gamewindow.h \
    glwidget.h

FORMS    += mainwindow.ui \
    gamewindow.ui

unix:LIBS += -lglut -lGL -lGLU

RESOURCES += \
    resources.qrc
