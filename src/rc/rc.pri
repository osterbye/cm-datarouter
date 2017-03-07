QT += network

INCLUDEPATH += $$PWD \
    $$PWD/mavlink/include

#DEPENDPATH += $$PWD/c-core/core

SOURCES += \
    $$PWD/remotecontrol.cpp \
    $$PWD/mavlinkparser.cpp

HEADERS += \
    $$PWD/remotecontrol.h \
    $$PWD/mavlinkparser.h \
    $$PWD/rclogging.h

