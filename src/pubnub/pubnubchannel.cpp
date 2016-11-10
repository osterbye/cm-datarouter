/*!
 * \class PubNubChannel
 * \brief Class representing a single PubNub channel.<BR>
 * Do not use this class directly. Use the PubNubHandler to access channels.
 *
 * An instance of PubNubChannel provides read and write access to a single
 * PubNub channel.<BR>
 * Classes can subscribe to automatically receive messages arriving on the
 * specific channel by inheriting from the PubNubSubscriber class.
 */
#include "pubnubchannel.h"
#include "pubnubsubscriber.h"

extern "C" {
#include "pubnub_helper.h"
}

/*! If defined debug messages will be written to log */
#define DEBUG_PUBNUBCHANNEL

#ifdef DEBUG_PUBNUBCHANNEL
#include "../logging.h"
#else
#define LOG_DEBUG(x)
#define LOG_INFO(x)
#define LOG_WARN(x)
#define LOG_CRITICAL(x)
#endif

/*!
 * \brief Constructor
 * \param name The name of the PubNub channel.
 * \param keypub The publish key, passed on from PubNubHandler.
 * \param keysub The subscribe key, passed on from PubNubHandler.
 * \param parent Parent class. Should always be a PubNubHandler instance.
 */
PubNubChannel::PubNubChannel(const QString name, const QString keypub, const QString keysub, QObject *parent) :
    QObject(parent),
    m_name(name),
    m_subscribers(0)
{
    LOG_DEBUG("Creating PubNub channel '" << name << "'");
    d_pb_publish.reset(new pubnub_qt(keypub,keysub));
    connect(d_pb_publish.data(), SIGNAL(outcome(pubnub_res)), this, SLOT(onPublish(pubnub_res)));

    d_pb_subscribe.reset(new pubnub_qt(keypub,keysub));
    connect(d_pb_subscribe.data(), SIGNAL(outcome(pubnub_res)), this, SLOT(onSubscribe(pubnub_res)));
    d_pb_subscribe->subscribe(name);
}

/*!
 * \brief Destructor
 */
PubNubChannel::~PubNubChannel()
{
    LOG_DEBUG("Destroying PubNub channel '" << m_name << "'");
}

/*!
 * \brief Returns the name of the PubNub channel accessed by this instance.
 * \return Channel name.
 */
const QString &PubNubChannel::name()
{
    return m_name;
}

/*!
 * \brief Subscribe to PubNub channel.
 * \param receiver Pointer to receiving class.
 *
 * Multiple classes can subscribe to the same channel.
 */
void PubNubChannel::subscribe(const PubNubSubscriber *receiver)
{
    connect(this, SIGNAL(newMessage(QString,QStringList)), receiver, SLOT(PubNubReceive(QString,QStringList)));
    if (1 == ++m_subscribers)
        d_pb_subscribe->subscribe(m_name);
}

/*!
 * \brief Unsubscribe from PubNub channel.
 * \param receiver Pointer to class which should no longer receive messages.
 */
void PubNubChannel::unsubscribe(const PubNubSubscriber *receiver)
{
    disconnect(this, SIGNAL(newMessage(QString,QStringList)), receiver, SLOT(PubNubReceive(QString,QStringList)));
    if (0 > --m_subscribers)
        m_subscribers = 0;
}

/*!
 * \brief Send a message to PubNub channel.
 * \param message The message to send.
 * \return Succeeded or failed to send message.
 */
bool PubNubChannel::sendMessage(const QString &message)
{
    LOG_DEBUG("sendMessage(" << m_name << ", " << message << ")");
    pubnub_res result = d_pb_publish->publish(m_name, message);
    if (result != PNR_STARTED) {
        LOG_WARN("Publish failed on channel " << m_name);
        return false;
    }
    return true;
}

/*!
 * \brief Called on publish.
 * \param result Succeeded or failed to publish message on channel.
 */
void PubNubChannel::onPublish(pubnub_res result)
{
    LOG_DEBUG("Publish result: '" << pubnub_res_2_string(result) << "', response: " << d_pb_publish->last_publish_result() << "\n");
}

/*!
 * \brief Called on subscribe.
 * \param result Succeeded or failed to subscribe to channel.
 *
 * Will also be called on channel timeout. In case there are still subscribers
 * for the channel, PubNubChannel will re-subscribe to the channel.
 */
void PubNubChannel::onSubscribe(pubnub_res result)
{
    if (PNR_OK != result) {
        LOG_DEBUG("Subscribe failed entering " << pubnub_res_2_string(result));
    } else {
        QStringList messages = d_pb_subscribe->get_all();
        LOG_DEBUG("Channel " << m_name << " recieved: " << messages);
        emit newMessage(m_name, messages);
    }

    if (m_subscribers > 0) {
        result = d_pb_subscribe->subscribe(m_name);
        if (result != PNR_STARTED) {
            LOG_WARN("subscribe failed new" << pubnub_res_2_string(result));
        }
    }
}

/*!
 * \fn void PubNubChannel::newMessage(const QString &channel, QStringList const message)
 * \brief Signal emitting new messages.
 * \param channel The name of the PubNub channel the message was received on.
 * \param message The message received.
 *
 * Do not connect directly to this signal. Use the subscribe function.
 */
