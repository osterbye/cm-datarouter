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
    busArray = busDevice.read(2 + 1 + 3 + 2); // includes preamble, frame type and frame length
    if (busArray.length() != 2 + 1 + 3 + 2)
        return false; // no more messages in file

    // check preamble
    Q_ASSERT(busArray.at(0) == '\xAA');
    Q_ASSERT(busArray.at(1) == '\x55');
    // message type
    Q_ASSERT_X(busArray.at(2) == '\x01', __FUNCTION__, "Unsupported message type");

    int frameLength =  (busArray[6] << 8) + busArray[7];
    int messageLength = frameLength - 2 - 1 - 3 - 2 - 8; // removing preamble, type, reserved, length, crc
    qDebug() << frameLength << messageLength;
    busArray = busDevice.read(messageLength);
    if (busArray.length() != messageLength)
        return false; // no more messages in file

    busDevice.read(8); // flush CRC
    emit newMessageReceived(busArray);
    return true;
}
