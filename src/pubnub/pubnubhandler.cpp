#include "pubnubhandler.h"
#include "pubnubchannel.h"
#include "pubnubsubscriber.h"
#include <QJsonDocument>

#define DEBUG_PUBNUBHANDLER

#ifdef DEBUG_PUBNUBHANDLER
#include "../logging.h"
#else
#define LOG_DEBUG(x)
#define LOG_INFO(x)
#define LOG_WARN(x)
#define LOG_CRITICAL(x)
#endif

PubNubHandler::PubNubHandler(QString keyPublish, QString keySubscribe,
                             QObject *parent)
    : QObject(parent), m_keySubscribe(keySubscribe), m_keyPublish(keyPublish)
{

}

PubNubHandler::~PubNubHandler()
{
    LOG_DEBUG("Destroying PubNubHandler");
}

void PubNubHandler::subscribe(const QString &channelName, const PubNubSubscriber *receiver)
{
    int channelIndex = getChannelIndex(channelName);
    if (channelIndex < 0) {
        m_channelPointers.append(new PubNubChannel(channelName, m_keyPublish, m_keySubscribe));
        channelIndex = getChannelIndex(channelName);
    }
    if (channelIndex >= 0 && channelIndex < m_channelPointers.count())
        m_channelPointers.at(channelIndex)->subscribe(receiver);
}

void PubNubHandler::unsubscribe(const QString &channelName, const PubNubSubscriber *receiver)
{
    int channelIndex = getChannelIndex(channelName);
    if (channelIndex < m_channelPointers.count())
        m_channelPointers.at(channelIndex)->unsubscribe(receiver);
}

bool PubNubHandler::sendMessage(const QString &channel, const QJsonDocument &message)
{
    int channelIndex = getChannelIndex(channel);
    if (channelIndex >= 0 && channelIndex < m_channelPointers.count())
        return m_channelPointers.at(channelIndex)->sendMessage(message.toJson(QJsonDocument::Compact));

    return false;
}

int PubNubHandler::getChannelIndex(const QString &channelName)
{
    for (int i = 0; i < m_channelPointers.count(); ++i) {
        if (channelName == m_channelPointers.at(i)->name()) {
            LOG_DEBUG("Found channel " << m_channelPointers.at(i)->name() << "at index " << i);
            return i;
        }
    }
    return -1;
}
