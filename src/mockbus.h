#ifndef MOCKBUS_H
#define MOCKBUS_H

#include <QObject>
#include <QFile>

class MockBus : public QObject
{
    Q_OBJECT
    bool readNextMessage();
public:
    explicit MockBus(QObject *parent = 0);
    ~MockBus();
    void readBus();

signals:
    void newMessageReceived(QByteArray msg);
public slots:

private:
    QByteArray busArray;
    QFile busDevice;
};

#endif // MOCKBUS_H
