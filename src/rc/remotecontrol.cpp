#include "remotecontrol.h"
#include "mavlinkparser.h"
#include <QDebug>

RemoteControl::RemoteControl(QString host, quint16 port, QObject *parent) :
    QObject(parent), m_host(host), m_port(port)
{
    m_parser = new MAVLinkParser(this);
    connect(this, SIGNAL(mavlinkMessage(QByteArray)), m_parser, SLOT(communicationReceive(QByteArray)));

    m_socket = new QTcpSocket(this);
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(connected()), this, SLOT(connected()));
    connect(m_socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(m_socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    m_socket->connectToHost(m_host, m_port);
    qDebug() << "[RC] " << "Connecting to " << m_host << " port " << m_port;
}

void RemoteControl::connected() {
    qDebug() << "[RC] " << "Connected to " << m_host << " port " << m_port;
}

void RemoteControl::disconnected() {
    m_socket->deleteLater();
    qDebug() << "[RC] " << "Disconnected";
}

void RemoteControl::error(QAbstractSocket::SocketError err) {
    qDebug() << "[RC] " << "Error: " << err;
}

void RemoteControl::readyRead() {
    QByteArray buffer;
    if (m_socket->bytesAvailable()) {
        buffer = m_socket->readAll();
        qDebug() << "[RC] " << "Got Data: \n\t" << buffer;
        emit mavlinkMessage(buffer);
    }
}
