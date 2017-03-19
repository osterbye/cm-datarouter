#include "datarouter.h"
#include "logging.h"
#include "pubnub_spiri.h"
#include "rc/remotecontrol.h"
#include "camerahandler/camerahandler.h"

/* TODO:
 *  SPI handler
 *  receiving commands from CMD engine (dbus)
 *  forwarding to Calc/checker? (dbus)
 */

Datarouter::Datarouter(QObject *parent) : QObject(parent), m_busDevice(this), m_messenger(this),
    m_stateWriter(this)
{
    m_pubnubHandler = new Pubnub_spiri(this, "pub-c-38db667a-fc13-4740-bbfc-c2a9298c1b22", "sub-c-edf80ad4-912f-11e6-bb6b-0619f8945a4f");
    m_rc = new RemoteControl("staging.drivr.com", 14551, this);
    m_camera = new CameraHandler(this);

    connect(m_rc, SIGNAL(startCameraStream()), m_camera, SLOT(startStreaming()));
    connect(m_rc, SIGNAL(control(float,float)), &m_messenger, SLOT(rcControl(float,float)));

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
