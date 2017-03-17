#include "remotecontrol.h"
#include "mavlinkparser.h"
#include "rclogging.h"

RemoteControl::RemoteControl(QString host, quint16 port, QObject *parent) :
    QObject(parent), m_host(host), m_port(port)
{
    m_parser = new MAVLinkParser(this);
    connect(m_parser, SIGNAL(heartbeat()), this, SLOT(heartbeat()));
    connect(m_parser, SIGNAL(control(float,float)), this, SIGNAL(control(float,float)));
    connect(m_parser, SIGNAL(control(float,float)), this, SLOT(controlCmd(float,float)));

    connect(m_parser, SIGNAL(packageLoss(quint16)), this, SIGNAL(packageLoss(quint16)));
    connect(this, SIGNAL(mavlinkMessage(QByteArray)), m_parser, SLOT(communicationReceive(QByteArray)));

    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(m_socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(m_socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    m_lastThrottle = 0.0;
    m_lastAngle = 0.0;
    m_heartbeatTimer.start();

    initConnection();
}

void RemoteControl::initConnection()
{
    m_socket->connectToHost(m_host, m_port);
    LOG_DEBUG("Connecting to " << m_host << " port " << m_port);
}

void RemoteControl::connected() {
    LOG_DEBUG("Connected to " << m_host << " port " << m_port);
}

void RemoteControl::disconnected() {
    m_socket->deleteLater();
    LOG_DEBUG("Disconnected");
}

void RemoteControl::error(QAbstractSocket::SocketError err) {
    LOG_DEBUG("Error: " << err);
    m_socket->reset();
    initConnection();
}

void RemoteControl::readyRead() {
    QByteArray buffer;
    if (m_socket->bytesAvailable()) {
        buffer = m_socket->readAll();
        //LOG_DEBUG("Got Data: \n\t" << buffer);
        emit mavlinkMessage(buffer);
    }
}

void RemoteControl::heartbeat()
{
    if (m_heartbeatTimer.restart() < 3000) {
        emit control(m_lastThrottle, m_lastAngle);
    }
}

void RemoteControl::controlCmd(float throttle, float angle)
{
    m_lastThrottle = throttle;
    m_lastAngle = angle;
    m_heartbeatTimer.start();
}
