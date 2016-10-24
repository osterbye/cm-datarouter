LIBS += /usr/local/lib/libprotobuf.so

PROTOS = ./intercomm/MessageDefinitions.proto

include(protobuf.pri)
QT += core sql testlib
QT -= gui

TARGET = tests
CONFIG += qt c++11
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += \
    src_tst/main.cpp \
    src_tst/pbmessenger_tst.cpp \
    src_tst/statewriter_tst.cpp \
    src/pbmessenger.cpp \
    src/statewriter.cpp \


HEADERS += \
    src/pbmessenger.h \
    src/statewriter.h \
    src_tst/pbmessenger_tst.h \
    src_tst/statewriter_tst.h

DISTFILES += \
    protobuf.pri \
    MessageDefinitions.pb

DESTDIR = tests
OBJECTS_DIR = tests/.obj
MOC_DIR = tests/.moc
RCC_DIR = tests/.rcc
UI_DIR = tests/.ui
