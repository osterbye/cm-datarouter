#ifndef STATEWRITER_TST_H
#define STATEWRITER_TST_H

#include <QObject>
#include <QtTest/QtTest>
#include "MessageDefinitions.pb.h"

#define private public // to access private members - for unit-testing only!
#include "src/statewriter.h"
#undef private

class StateWriter_Tst: public QObject
{
    Q_OBJECT

public:
    StateWriter_Tst(QObject *parent = 0) : QObject(parent)
    {}
private slots:
    // actual tests

    void openDatabase()
    {
        statewriter = new StateWriter();
        QVERIFY(statewriter->db.isOpen());
        QVERIFY(statewriter->db.isValid());
        delete statewriter;
    }

    void receiveUpdate()
    {
        statewriter = new StateWriter(0, ":memory:");

        // prepare test data
        StatusUpdate status;
        status.set_timestamp(42);

        // load SQLite schema to construct table in memory db
        QFile schema;
        schema.setFileName("../status_updates.schema");
        if (! schema.open(QIODevice::ReadOnly))
        {
            qDebug() << "Could not open SQLite schema";
        }
        QSqlQuery query;
        query.prepare(schema.readAll());
        if (!query.exec()) {
            qCritical("Could not create table");
            qDebug() << "\t" << statewriter->getLastExecutedQuery(query);
            qDebug() << query.lastError();
        }
        statewriter->receiveStatus(status);
        query.finish();
        query.prepare("SELECT (timestamp) FROM measurements");
        if (!query.exec()) {
            qCritical("Could not execute query");
            qDebug() << "\t" << statewriter->getLastExecutedQuery(query);
            qDebug() << query.lastError();
        }
        // verify test data
        QVERIFY(query.next());
        QVERIFY(query.value(0) == 42);
        QVERIFY(!query.next());
        delete statewriter;
    }

private:
    StateWriter * statewriter;
};

#endif // STATEWRITER_TST_H
