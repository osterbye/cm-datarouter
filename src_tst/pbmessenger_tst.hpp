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
public:
    PBMessenger_Tst(QObject *parent = 0) : QObject(parent)
    {}
private slots:
    void initTestCase()
    { // called once before first test case
        qRegisterMetaType<StatusUpdate>();
        qRegisterMetaType<CommandRequest>();
        qRegisterMetaType<CommandResponse>();
    }

    void init()
    { // called before each test
        messenger = new PBMessenger();
        spyStatus = new QSignalSpy(messenger, SIGNAL(newStatusReceived(StatusUpdate)));
        spyRequest = new QSignalSpy(messenger, SIGNAL(newCommandRequestReceived(CommandRequest)));
        spyResponse = new QSignalSpy(messenger, SIGNAL(newCommandResponseReceived(CommandResponse)));
    }

    void cleanup()
    { // called after each test
        delete messenger;
        delete spyStatus;
        delete spyRequest;
        delete spyResponse;
    }

    // actual tests

    void emptyMessage()
    {
        QByteArray array; // empty
        messenger->receiveMessage(array);
        QCOMPARE(spyStatus->count(),   0);
        QCOMPARE(spyRequest->count(),  0);
        QCOMPARE(spyResponse->count(), 0);
    }

    void statusMessage()
    {
        ContainerMessage cm;
        cm.mutable_statusupdate();
        QByteArray array(cm.SerializeAsString().c_str());
        messenger->receiveMessage(array);
        QCOMPARE(spyStatus->count(),   1);
        QCOMPARE(spyRequest->count(),  0);
        QCOMPARE(spyResponse->count(), 0);
    }

    void requestMessage()
    {
        ContainerMessage cm;
        cm.mutable_commandrequest();
        QByteArray array(cm.SerializeAsString().c_str());
        messenger->receiveMessage(array);
        QCOMPARE(spyStatus->count(),   0);
        QCOMPARE(spyRequest->count(),  1);
        QCOMPARE(spyResponse->count(), 0);
    }

    void responseMessage()
    {
        ContainerMessage cm;
        cm.mutable_commandresponse();
        QByteArray array(cm.SerializeAsString().c_str());
        messenger->receiveMessage(array);
        QCOMPARE(spyStatus->count(),   0);
        QCOMPARE(spyRequest->count(),  0);
        QCOMPARE(spyResponse->count(), 1);
    }
#if 0
    void messageField()
    {
        StatusUpdate status = StatusUpdate();
        status.Clear();
        status.set_timestamp(42);
        const google::protobuf::Descriptor * descriptor = status.GetDescriptor();
        const google::protobuf::FieldDescriptor * fieldDescriptor = descriptor->FindFieldByLowercaseName("timestamp");
        QVariant value = messenger->getMessageField(&status, fieldDescriptor);
        QVERIFY(value.isValid());
        qDebug() << "type is " << value.typeName();
        QVERIFY(value.toInt() == 42);
    }
#endif

private:
    PBMessenger * messenger;
    QSignalSpy * spyStatus;
    QSignalSpy * spyRequest;
    QSignalSpy * spyResponse;
};

#endif // PBMESSENGER_TST_H
