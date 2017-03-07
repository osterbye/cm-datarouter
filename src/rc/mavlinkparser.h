#ifndef MAVLINKPARSER_H
#define MAVLINKPARSER_H

#include <QObject>

class MAVLinkParser : public QObject
{
    Q_OBJECT
public:
    explicit MAVLinkParser(QObject *parent = 0);

    quint16 droppedPackages();

signals:
    void heartbeat();
    void control(float throttle, float angle);
    void packageLoss(quint16 count);

public slots:
    void communicationReceive(QByteArray ba);

private:
    quint16 m_droppedPackages;
};

#endif // MAVLINKPARSER_H
