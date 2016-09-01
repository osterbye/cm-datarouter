#include "pbmessenger.h"
#include <QFile>

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

PBMessenger::PBMessenger(QObject *parent) : QObject(parent)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
}

PBMessenger::~PBMessenger()
{
    google::protobuf::ShutdownProtobufLibrary();
}

void PBMessenger::receiveMessage(QByteArray msg)
{
    ContainerMessage container;
    container.ParseFromArray(msg, msg.size());

    if (container.has_statusupdate()) {
        qInfo("Received Status Update message");
        emit newStatusReceived(container.statusupdate());
    } else if (container.has_commandrequest()) {
        qInfo("Received Command Request message");
        emit newCommandRequestReceived(container.commandrequest());
    } else if (container.has_commandresponse()) {
        qInfo("Received Command Response message");
        emit newCommandResponseReceived(container.commandresponse());
    } else {
        qWarning("Unsupported message format!");
    }
    //qDebug() << " timestamp " << container.has_statusupdate() << container.has_commandrequest();
}
