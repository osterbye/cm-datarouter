#include "mockbus.h"
#include "logging.h"
#include <QIODevice>
#include <QFile>

MockBus::MockBus(QObject *parent) : QObject(parent)
{
    QString filename = "../intercomm/tools/sample_pb.bin";
    m_busDevice.setFileName(filename);
    if (! m_busDevice.open(QIODevice::ReadOnly)) {
        LOG_CRITICAL("Could not open specified input file: " << filename);
    }
    // Run task periodically
    QObject::connect(&m_readTimer, &QTimer::timeout, this, &MockBus::readNextMessage);
    m_readTimer.setSingleShot(false);
    m_readTimer.start(500);
}

MockBus::~MockBus()
{
    m_busDevice.close();
}

void MockBus::readNextMessage()
{
    m_busArray = m_busDevice.read(2 + 1 + 3 + 2); // includes preamble, frame type and frame length
    if (m_busArray.length() != 2 + 1 + 3 + 2) {
        LOG_INFO("End of file, no more messages");
        return;
    }

    // check preamble
    Q_ASSERT(m_busArray.at(0) == '\xAA');
    Q_ASSERT(m_busArray.at(1) == '\x55');
    // message type
    Q_ASSERT_X(m_busArray.at(2) == '\x01', __FUNCTION__, "Unsupported message type");

    int frameLength =  (m_busArray[6] << 8) + m_busArray[7];
    int messageLength = frameLength - 2 - 1 - 3 - 2 - 8; // removing preamble, type, reserved, length, crc
    m_busArray = m_busDevice.read(messageLength);
    if (m_busArray.length() != messageLength)
        return; // no more messages in file

    m_busDevice.read(8); // flush CRC
    emit newMessageReceived(m_busArray);
    return;
}
