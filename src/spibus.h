#ifndef SPIBUS_H
#define SPIBUS_H

#include <QObject>
#include <QFile>
#include <QTimer>

//#include <unistd.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <getopt.h>
//#include <fcntl.h>
#include <sys/ioctl.h>
//#include <linux/types.h>
#include <linux/spi/spidev.h>

class SpiBus : public QObject
{
    Q_OBJECT
public:
    explicit SpiBus(QObject *parent = 0);
    ~SpiBus();

signals:
    void newMessageReceived(QByteArray msg);

public slots:
    void pollBus(); /* internal slot for timer */
    void sendMessage(QByteArray payload);

private:
    void parseRxMessage();
    static const int spiMsgLength = 8;
    static const char PREAMBLE_BYTE1 = 0xAAu;
    static const char PREAMBLE_BYTE2 = 0x55u;
    static const int MAX_FRAME_LENGTH = 10 * 1024;
    static const int FRAME_OVERHEAD = 2 + 1 + 3 + 2 + 8; // preamble, type, reserved, length, crc
    static const int MESSAGE_TYPE_PROTOBUF = 1;

    uint8_t rx[spiMsgLength] = { 0x00, };
    int fileDescriptor;
    QByteArray rxArray;
    QByteArray txArray;
    QTimer m_readTimer;
    QFile busDeviceFile;
    struct spi_ioc_transfer transaction;
};

#endif // SPIBUS_H
