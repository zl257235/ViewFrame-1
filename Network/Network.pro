
QT       -= gui

QT       += network
QT       += xml

TARGET = Network
TEMPLATE = lib

CONFIG += c++11

DEFINES += NETWORK_LIBRARY

CONFIG(debug, debug|release) {
#  TARGET = $$join(TARGET,,,d)           #为debug版本生成的文件增加d的后缀

  contains(TEMPLATE, "lib") {
    DESTDIR = ../Lib
    DLLDESTDIR = ../Bin
  } else {
    DESTDIR = ../Bin
  }
} else {
  contains(TEMPLATE, "lib") {
    DESTDIR = ../Lib
    DLLDESTDIR = ../Bin
  } else {
    DESTDIR = ../Bin
  }
}

SOURCES += \
    networkinterface.cpp \
    config/readxmlinfo.cpp \
    connect/socketconnectudp.cpp \
    netbuff/ccyclebuffer.cpp \
    netspace/networknamespace.cpp \
    thread/udprecvthread.cpp

HEADERS +=\
        network_global.h \
    networkinterface.h \
    config/readxmlinfo.h \
    config/usertypedef.h \
    connect/socketconnectudp.h \
    netbuff/ccyclebuffer.h \
    netspace/networknamespace.h \
    thread/udprecvthread.h

INCLUDEPATH += $$PWD/../../ViewFrame/

win32-msvc2013{
    LIBS += ../Lib/Util.lib
}

unix {
    target.path = /usr/lib
    INSTALLS += target
}
