#ifndef PUBNUBSUBSCRIBER_H
#define PUBNUBSUBSCRIBER_H

#include <QObject>

class PubNubSubscriber : public QObject
{
    Q_OBJECT
public slots:
    virtual void PubNubReceive(QString, QStringList) = 0;
};

#endif // PUBNUBSUBSCRIBER_H
