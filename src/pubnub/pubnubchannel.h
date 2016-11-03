#ifndef PUBNUBCHANNEL_H
#define PUBNUBCHANNEL_H

#include <QObject>
//#include <QJsonDocument>
//#include <QJsonObject>
#include "pubnub_qt.h"

class PubNubChannel : public QObject
{
    Q_OBJECT
public:
    explicit PubNubChannel(const QString name, const QString keypub, const QString keysub, QObject *parent = 0);
    ~PubNubChannel();

    QString name();
    bool sendMessage(const QString &message);

signals:
    //void pnMessageReceived(QString const &channel, QJsonDocument const message);
    //void cmdRequestDoorLock(bool lock);
    void newMessage(const QString &channel, QStringList const message);

public slots:
    //void pnSendMessage(QString const &channel, QJsonDocument const message);
    //void sendStatus(QJsonObject status);
    //void sendMessage(const QString &message);

private slots:
    //void printMessageReceived(QString const &channel, QJsonDocument const message);
    void onPublish(pubnub_res result);
    void onSubscribe(pubnub_res result);

private:
    const QString m_name;
    //const QString m_keypub;
    //const QString m_keysub;
    QScopedPointer<pubnub_qt> d_pb_publish;
    QScopedPointer<pubnub_qt> d_pb_subscribe;

};

#endif // PUBNUBCHANNEL_H
