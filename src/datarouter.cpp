#include "datarouter.h"
#include "logging.h"
#include <QtCore>

/* TODO:
 *  SPI handler
 *  receiving commands from CMD engine (dbus)
 *  forwarding to Calc/checker? (dbus)
 */

Datarouter::Datarouter(QObject *parent) : QObject(parent), m_busDevice(this), m_messenger(this), m_stateWriter(this)
{
    QObject::connect(&m_busDevice, &MockBus::newMessageReceived, &m_messenger, &PBMessenger::receiveMessage);
    //QObject::connect(m_busDevice, &SpiBus::readyRead, &PBMessager.receiveInput);
    QObject::connect(&m_messenger, &PBMessenger::newStatusReceived, &m_stateWriter, &StateWriter::receiveStatus);
    //QObject::connect(messenger, &PBMessenger::newStatusUpdate, &calc.receiveStatus);
    LOG_INFO("Datarouter operational");
}

Datarouter::~Datarouter()
{
    LOG_INFO("Datarouter exiting");
    google::protobuf::ShutdownProtobufLibrary();
}
