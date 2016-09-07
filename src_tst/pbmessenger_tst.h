#ifndef PBMESSENGER_TST_H
#define PBMESSENGER_TST_H

#include <QObject>
#include <QtTest/QtTest>
#include "src/pbmessenger.h"
#include "MessageDefinitions.pb.h"

// for QSignalSpy, to suppress warning "unable to handle parameter"
Q_DECLARE_METATYPE(StatusUpdate);
Q_DECLARE_METATYPE(CommandRequest);
Q_DECLARE_METATYPE(CommandResponse);

class PBMessenger_Tst: public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void init();
    void cleanup();

    // actual tests
    void emptyMessage();
    void statusMessage();
    void requestMessage();
    void responseMessage();


private:
    PBMessenger * messenger;
    QSignalSpy * spyStatus;
    QSignalSpy * spyRequest;
    QSignalSpy * spyResponse;
};

#endif // PBMESSENGER_TST_H
