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
    src/pbmessenger.cpp \
    src/statewriter.cpp \


HEADERS += \
    src_tst/pbmessenger_tst.hpp \
    src_tst/statewriter_tst.hpp \
    src/pbmessenger.h \
    src/statewriter.h \

DISTFILES += \
    protobuf.pri \
    MessageDefinitions.pb

DESTDIR = tests
OBJECTS_DIR = tests/.obj
MOC_DIR = tests/.moc
RCC_DIR = tests/.rcc
UI_DIR = tests/.ui
