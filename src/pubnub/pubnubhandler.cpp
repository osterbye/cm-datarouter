/*!
 * \class PubNubHandler
 * \brief Handler class keeping track of PubNub channels and subscribers.
 */
#include "pubnubhandler.h"
#include "pubnubchannel.h"
#include "pubnubsubscriber.h"
#include <QJsonDocument>

/*! If defined debug messages will be written to log */
#define DEBUG_PUBNUBHANDLER

#ifdef DEBUG_PUBNUBHANDLER
#include "../logging.h"
#else
#define LOG_DEBUG(x)
#define LOG_INFO(x)
#define LOG_WARN(x)
#define LOG_CRITICAL(x)
#endif

/*!
 * \brief Constructor
 * \param keyPublish The PubNub publish key for the project.
 * \param keySubscribe The PubNub subscribe key for the project.
 * \param parent Parent class.
 */
PubNubHandler::PubNubHandler(QString keyPublish, QString keySubscribe,
                             QObject *parent)
    : QObject(parent), m_keySubscribe(keySubscribe), m_keyPublish(keyPublish)
{

}

/*!
 * \brief Destructor
 */
PubNubHandler::~PubNubHandler()
{
    LOG_DEBUG("Destroying PubNubHandler");
}

/*!
 * \brief Subscribe to receive messages posted on a specific PubNub channel.
 * \param channelName Name of the PubNub channel to receive messages from.
 * \param receiver The class which should receive the messages.
 *
 * If channel doesn't exist it will be created.
 */
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

/*!
 * \brief Unsubscribe to messages posted on a specific PubNub channel.
 * \param channelName Name of the PubNub channel to unsubscribe from.
 * \param receiver The class which should no longer receive messages.
 *
 * If there are no more subscribers for a specific channel, the appropriate
 * PubNubChannel instance wil stop listening on the channel.
 */
void PubNubHandler::unsubscribe(const QString &channelName, const PubNubSubscriber *receiver)
{
    int channelIndex = getChannelIndex(channelName);
    if (channelIndex >= 0 && channelIndex < m_channelPointers.count())
        m_channelPointers.at(channelIndex)->unsubscribe(receiver);
}

/*!
 * \brief Send a message to a specific PubNub channel.
 * \param channel Name of the PubNub channel the message should be posted to.
 * \param message Message which should be posted to channel, in JSON format.
 * \return Succeeded or failed to send message.
 */
bool PubNubHandler::sendMessage(const QString &channelName, const QJsonDocument &message)
{
    int channelIndex = getChannelIndex(channelName);
    if (channelIndex < 0) {
        m_channelPointers.append(new PubNubChannel(channelName, m_keyPublish, m_keySubscribe));
        channelIndex = getChannelIndex(channelName);
    }
    if (channelIndex >= 0 && channelIndex < m_channelPointers.count())
        return m_channelPointers.at(channelIndex)->sendMessage(message.toJson(QJsonDocument::Compact));

    return false;
}

/*!
 * \brief Helper function to get a channels index in the pointer list.
 * \param channelName Name of the PubNub channel to find.
 * \return Index number of the channel or -1 in case the channel doesn't exist.
 */
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
