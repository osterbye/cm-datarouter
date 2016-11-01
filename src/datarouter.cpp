#include "datarouter.h"
#include "logging.h"
#include "pubnub_spiri.h"

/* TODO:
 *  SPI handler
 *  receiving commands from CMD engine (dbus)
 *  forwarding to Calc/checker? (dbus)
 */

Datarouter::Datarouter(QObject *parent) : QObject(parent), m_busDevice(this), m_messenger(this),
    m_stateWriter(this)
{
    m_pubnubHandler = new Pubnub_spiri(this);

    connect(m_pubnubHandler, SIGNAL(cmdRequestDoorLock(bool)), &m_messenger , SLOT(cmdRequestDoorLock(bool)));
    connect(&m_messenger, SIGNAL(sendCmdRequest(QByteArray)), &m_busDevice, SLOT(sendMessage(QByteArray)));
    connect(&m_stateWriter, SIGNAL(sendStatusToServer(QJsonObject)), m_pubnubHandler, SLOT(sendStatus(QJsonObject)));

    QObject::connect(&m_busDevice, &SpiBus::newMessageReceived, &m_messenger,
                     &PBMessenger::receiveMessage);
    //QObject::connect(m_busDevice, &SpiBus::readyRead, &PBMessager.receiveInput);
    QObject::connect(&m_messenger, &PBMessenger::newStatusReceived, &m_stateWriter,
                     &StateWriter::receiveStatus);
    //QObject::connect(messenger, &PBMessenger::newStatusUpdate, &calc.receiveStatus);
    LOG_INFO("Datarouter operational");
}

Datarouter::~Datarouter()
{
    LOG_INFO("Datarouter exiting");
    google::protobuf::ShutdownProtobufLibrary();
}
