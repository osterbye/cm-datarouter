#include "pubnubchannel.h"

extern "C" {
#include "pubnub_helper.h"
}

#define DEBUG_PUBNUB

#ifdef DEBUG_PUBNUB
#include "../logging.h"
#else
#define LOG_DEBUG(x)
#define LOG_INFO(x)
#define LOG_WARN(x)
#define LOG_CRITICAL(x)
#endif
PubNubChannel::PubNubChannel(const QString name, const QString keypub, const QString keysub, QObject *parent) :
    QObject(parent),
    m_name(name)
{
    LOG_DEBUG("Creating PubNub channel '" << name << "'");
    d_pb_publish.reset(new pubnub_qt(keypub,keysub));
    connect(d_pb_publish.data(), SIGNAL(outcome(pubnub_res)), this, SLOT(onPublish(pubnub_res)));

    d_pb_subscribe.reset(new pubnub_qt(keypub,keysub));
    connect(d_pb_subscribe.data(), SIGNAL(outcome(pubnub_res)), this, SLOT(onSubscribe(pubnub_res)));
    d_pb_subscribe->subscribe(name);
}

PubNubChannel::~PubNubChannel()
{
    LOG_DEBUG("Destroying PubNub channel '" << m_name << "'");
}

QString PubNubChannel::name()
{
    return m_name;
}

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

void PubNubChannel::onPublish(pubnub_res result)
{
    LOG_DEBUG("Publish result: '" << pubnub_res_2_string(result) << "', response: " << d_pb_publish->last_publish_result() << "\n");
}

void PubNubChannel::onSubscribe(pubnub_res result)
{
    if (PNR_OK != result) {
        LOG_DEBUG("Subscribe failed entering " << pubnub_res_2_string(result));
    } else {
        QStringList messages = d_pb_subscribe->get_all();
        LOG_DEBUG("Channel " << m_name << " recieved: " << messages);
        emit newMessage(m_name, messages);
    }

    result = d_pb_subscribe->subscribe(m_name);
    if (result != PNR_STARTED) {
        LOG_WARN("subscribe failed new" << pubnub_res_2_string(result));
    }
}
