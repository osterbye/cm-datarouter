#ifndef STATEWRITER_H
#define STATEWRITER_H

#include <QObject>
#include <QtSql>
#include "MessageDefinitions.pb.h"

class StateWriter : public QObject
{
    Q_OBJECT
public:
    explicit StateWriter(QObject *parent = 0, QString filename = "../status_updates.sqlite");
    ~StateWriter();

private:
    QSqlDatabase m_db;
    QSqlQuery m_stateUpdateQuery;
    QString m_queryTemplate;

    QString getLastExecutedQuery(const QSqlQuery &query);
    QStringList getFieldNames(const google::protobuf::Message *message);

signals:

public slots:
    void receiveStatus(StatusUpdate status);
};

#endif // STATEWRITER_H
