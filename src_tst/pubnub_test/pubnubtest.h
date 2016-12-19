#ifndef PUBNUBTEST_H
#define PUBNUBTEST_H

#include "pubnubsubscriber.h"
#include <QJsonDocument>

class PubNubHandler;
class QNetworkSession;

class PubNubTest : public PubNubSubscriber
{
public:
    PubNubTest(QObject *parent = 0);

    void PubNubReceive(QString channel, QStringList messages);

private:
    QJsonDocument getMessage();

private slots:
    void sessionOpened();

private:
    PubNubHandler *m_handler;
    QNetworkSession *m_session;
    int m_counter;
    QString m_channelName;
};

#endif // PUBNUBTEST_H
