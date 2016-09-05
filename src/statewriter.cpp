#include "statewriter.h"
#include "pbmessenger.h"
#include "MessageDefinitions.pb.h"
#include <QtSql>

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
    queryTemplate = QStringLiteral("INSERT INTO measurements (%1) VALUES (%2)").arg(columnNames).arg(placeholders);
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
QStringList StateWriter::getFieldNames(const google::protobuf::Message *message)
{
    const Descriptor *desc = message->GetDescriptor();
    int fieldCount = desc->field_count();

    QStringList fieldList;

    // walk through field list and extract field names
    for (int i = 0; i < fieldCount; i++) {
        const FieldDescriptor *field = desc->field(i);
        fieldList.append(QString::fromStdString(field->name()));
    }
    return fieldList;
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
            QVariant fieldValue = PBMessenger::getMessageField(&status, field);
        QString fieldName = QString::fromStdString(":" + field->name());
        stateUpdateQuery.bindValue(fieldName, fieldValue);
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
