#ifndef MAVLINKPARSER_H
#define MAVLINKPARSER_H

#include <QObject>

class MAVLinkParser : public QObject
{
    Q_OBJECT
public:
    explicit MAVLinkParser(QObject *parent = 0);

signals:

public slots:
    void communicationReceive(QByteArray ba);

private:
    quint16 m_droppedPackages;
};

#endif // MAVLINKPARSER_H
