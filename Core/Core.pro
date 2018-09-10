#-------------------------------------------------
#
# Project created by QtCreator 2018-08-26T09:54:14
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ViewFrame
TEMPLATE = app

TRANSLATIONS += $${TARGET}_zh_CN.ts
#RC_ICONS += $${TARGET}.ico

CONFIG(debug, debug|release) {

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

SOURCES += main.cpp \
    widgets/mainwindow.cpp \
    protocol/datastruct.cpp \
    protocol/protocoldata.cpp \
    file/globalconfigfile.cpp \
    file/xmlparse.cpp \
    global.cpp

HEADERS  += \
    widgets/mainwindow.h \
    constants.h \
    protocol/datastruct.h \
    protocol/protocoldata.h \
    file/globalconfigfile.h \
    file/xmlparse.h \
    global.h

FORMS    += mainwindow.ui

win32-msvc2013{
    LIBS+= ../Lib/Base.lib
    LIBS+= ../Lib/Network.lib
}

win32-g++{
    LIBS+= -L../Lib/ -lBase
    QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder
}

unix{
    LIBS+= -L../Lib/ -lNetwork -lBase
}

INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/../../$${TARGET}/

RESOURCES += \
    res.qrc
