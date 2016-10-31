#include "pubnub_spiri.h"

extern "C" {
#include "pubnub_helper.h"
}

#define DEBUG_PUBNUB

#ifdef DEBUG_PUBNUB
#include "logging.h"
#define PN_DBG(x)   LOG_DEBUG("[PubNub] " << x)
#else
#define PN_DBG(x)
#endif
Pubnub_spiri::Pubnub_spiri(QObject *parent, QString keypub, QString keysub) :
    QObject(parent),
    keypub(keypub),
    keysub(keysub)
{
    PN_DBG("resetPubnub()");
    d_pb_publish.reset(new pubnub_qt(keypub,keysub));
    connect(d_pb_publish.data(), SIGNAL(outcome(pubnub_res)), this, SLOT(onPublish(pubnub_res)));

    d_pb_subscribe.reset(new pubnub_qt(keypub,keysub));
    connect(d_pb_subscribe.data(), SIGNAL(outcome(pubnub_res)), this, SLOT(onSubscribe(pubnub_res)));
    d_pb_subscribe->subscribe("chn-test"); // TODO - figure out a way of keeping track of subscribed channels

    connect(this, SIGNAL(pnMessageReceived(QString,QJsonDocument)), this, SLOT(printMessageReceived(QString,QJsonDocument)) );
}

Pubnub_spiri::~Pubnub_spiri()
{
    PN_DBG("closingPubnub");
}

void Pubnub_spiri::pnSendMessage(QString const &channel, QJsonDocument const message)
{
    QString messagestring(message.toJson(QJsonDocument::Compact));

    PN_DBG("sendMessage(" << channel << ", " << messagestring << ")");
    pubnub_res result = d_pb_publish->publish(channel, messagestring);
    if (result != PNR_STARTED) {
        PN_DBG("Publish failed");
    }
}

void Pubnub_spiri::onPublish(pubnub_res result)
{
    PN_DBG("Publish result: '" << pubnub_res_2_string(result) << "', response: " << d_pb_publish->last_publish_result() << "\n");
}

void Pubnub_spiri::onSubscribe(pubnub_res result)
{
    PN_DBG("subscribe");
    if (PNR_OK != result) {
        PN_DBG("subscribe failed entering " << pubnub_res_2_string(result));
    } else {
        PN_DBG("subscribe parsing");
        QList<QString> msg = d_pb_subscribe->get_all();
        for (int i = 0; i < msg.size(); ++i) {
            PN_DBG("subscribe message " + msg[i] + '\n');
            QJsonDocument json = QJsonDocument::fromJson(msg[i].toUtf8());
            QJsonObject object = json.object();

            emit pnMessageReceived("chn-test", json);

        }
    }


    result = d_pb_subscribe->subscribe("chn-test");
    if (result != PNR_STARTED) {
        PN_DBG("subscribe failed new" << pubnub_res_2_string(result));
    }
}


void Pubnub_spiri::printMessageReceived(QString const &channel, QJsonDocument const message)
{
    PN_DBG("printMessageReceived");
    PN_DBG(channel);
    PN_DBG(message.toJson(QJsonDocument::Compact));
}
