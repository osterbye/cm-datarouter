QT += core
QT -= gui

CONFIG += c++11

TARGET = pubnub_test
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    pubnubtest.cpp

HEADERS += \
    pubnubtest.h

include(../../src/pubnub/pubnub.pri)
