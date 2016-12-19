#include "pubnubtest.h"
#include "pubnubhandler.h"
#include <QCoreApplication>
#include <QDebug>
#include <QJsonObject>
#include <QTime>

#include <QNetworkConfigurationManager>
#include <QNetworkSession>

PubNubTest::PubNubTest(QObject *parent) : PubNubSubscriber(parent)
{
    qDebug() << "PubNubTest\n----------\n";



    qDebug() << "QNetworkConfigurationManager\n....................\n";
    QNetworkConfigurationManager *net = new QNetworkConfigurationManager(this);
    qDebug() << net->capabilities();
    //qDebug() << net->allConfigurations();

    QNetworkConfiguration pppConfig;

    QList<QNetworkConfiguration> confs = net->allConfigurations();
    for (int i = 0; i < confs.count(); ++i) {
        qDebug() << confs.at(i).identifier() << " : " << confs.at(i).bearerTypeName() << ", type: " << confs.at(i).type() << ", valid: " << confs.at(i).isValid() << ", name: " << confs.at(i).name() << ", state: " << confs.at(i).state();
        if (confs.at(i).name().contains("ppp", Qt::CaseInsensitive))
            pppConfig = confs[i];
    }

    qDebug() << "pppConfig name: " << pppConfig.name();
    m_session = new QNetworkSession(pppConfig, this);
    qDebug() << "Session state: " << m_session->state();
    //connect(m_session, SIGNAL(opened()), this, SLOT(sessionOpened()));
    m_session->open();
    qDebug() << "Session state after open(): " << m_session->state();
    m_session->waitForOpened(-1);

    qDebug() << "Session is now opened. State: " << m_session->state();
    m_handler = new PubNubHandler("pub-c-38db667a-fc13-4740-bbfc-c2a9298c1b22",
                                  "sub-c-edf80ad4-912f-11e6-bb6b-0619f8945a4f",
                                  this);

    m_counter = 0;
    m_channelName = "test_channel";

    m_handler->sendMessage(m_channelName, getMessage());

    m_handler->sendMessage(m_channelName, getMessage());

    QTime t = QTime::currentTime();

    qDebug() << "|||||||||||||||||||||||||\n";

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

void PubNubTest::sessionOpened()
{
}
