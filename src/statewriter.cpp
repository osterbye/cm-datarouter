#include <QtSql>
#include "MessageDefinitions.pb.h"
#include <google/protobuf/stubs/common.h>
#include "statewriter.h"

using namespace google::protobuf;

const QString filename = "../status_updates.sqlite";

StateWriter::StateWriter(QObject *parent) : QObject(parent)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("/home/josip/spiri/cm-datarouter/status_updates.sqlite");
    if (db.open())
        qDebug("SQL connection OK");
    else
        qCritical("SQL file connection error");
    stateUpdateQuery = QSqlQuery(db);

    StatusUpdate statusUpdate;
    QStringList fieldList = getFieldNames(&statusUpdate);
    QString columnNames  = fieldList.join(',');
    QString placeholders = ":" + fieldList.join(", :");
    queryTemplate = QStringLiteral("INSERT INTO measurements (%1) VALUES (%2)").arg(columnNames).arg(
                        placeholders);
    // We should now have template like this: "INSERT INTO measurements (timestamp, speed) VALUES (:timestamp, :speed)"
}

StateWriter::~StateWriter()
{
    db.close();
}

//// Inject all binded values into query to reach final query string
/** WARNING: to be used only for debug output - no protection against SQL injection! */
QString StateWriter::getLastExecutedQuery(const QSqlQuery &query)
{
    QString str = query.lastQuery();
    QMapIterator<QString, QVariant> it(query.boundValues());
    while (it.hasNext()) {
        it.next();
        str.replace(it.key(), it.value().toString());
    }
    return str;
}

//// Construct list of all possible fields in PB message
QStringList StateWriter::getFieldNames(google::protobuf::Message *message)
{
    const Descriptor *desc = message->GetDescriptor();
    //message->GetDescriptor();
    int fieldCount = desc->field_count();

    QStringList fieldList;

    // walk through field list and extract field names
    for (int i = 0; i < fieldCount; i++) {
        const FieldDescriptor *field = desc->field(i);
        fieldList.append(QString::fromStdString(field->name()));
    }
    return fieldList;
}

QVariant StateWriter::getMessageField(const google::protobuf::Message * message, const FieldDescriptor * field)
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

//// Receive status update and write to database
void StateWriter::receiveStatus(StatusUpdate status)
{
    const Descriptor *desc = status.GetDescriptor();
    stateUpdateQuery.prepare(queryTemplate);

    // iterate through all fields: extract and bind values if field exists in message, otherwise bind null value
    int fieldCount = desc->field_count();
    for (int i = 0; i < fieldCount; i++) {
        const FieldDescriptor *field = desc->field(i);
        QString fieldName = QString::fromStdString(":" + field->name());
        stateUpdateQuery.bindValue(fieldName, getMessageField(&status, field));
    }
    // the query is prepared, attempt to execute and report any errors
    if (!stateUpdateQuery.exec()) {
        qCritical("Could not execute query");
        qDebug() << "\t" << getLastExecutedQuery(stateUpdateQuery);
        qDebug() << stateUpdateQuery.lastError();
        qDebug("Most likely the SQL table column names are different from ProtoBuf message field names");
    }
    stateUpdateQuery.finish();
}
