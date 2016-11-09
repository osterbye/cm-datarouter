#ifndef PUBNUBHANDLER_H
#define PUBNUBHANDLER_H

#include <QObject>
#include <QList>

class PubNubChannel;
class PubNubSubscriber;
class QJsonDocument;

class PubNubHandler : public QObject
{
    Q_OBJECT
public:
    explicit PubNubHandler(QString keyPublish, QString keySubscribe, QObject *parent = 0);
    ~PubNubHandler();

    void subscribe(const QString &channelName, const PubNubSubscriber *receiver);
    void unsubscribe(const QString &channelName, const PubNubSubscriber *receiver);
    bool sendMessage(const QString &channelName, QJsonDocument const &message);

private:
    int getChannelIndex(const QString &channelName);

private:
    QList<PubNubChannel*> m_channelPointers;
    const QString m_keySubscribe;
    const QString m_keyPublish;
};

#endif // PUBNUBHANDLER_H
