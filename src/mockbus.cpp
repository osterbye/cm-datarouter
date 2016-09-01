#include "mockbus.h"
#include <QIODevice>
#include <QFile>
#include <QDebug>

MockBus::MockBus(QObject *parent) : QObject(parent)
{
    QString filename = "../intercomm/tools/sample_pb.bin";
    busDevice.setFileName(filename);
    if (! busDevice.open(QIODevice::ReadOnly))
    {
        qDebug() << "Could not open specified input file: " << filename;
    }
}

MockBus::~MockBus()
{
    busDevice.close();
}

void MockBus::readBus()
{
    if (!readNextMessage())
        qDebug() << "End of file, no more messages";
}

bool MockBus::readNextMessage()
{
    busArray = busDevice.read(5); // includes preamble, frame type and frame length
    if (busArray.length() != 5)
        return false; // no more messages in file

    // check preamble
    Q_ASSERT(busArray.at(0) == '\x50');
    Q_ASSERT(busArray.at(1) == '\x41');
    Q_ASSERT(busArray.at(2) == '\x01');

    int frameLength = busArray[3] + (busArray[4] << 8);
    int messageLength = frameLength - 2 - 1 - 2 - 2; // removing preamble, type, length, crc
    busArray = busDevice.read(messageLength);
    if (busArray.length() != messageLength)
        return false; // no more messages in file

    busDevice.read(2); // flush CRC
    emit newMessageReceived(busArray);
    return true;
}
