#include "mavlinkparser.h"
#include "common/mavlink.h"

#include <QDebug>


MAVLinkParser::MAVLinkParser(QObject *parent) : QObject(parent)
{
    m_droppedPackages = 0;
}

void MAVLinkParser::communicationReceive(QByteArray ba)
{
    mavlink_message_t msg;
    mavlink_status_t status;
    mavlink_heartbeat_t heartbeat;
    mavlink_command_long_t commandlong;

    for (int i = 0; i < ba.count(); ++i) {
        if (mavlink_parse_char(0, ba.at(i), &msg, &status)) {
            switch (msg.msgid) {
            case MAVLINK_MSG_ID_HEARTBEAT:
                mavlink_msg_heartbeat_decode(&msg, &heartbeat);
                qDebug() << "[mavlink] " << "Got a heartbeat";
                break;
            case MAVLINK_MSG_ID_COMMAND_LONG:
                mavlink_msg_command_long_decode(&msg, &commandlong);
                qDebug() << "[mavlink] " << "Got a command long message (arm message?)";
                break;
            default:
                qDebug() << "[mavlink] " << "Unknown message id: " << msg.msgid;
                break;
            }
        }
    }
    m_droppedPackages += status.packet_rx_drop_count;
}
