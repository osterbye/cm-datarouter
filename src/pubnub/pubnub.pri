QT       += network

INCLUDEPATH +=  $$PWD \
                $$PWD/c-core/core \
                $$PWD/c-core/qt

DEPENDPATH +=   $$PWD/c-core/core

# Spiri PubNub handling
SOURCES +=  $$PWD/pubnubchannel.cpp \
            $$PWD/pubnubhandler.cpp

HEADERS +=  $$PWD/pubnubchannel.h \
            $$PWD/pubnubhandler.h \
            $$PWD/pubnubsubscriber.h


# PubNub Qt wrapper
SOURCES +=  $$PWD/c-core/qt/pubnub_qt.cpp

HEADERS +=  $$PWD/c-core/qt/pubnub_config.h \
            $$PWD/c-core/qt/pubnub_internal.h \
            $$PWD/c-core/qt/pubnub_qt.h


# PubNub C-core
SOURCES +=  $$PWD/c-core/core/pubnub_ccore.c \
            $$PWD/c-core/core/pubnub_assert_std.c \
            $$PWD/c-core/core/pubnub_json_parse.c \
            $$PWD/c-core/core/pubnub_helper.c
