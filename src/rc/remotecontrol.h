#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include <QObject>
#include <QTcpSocket>

class MAVLinkParser;

class RemoteControl : public QObject
{
    Q_OBJECT
public:
    explicit RemoteControl(QString host, quint16 port, QObject *parent = 0);

signals:
    void mavlinkMessage(QByteArray ba);

private slots:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError err);
    void readyRead();

private:
    MAVLinkParser *m_parser;
    QTcpSocket *m_socket;
    QString m_host;
    quint16 m_port;
};

#endif // REMOTECONTROL_H
