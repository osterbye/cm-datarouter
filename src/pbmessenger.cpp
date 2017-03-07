#include "pbmessenger.h"
#include "logging.h"
#include <QVariant>

using namespace google::protobuf;

PBMessenger::PBMessenger(QObject *parent) : QObject(parent)
{
    GOOGLE_PROTOBUF_VERIFY_VERSION;
}

PBMessenger::~PBMessenger()
{
}

void PBMessenger::receiveMessage(QByteArray msg)
{
    ContainerMessage container;
    container.ParseFromArray(msg, msg.size());
    if (container.has_status()) {
        LOG_INFO("Received Status Update message");
        emit newStatusReceived(container.status());
    } else if (container.has_commandrequest()) {
        LOG_INFO("Received Command Request message");
        emit newCommandRequestReceived(container.commandrequest());
    } else if (container.has_commandresponse()) {
        LOG_INFO("Received Command Response message");
        emit newCommandResponseReceived(container.commandresponse());
    } else {
        LOG_WARN("Unsupported message format! " << msg);
    }
}


//// Geet field value from any PB message and return as QVariant
QVariant PBMessenger::getMessageField(const Message *message, const FieldDescriptor *field)
{
    const Reflection *refl = message->GetReflection();
    const FieldDescriptor::Type field_type = field->type();
    const QVariant nullValue = QVariant(QVariant::String);
    // QVariant type irrelevant (SQLite is dynamically typed)

    QVariant variant;
    if (refl->HasField(*message, field)) {
        switch (field_type) {
        case FieldDescriptor::Type::TYPE_FLOAT:
            variant = QVariant(refl->GetFloat(*message, field));
            break;
        case FieldDescriptor::Type::TYPE_INT32:
            variant = QVariant(refl->GetInt32(*message, field));
            break;
        case FieldDescriptor::Type::TYPE_INT64:
            variant = QVariant((qint64)refl->GetInt64(*message, field));
            break;
        case FieldDescriptor::Type::TYPE_SINT32:
            variant = QVariant(refl->GetInt32(*message, field));
            break;
        case FieldDescriptor::Type::TYPE_SINT64:
            variant = QVariant((qint64)refl->GetInt64(*message, field));
            break;
        case FieldDescriptor::Type::TYPE_BOOL:
            variant = QVariant(refl->GetBool(*message, field));
            break;
        case FieldDescriptor::Type::TYPE_STRING:
            variant = QVariant(refl->GetString(*message, field).c_str());
            break;
        /* TODO: not sure how ENUMs are fetched from PB message
        case FieldDescriptor::Type::TYPE_ENUM:
            stateUpdateQuery.bindValue(fieldName, QString::number(refl->GetEnumValue(status, field)));
            break;*/
        default:
            LOG_CRITICAL("Unsupported field type " << field->name().c_str());
            variant = nullValue;
        }
    } else {
        variant = nullValue;
    }
    return variant;
}

void PBMessenger::cmdRequestDoorLock(bool lock)
{
    ContainerMessage * container = new ContainerMessage();
    CommandRequest * doorCmdRequest = container->mutable_commandrequest();
    doorCmdRequest->set_type(doorCmdRequest->SETDOORLOCK);
    doorCmdRequest->mutable_setdoorlock()->set_locked(lock);
    QByteArray serializedPayload = QByteArray::fromStdString(container->SerializeAsString());
    LOG_DEBUG("Sending lock/unlock command: " << serializedPayload);
    emit sendCmdRequest(serializedPayload);
}

void PBMessenger::rcHeartbeat()
{
    // TODO: should we repeat last control cmd for the heartbeat?
}

void PBMessenger::rcControl(float throttle, float angle)
{
    ContainerMessage * container = new ContainerMessage();
    CommandRequest * cmdRequest = container->mutable_commandrequest();
    cmdRequest->set_type(cmdRequest->AP102CTRL);
    cmdRequest->mutable_ap102control()->set_speed(throttle);
    cmdRequest->mutable_ap102control()->set_direction(angle);
    QByteArray serializedPayload = QByteArray::fromStdString(container->SerializeAsString());
    LOG_DEBUG("Sending rcControl command: " << serializedPayload);
    emit sendCmdRequest(serializedPayload);
}
