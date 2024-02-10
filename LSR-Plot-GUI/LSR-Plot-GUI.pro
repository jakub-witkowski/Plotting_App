QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#QMAKE_CFLAGS += `root-config --cflags`
QMAKE_LIBDIR_FLAGS += `root-config --cflags --libs`

TARGET = LSR-Plot-GUI
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++17

INCLUDEPATH += "/snap/root-framework/928/usr/local/include/"

HEADERS += mainwindow.h \
           tdata.h \
           tplot.h \
           tpolynomial.h \
           tsegment.h

SOURCES += main.cpp \
           mainwindow.cpp \
           tdata.cpp \
           tplot.cpp \
           tpolynomial.cpp \
           tsegment.cpp

FORMS += mainwindow.ui
