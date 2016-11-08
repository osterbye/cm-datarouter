#ifndef PUBNUBCHANNEL_H
#define PUBNUBCHANNEL_H

#include <QObject>
#include "pubnub_qt.h"

class PubNubSubscriber;

class PubNubChannel : public QObject
{
    Q_OBJECT
public:
    PubNubChannel(const QString name, const QString keypub, const QString keysub, QObject *parent = 0);
    ~PubNubChannel();

    const QString &name();
    void subscribe(const PubNubSubscriber *receiver);
    void unsubscribe(const PubNubSubscriber *receiver);
    bool sendMessage(const QString &message);

signals:
    void newMessage(const QString &channel, QStringList const message);

private slots:
    void onPublish(pubnub_res result);
    void onSubscribe(pubnub_res result);

private:
    const QString m_name;
    int m_subscribers;
    QScopedPointer<pubnub_qt> d_pb_publish;
    QScopedPointer<pubnub_qt> d_pb_subscribe;

};

#endif // PUBNUBCHANNEL_H
