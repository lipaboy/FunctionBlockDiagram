#-------------------------------------------------
#
# Project created by QtCreator 2022-11-29T15:27:05
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FunctionBlockDiagram
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17
win32: QMAKE_CXXFLAGS += /std:c++17
unix:  QMAKE_CXXFLAGS += -std=c++17

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    widgets/PinItem.cpp \
    models/FunctionGraph.cpp \
    widgets/BlockItem.cpp \
    widgets/FunctionBlockDiagramWidget.cpp \
    widgets/FunctionBlockItem.cpp \
    widgets/ConnectionItem.cpp

HEADERS += \
    mainwindow.h \
    widgets/PinItem.h \
    models/FunctionGraph.h \
    widgets/BlockItem.h \
    widgets/FunctionBlockDiagramWidget.h \
    widgets/FunctionBlockItem.h \
    widgets/ConnectionItem.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
