QT       += core widgets xml
QT       += sql

CONFIG += c++11

TARGET = Base
CONFIG -= app_bundle

DEFINES += BASE_LIBRARY

TEMPLATE = lib

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


HEADERS += \
    pluginmanager/observer.h \
    pluginmanager/pluginmanager.h \
    pluginmanager/subject.h \
    actionmanager/action.h \
    actionmanager/actioncontainer.h \
    actionmanager/actionmanager.h \
    actionmanager/command.h \
    actionmanager/id.h \
    base_global.h \
    messagetype.h \
    component/rcomponent.h \
    util/fileutils.h \
    util/rbuffer.h \
    util/regexp.h \
    util/rlog.h \
    util/rsingleton.h \
    util/rutil.h \
    util/scaleswitcher.h \
    selfwidget/dialogproxy.h \
    selfwidget/rbutton.h \
    sql/autotransaction.h \
    sql/database.h \
    sql/databasemanager.h \
    sql/datatable.h \
    sql/rpersistence.h \
    protocol/datastruct.h \
    constants.h

SOURCES += \
    pluginmanager/observer.cpp \
    pluginmanager/pluginmanager.cpp \
    pluginmanager/subject.cpp \
    actionmanager/action.cpp \
    actionmanager/actioncontainer.cpp \
    actionmanager/actionmanager.cpp \
    actionmanager/command.cpp \
    actionmanager/id.cpp \
    component/rcomponent.cpp \
    util/fileutils.cpp \
    util/rbuffer.cpp \
    util/regexp.cpp \
    util/rlog.cpp \
    util/rsingleton.cpp \
    util/rutil.cpp \
    util/scaleswitcher.cpp \
    selfwidget/dialogproxy.cpp \
    selfwidget/rbutton.cpp \
    sql/autotransaction.cpp \
    sql/database.cpp \
    sql/databasemanager.cpp \
    sql/datatable.cpp \
    sql/rpersistence.cpp \
    protocol/datastruct.cpp
