#ifndef PUBNUBSUBSCRIBER_H
#define PUBNUBSUBSCRIBER_H

#include <QObject>


/*!
 * \brief Base class for PubNub subscribers
 *
 * Classes that wish to receive messages via PubNub should inherit from this
 * class and implement their own PubNubReceive function.
 */
class PubNubSubscriber : public QObject
{
    Q_OBJECT
public:
    PubNubSubscriber(QObject *parent = 0) : QObject(parent){}
public slots:
    /*!
     * \brief Slot for receiving PubNub messages
     * \param channel Name of the PubNub channel the messages where received on.
     * \param messages The messages received.
     */
    virtual void PubNubReceive(QString channel, QStringList messages) = 0;
};

#endif // PUBNUBSUBSCRIBER_H
