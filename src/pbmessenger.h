#ifndef PBMESSENGER_H
#define PBMESSENGER_H

#include <QObject>
#include <QIODevice>
#include "MessageDefinitions.pb.h"

class PBMessenger : public QObject
{
    Q_OBJECT
public:
    explicit PBMessenger(QObject *parent = 0);
    ~PBMessenger();

    static QVariant getMessageField(const google::protobuf::Message *message,
                                    const google::protobuf::FieldDescriptor *field);

private:

signals:
    void newStatusReceived(StatusUpdate status);
    void newCommandRequestReceived(CommandRequest request);
    void newCommandResponseReceived(CommandResponse response);

    void sendCmdRequest(QByteArray payload);

public slots:
    void receiveMessage(QByteArray msg);
    void cmdRequestDoorLock(bool lock);
};

#endif // PBMESSENGER_H
