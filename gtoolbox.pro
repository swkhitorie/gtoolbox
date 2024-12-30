#-------------------------------------------------
#
# Project created by QtCreator 2024-12-18T13:41:01
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = gtoolbox
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

CONFIG += c++11
CONFIG += warn_off
QMAKE_CXXFLAGS = -Wall
QMAKE_CXXFLAGS += -Wno-unused-parameter
QMAKE_CXXFLAGS += -Wno-unused-function
QMAKE_CXXFLAGS += -Wno-unused-but-set-variable

SOURCES += ./src/main.cpp
SOURCES += ./src/qtb.cpp
SOURCES += ./src/uiforms/multifileformattrans.cpp
SOURCES += ./src/uiforms/serialprotocoldebugger.cpp
SOURCES += ./src/tools/filetransformat.cpp
SOURCES += ./src/rtklib/rtkcmn.c
SOURCES += ./src/rtklib/rtcm3.c

INCLUDEPATH += ./src/rtklib

HEADERS += ./src/qtb.h
HEADERS += ./src/uiforms/multifileformattrans.h
HEADERS += ./src/uiforms/serialprotocoldebugger.h
HEADERS += ./src/tools/filetransformat.h
HEADERS += ./src/rtklib/rtklib.h

FORMS += ./uiforms/qtb.ui
FORMS += ./uiforms/multifileformattrans.ui
FORMS += ./uiforms/serialprotocoldebugger.ui

RC_ICONS = ./resources/icons/qtoolbox.ico

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
