#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include <QObject>
#include <QTcpSocket>
#include <QTime>

class MAVLinkParser;

class RemoteControl : public QObject
{
    Q_OBJECT
public:
    explicit RemoteControl(QString host, quint16 port, QObject *parent = 0);

signals:
    void control(float throttle, float angle);
    void packageLoss(quint16 count);
    void mavlinkMessage(QByteArray ba);

private:
    void initConnection();

private slots:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError err);
    void readyRead();
    void heartbeat();
    void controlCmd(float throttle, float angle);

private:
    MAVLinkParser *m_parser;
    QTcpSocket *m_socket;
    QTime m_heartbeatTimer;
    QString m_host;
    quint16 m_port;
    float m_lastThrottle;
    float m_lastAngle;
};

#endif // REMOTECONTROL_H
