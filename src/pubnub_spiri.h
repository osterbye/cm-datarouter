#ifndef PUBNUB_SPIRI_H
#define PUBNUB_SPIRI_H

#include <QObject>
#include <QJsonDocument>
#include <QJsonObject>
#include "pubnub_qt.h"

class Pubnub_spiri : public QObject
{
    Q_OBJECT
public:
    explicit Pubnub_spiri(QObject *parent = 0,
                          QString keypub = "pub-c-06919e73-f910-4788-94f9-faf8b4c96f5f", // spiri test publish key
                          QString keysub = "sub-c-17941266-9130-11e6-a8c4-0619f8945a4f");// spiri test subscribe key
    ~Pubnub_spiri();


signals:
    void pnMessageReceived(QString const &channel, QJsonDocument const message);
    void cmdRequestDoorLock(bool lock);

public slots:
    void pnSendMessage(QString const &channel, QJsonDocument const message);
    void sendStatus(QJsonObject status);

private slots:
    void printMessageReceived(QString const &channel, QJsonDocument const message);
    void onPublish(pubnub_res result);
    void onSubscribe(pubnub_res result);

private:
    QScopedPointer<pubnub_qt> d_pb_publish;
    QScopedPointer<pubnub_qt> d_pb_subscribe;
    QString keypub;
    QString keysub;

};

#endif // PUBNUB_SPIRI_H
