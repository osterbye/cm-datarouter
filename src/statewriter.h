#ifndef STATEWRITER_H
#define STATEWRITER_H

#include <QObject>
#include <QtSql>
#include "MessageDefinitions.pb.h"

class StateWriter : public QObject
{
    Q_OBJECT
public:
    explicit StateWriter(QObject *parent = 0);
    ~StateWriter();
private:
    QSqlQuery stateUpdateQuery;
    QSqlDatabase db;
    QString queryTemplate;

    QString getLastExecutedQuery(const QSqlQuery &query);
    QStringList getFieldNames(google::protobuf::Message * message);
    QVariant getMessageField(const google::protobuf::Message *message, const google::protobuf::FieldDescriptor *field);
signals:

public slots:
    void receiveStatus(StatusUpdate status);
};

#endif // STATEWRITER_H
