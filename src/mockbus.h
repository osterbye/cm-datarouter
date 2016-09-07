#ifndef MOCKBUS_H
#define MOCKBUS_H

#include <QObject>
#include <QFile>
#include <QTimer>

class MockBus : public QObject
{
    Q_OBJECT
public:
    explicit MockBus(QObject *parent = 0);
    ~MockBus();

signals:
    void newMessageReceived(QByteArray msg);

public slots:
    void readNextMessage();

private:
    QByteArray m_busArray;
    QFile m_busDevice;
    QTimer m_readTimer;
};

#endif // MOCKBUS_H
