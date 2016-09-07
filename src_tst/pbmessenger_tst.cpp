#include "pbmessenger_tst.h"

void PBMessenger_Tst::initTestCase()
{ // called once before first test case
    qRegisterMetaType<StatusUpdate>();
    qRegisterMetaType<CommandRequest>();
    qRegisterMetaType<CommandResponse>();
}

void PBMessenger_Tst::init()
{ // called before each test
    m_messenger = new PBMessenger();
    m_spyStatus = new QSignalSpy(m_messenger, SIGNAL(newStatusReceived(StatusUpdate)));
    m_spyRequest = new QSignalSpy(m_messenger, SIGNAL(newCommandRequestReceived(CommandRequest)));
    m_spyResponse = new QSignalSpy(m_messenger, SIGNAL(newCommandResponseReceived(CommandResponse)));
}

void PBMessenger_Tst::cleanup()
{ // called after each test
    delete m_messenger;
    delete m_spyStatus;
    delete m_spyRequest;
    delete m_spyResponse;
}

void PBMessenger_Tst::emptyMessage()
{
    QByteArray array; // empty
    m_messenger->receiveMessage(array);
    QCOMPARE(m_spyStatus->count(),   0);
    QCOMPARE(m_spyRequest->count(),  0);
    QCOMPARE(m_spyResponse->count(), 0);
}

void PBMessenger_Tst::statusMessage()
{
    ContainerMessage cm;
    cm.mutable_statusupdate();
    QByteArray array(cm.SerializeAsString().c_str());
    m_messenger->receiveMessage(array);
    QCOMPARE(m_spyStatus->count(),   1);
    QCOMPARE(m_spyRequest->count(),  0);
    QCOMPARE(m_spyResponse->count(), 0);
}

void PBMessenger_Tst::requestMessage()
{
    ContainerMessage cm;
    cm.mutable_commandrequest();
    QByteArray array(cm.SerializeAsString().c_str());
    m_messenger->receiveMessage(array);
    QCOMPARE(m_spyStatus->count(),   0);
    QCOMPARE(m_spyRequest->count(),  1);
    QCOMPARE(m_spyResponse->count(), 0);
}

void PBMessenger_Tst::responseMessage()
{
    ContainerMessage cm;
    cm.mutable_commandresponse();
    QByteArray array(cm.SerializeAsString().c_str());
    m_messenger->receiveMessage(array);
    QCOMPARE(m_spyStatus->count(),   0);
    QCOMPARE(m_spyRequest->count(),  0);
    QCOMPARE(m_spyResponse->count(), 1);
}
