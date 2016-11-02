#include "spibus.h"
#include "logging.h"
#include <QIODevice>
#include <QCommandLineParser>
#include <QFile>

/* This whole class is done with C stdio interface because Qt can't do ioctl.
 * It should be rewritten to use custom kernel SPI protocol driver to jiggle GPIOs.
*/

SpiBus::SpiBus(QObject *parent) : QObject(parent)
{
    LOG_INFO("Starting spibus");

    QString deviceFilename = "/dev/spidev0.0";
    busDeviceFile.setFileName(deviceFilename);
    if (! busDeviceFile.open(QIODevice::ReadWrite)) {
        LOG_CRITICAL("Could not open specified input file: " << deviceFilename);
    }
    fileDescriptor = busDeviceFile.handle();

    const uint8_t mode = 0;
    const uint8_t bits = 8;
    const uint32_t speed = 1000;
    const uint16_t delay = 0;

    int ret;
    ret = ioctl(fileDescriptor, SPI_IOC_WR_MODE, &mode);
    if (ret == -1)
        LOG_CRITICAL("Can't set spi mode");
    ret = ioctl(fileDescriptor, SPI_IOC_RD_MODE, &mode);
    if (ret == -1)
        LOG_CRITICAL("Can't get spi mode");
    ret = ioctl(fileDescriptor, SPI_IOC_WR_BITS_PER_WORD, &bits);
    if (ret == -1)
        LOG_CRITICAL("Can't set bits per word");
    ret = ioctl(fileDescriptor, SPI_IOC_RD_BITS_PER_WORD, &bits);
    if (ret == -1)
        LOG_CRITICAL("Can't get bits per word");
    ret = ioctl(fileDescriptor, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        LOG_CRITICAL("Can't set max speed hz");
    ret = ioctl(fileDescriptor, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
    if (ret == -1)
        LOG_CRITICAL("Can't get max speed hz");

    transaction.rx_buf = (unsigned long)rx;
    transaction.len = spiMsgLength;
    transaction.delay_usecs = delay;
    transaction.speed_hz = 0;
    transaction.bits_per_word = 0;

    // Testing code for sending message from command line arguments:
    //  ./datarouter 01 02 03 04 0A 0B 0C 0D
    //QCommandLineParser parser;
    QByteArray payload;
    //const QStringList args = parser.positionalArguments();
    for (int i = 1; i < QCoreApplication::arguments().length(); i++) {
        QString s = QCoreApplication::arguments().at(i); //args.at(i);
        payload.append(s.toInt(NULL, 16));
    }
    if (payload.length()) {
        LOG_INFO("Sending data to SPI bus: " << payload);
        sendMessage(payload);
    }

    // Run task periodically
    QObject::connect(&m_readTimer, &QTimer::timeout, this, &SpiBus::pollBus);
    m_readTimer.setSingleShot(false);
    m_readTimer.start(20);
}

SpiBus::~SpiBus()
{
    busDeviceFile.close();
}

void SpiBus::sendMessage(QByteArray payload)
{
    txArray.reserve(txArray.length() + payload.length() + FRAME_OVERHEAD);
    txArray.append("\xAA\x55\x01"); // add 2 byte preamble, msg type
    txArray.append(3, '\x00');      // add 3 reserved bytes
    unsigned short int length = payload.length() + FRAME_OVERHEAD;
    txArray.append((char)(length >> 8));
    txArray.append((char)(length & 0xFF));
    txArray.append(payload);
    txArray.append(8, '\x00'); // TODO: CRC
}

void SpiBus::parseRxMessage() {
    int locPB1 = rxArray.indexOf(PREAMBLE_BYTE1);
    int locPB2 = rxArray.indexOf(PREAMBLE_BYTE2);

    // if preamble not in place, we need to discard some data
    if (locPB1 != 0 || locPB2 != 1) {

        // preamble found in middle of stream
        if ((locPB2 == locPB1 + 1) && (locPB1 >= 0)) {
            // TODO: remove printing of discarged data after com is tested
            if (locPB1 > 0 && (rxArray.mid(0, locPB1).count('\x00') != locPB1))
                LOG_DEBUG("discarding junk: " << rxArray.mid(0, locPB1));
            rxArray = rxArray.mid(locPB1); // TODO: unnecessary memory allocation? copy instead?
        }

        // no preamble found in stream
        if (locPB2 < 0) {
            if (locPB1 != rxArray.length() - 1) {
                LOG_DEBUG("discarding junk: " << rxArray);
                rxArray.clear();
            }
            return;
        }
    }

    if (rxArray.length() < FRAME_OVERHEAD)
        return; // we don't have enough of stream to decode frame

    if (rxArray.at(2) != MESSAGE_TYPE_PROTOBUF) {
        QString out;
        LOG_WARN(out.sprintf("Discarding message with unsupported type 0x%02X", rxArray.at(2)));
        rxArray = rxArray.mid(2); // remove preamble of unparsable frame
        return;
    }

    int frameLength =  (rxArray.at(6) << 8) + rxArray.at(7);
    int messageLength = frameLength - FRAME_OVERHEAD; // removing preamble, type, reserved, length, crc

    if ((frameLength < FRAME_OVERHEAD) || frameLength > MAX_FRAME_LENGTH) {
        LOG_WARN("Discarding frame of impossible length " << frameLength);
        rxArray = rxArray.mid(2); // remove preamble of unparsable message
        return;
    }

    if (rxArray.length() < frameLength)
        return; // not enough data in stream
        // TODO: instead of returning we should check for more preambles within buffer
        // so we catch next frames without delays if our current frame is invalid

    // TODO: do CRC check on frame

    emit newMessageReceived(rxArray.mid(2 + 1 + 3 + 2, messageLength));
    rxArray = rxArray.mid(frameLength); // remove frame from buffer
}

void SpiBus::pollBus()
{
    int ret;

    // poll SPI bus by sending dummy data when there's no useful outbound data
    if (txArray.length() < spiMsgLength)
        txArray = txArray.append(spiMsgLength - txArray.length(), '\x00');

    transaction.tx_buf = (unsigned long) txArray.constData();

    ret = ioctl(fileDescriptor, SPI_IOC_MESSAGE(1), &transaction);

    if (ret == 1)
        LOG_CRITICAL("Can't send spi message");
    else // remove transmited bytes from txArray
        txArray = txArray.mid(spiMsgLength);

    // incomming stream is just concatinated
    rxArray.append((char *) rx, spiMsgLength);

    // try to make sense of stream and remove parsed sections from rxArray
    parseRxMessage();
}
