QT += network

INCLUDEPATH += $$PWD \
    $$PWD/mavlink/include

SOURCES += \
    $$PWD/remotecontrol.cpp \
    $$PWD/mavlinkparser.cpp

HEADERS += \
    $$PWD/remotecontrol.h \
    $$PWD/mavlinkparser.h \
    $$PWD/rclogging.h

