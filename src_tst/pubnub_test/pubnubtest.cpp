#include "pubnubtest.h"
#include "pubnubhandler.h"
#include <QCoreApplication>
#include <QDebug>
#include <QJsonObject>
#include <QTime>

PubNubTest::PubNubTest(QObject *parent) : PubNubSubscriber(parent)
{
    qDebug() << "PubNubTest\n----------\n";

    m_handler = new PubNubHandler("pub-c-38db667a-fc13-4740-bbfc-c2a9298c1b22",
                                  "sub-c-edf80ad4-912f-11e6-bb6b-0619f8945a4f",
                                  this);

    m_counter = 0;
    m_channelName = "test_channel";

    m_handler->sendMessage(m_channelName, getMessage());

    m_handler->sendMessage(m_channelName, getMessage());

    QTime t = QTime::currentTime();

    while (true) {
        t = QTime::currentTime();
        t = t.addSecs(15);
        while (QTime::currentTime().msecsTo(t) > 0) {QCoreApplication::processEvents();}

        qDebug() << QTime::currentTime() << ": 15 seconds later...";
        m_handler->sendMessage(m_channelName, getMessage());
    }
}

void PubNubTest::PubNubReceive(QString channel, QStringList messages)
{
    qDebug() << "\n-----------\n";
    qDebug() << "Channel: " << channel << "\nMessages:\n" << messages << "\n";
    qDebug() << "\n...........\n";
}

QJsonDocument PubNubTest::getMessage()
{
    QJsonObject jsonObj;
    jsonObj.insert("counter", ++m_counter);
    return QJsonDocument(jsonObj);
}
