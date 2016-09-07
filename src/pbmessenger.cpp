#include "pbmessenger.h"
#include <QVariant>
#include <QDebug>

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


//// Geet field value from any PB message and return as QVariant
QVariant PBMessenger::getMessageField(const Message * message, const FieldDescriptor * field)
{
    const Reflection *refl = message->GetReflection();
    const FieldDescriptor::Type field_type = field->type();
    const QVariant nullValue = QVariant(QVariant::String); // type doesn't matter as SQLite is dynamically typed

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
            variant = QVariant(refl->GetInt64(*message, field));
            break;
        case FieldDescriptor::Type::TYPE_SINT32:
            variant = QVariant(refl->GetInt32(*message, field));
            break;
        case FieldDescriptor::Type::TYPE_SINT64:
            variant = QVariant(refl->GetInt64(*message, field));
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
            qCritical("Unsupported field type %s", field->name().c_str());
            variant = nullValue;
        }
    } else {
        variant = nullValue;
    }
    return variant;
}
