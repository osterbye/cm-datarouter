!include(../local_conf.pri): error(local_conf.pri file not found)
include(protobuf.pri)
include(src/pubnub/pubnub.pri)

QT += core sql
QT -= gui

CONFIG += qt c++11

TARGET = Datarouter
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/datarouter.cpp \
    src/pbmessenger.cpp \
    src/mockbus.cpp \
    src/statewriter.cpp \
    src/spibus.cpp \
    src/pubnub_spiri.cpp

HEADERS += \
    src/datarouter.h \
    src/pbmessenger.h \
    src/mockbus.h \
    src/statewriter.h \
    src/logging.h\
    src/spibus.h \
    src/pubnub_spiri.h

DISTFILES += \
    protobuf.pri \
    MessageDefinitions.pb

release:DESTDIR = release
release:OBJECTS_DIR = release/.obj
release:MOC_DIR = release/.moc
release:RCC_DIR = release/.rcc
release:UI_DIR = release/.ui

debug:DESTDIR = debug
debug:OBJECTS_DIR = debug/.obj
debug:MOC_DIR = debug/.moc
debug:RCC_DIR = debug/.rcc
debug:UI_DIR = debug/.ui
