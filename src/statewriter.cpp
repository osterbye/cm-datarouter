#include "statewriter.h"
#include "pbmessenger.h"
#include "MessageDefinitions.pb.h"
#include <QtSql>

using namespace google::protobuf;

StateWriter::StateWriter(QObject *parent, QString filename) : QObject(parent)
{
    // connect to SQLite db specified in filename argument
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(filename);
    if (m_db.open())
        qDebug("SQL connection OK");
    else
        qCritical("SQL file connection error");

    // prepare SQL query for inserting status updates into table
    m_stateUpdateQuery = QSqlQuery(m_db);
    StatusUpdate statusUpdate;
    QStringList fieldList = getFieldNames(&statusUpdate);
    QString columnNames  = fieldList.join(',');
    QString placeholders = ":" + fieldList.join(", :");
    m_queryTemplate = QStringLiteral("INSERT INTO measurements (%1) VALUES (%2)").arg(columnNames).arg(placeholders);
    // We should now have template like this: "INSERT INTO measurements (timestamp, speed) VALUES (:timestamp, :speed)"
}

StateWriter::~StateWriter()
{
    m_db.close();
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
    m_stateUpdateQuery.prepare(m_queryTemplate);

    // iterate through all fields: extract and bind values if field exists in message, otherwise bind null value
    int fieldCount = desc->field_count();
    for (int i = 0; i < fieldCount; i++) {
        const FieldDescriptor *field = desc->field(i);
            QVariant fieldValue = PBMessenger::getMessageField(&status, field);
        QString fieldName = QString::fromStdString(":" + field->name());
        m_stateUpdateQuery.bindValue(fieldName, fieldValue);
    }
    // the query is prepared, attempt to execute and report any errors
    if (!m_stateUpdateQuery.exec()) {
        qCritical("Could not execute query");
        qDebug() << "\t" << getLastExecutedQuery(m_stateUpdateQuery);
        qDebug() << m_stateUpdateQuery.lastError();
        qDebug("Most likely the SQL table column names are different from ProtoBuf message field names");
    } else
        qDebug() << "Status update stored in SQL db";
    m_stateUpdateQuery.finish();
}
