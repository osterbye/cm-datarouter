!include(../local_conf.pri): error(local_conf.pri file not found)
include(protobuf.pri)
include(src/pubnub/pubnub.pri)
include(src/rc/rc.pri)
include(src/camerahandler/camerahandler.pri)

QT += core sql
QT -= gui

CONFIG += qt c++11 console
CONFIG -= app_bundle

TARGET = datarouter
TEMPLATE = app

# sysroot flags
QMAKE_CFLAGS -= --sysroot=$$[QT_SYSROOT]
QMAKE_CFLAGS += --sysroot=$${YOCTO_BUILD_DIR}/tmp/sysroots/apalis-imx6
QMAKE_CXXFLAGS -= --sysroot=$$[QT_SYSROOT]
QMAKE_CXXFLAGS += --sysroot=$${YOCTO_BUILD_DIR}/tmp/sysroots/apalis-imx6
QMAKE_LFLAGS -= --sysroot=$$[QT_SYSROOT]
QMAKE_LFLAGS += --sysroot=$${YOCTO_BUILD_DIR}/tmp/sysroots/apalis-imx6

# files
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
    MessageDefinitions.pb

# build directories
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

# install
target.path = /usr/bin
INSTALLS += target
