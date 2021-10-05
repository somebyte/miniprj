TEMPLATE = app
TARGET = selector
INCLUDEPATH += .
include(./selector.pri)

QT += core gui widgets

# Input
HEADERS += src/mainwindow.hpp
SOURCES += src/main.cpp src/mainwindow.cpp

RESOURCES += selector.qrc

