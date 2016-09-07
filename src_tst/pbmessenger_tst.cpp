#include "pbmessenger_tst.h"

void PBMessenger_Tst::initTestCase()
{ // called once before first test case
    qRegisterMetaType<StatusUpdate>();
    qRegisterMetaType<CommandRequest>();
    qRegisterMetaType<CommandResponse>();
}

void PBMessenger_Tst::init()
{ // called before each test
    messenger = new PBMessenger();
    spyStatus = new QSignalSpy(messenger, SIGNAL(newStatusReceived(StatusUpdate)));
    spyRequest = new QSignalSpy(messenger, SIGNAL(newCommandRequestReceived(CommandRequest)));
    spyResponse = new QSignalSpy(messenger, SIGNAL(newCommandResponseReceived(CommandResponse)));
}

void PBMessenger_Tst::cleanup()
{ // called after each test
    delete messenger;
    delete spyStatus;
    delete spyRequest;
    delete spyResponse;
}

void PBMessenger_Tst::emptyMessage()
{
    QByteArray array; // empty
    messenger->receiveMessage(array);
    QCOMPARE(spyStatus->count(),   0);
    QCOMPARE(spyRequest->count(),  0);
    QCOMPARE(spyResponse->count(), 0);
}

void PBMessenger_Tst::statusMessage()
{
    ContainerMessage cm;
    cm.mutable_statusupdate();
    QByteArray array(cm.SerializeAsString().c_str());
    messenger->receiveMessage(array);
    QCOMPARE(spyStatus->count(),   1);
    QCOMPARE(spyRequest->count(),  0);
    QCOMPARE(spyResponse->count(), 0);
}

void PBMessenger_Tst::requestMessage()
{
    ContainerMessage cm;
    cm.mutable_commandrequest();
    QByteArray array(cm.SerializeAsString().c_str());
    messenger->receiveMessage(array);
    QCOMPARE(spyStatus->count(),   0);
    QCOMPARE(spyRequest->count(),  1);
    QCOMPARE(spyResponse->count(), 0);
}

void PBMessenger_Tst::responseMessage()
{
    ContainerMessage cm;
    cm.mutable_commandresponse();
    QByteArray array(cm.SerializeAsString().c_str());
    messenger->receiveMessage(array);
    QCOMPARE(spyStatus->count(),   0);
    QCOMPARE(spyRequest->count(),  0);
    QCOMPARE(spyResponse->count(), 1);
}
