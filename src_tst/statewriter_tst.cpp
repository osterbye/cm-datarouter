#include "statewriter_tst.h"
#include "MessageDefinitions.pb.h"
#include <QtTest>

void StateWriter_Tst::openDatabase()
{
    m_statewriter = new StateWriter();
    QVERIFY(m_statewriter->m_db.isOpen());
    QVERIFY(m_statewriter->m_db.isValid());
    delete m_statewriter;
}

void StateWriter_Tst::receiveUpdate()
{
    m_statewriter = new StateWriter(0, ":memory:");

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
        qDebug() << "\t" << m_statewriter->getLastExecutedQuery(query);
        qDebug() << query.lastError();
    }
    m_statewriter->receiveStatus(status);
    query.finish();
    query.prepare("SELECT (timestamp) FROM measurements");
    if (!query.exec()) {
        qCritical("Could not execute query");
        qDebug() << "\t" << m_statewriter->getLastExecutedQuery(query);
        qDebug() << query.lastError();
    }
    // verify test data
    QVERIFY(query.next());
    QVERIFY(query.value(0) == 42);
    QVERIFY(!query.next());
    delete m_statewriter;
}
