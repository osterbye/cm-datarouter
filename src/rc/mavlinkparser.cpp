#include "mavlinkparser.h"
#include "rclogging.h"
#include "common/mavlink.h"

MAVLinkParser::MAVLinkParser(QObject *parent) : QObject(parent)
{
    m_droppedPackages = 0;
}

quint16 MAVLinkParser::droppedPackages()
{
    return m_droppedPackages;
}

void MAVLinkParser::communicationReceive(QByteArray ba)
{
    mavlink_message_t msg;
    mavlink_status_t status;

    for (int i = 0; i < ba.count(); ++i) {
        if (mavlink_parse_char(0, ba.at(i), &msg, &status)) {
            switch (msg.msgid) {
            case MAVLINK_MSG_ID_HEARTBEAT:
                mavlink_heartbeat_t heartbeatMsg;
                mavlink_msg_heartbeat_decode(&msg, &heartbeatMsg);
                emit heartbeat();
                LOG_DEBUG("Got a heartbeat. MAVLink version: " << heartbeatMsg.mavlink_version << ". Vehicle type: " << heartbeatMsg.type);
                break;
            case MAVLINK_MSG_ID_RC_CHANNELS_OVERRIDE:
                mavlink_rc_channels_override_t channelsOverride;
                float turnAngle;
                float throttle;
                mavlink_msg_rc_channels_override_decode(&msg, &channelsOverride);
                turnAngle = (channelsOverride.chan1_raw >= 1100 && channelsOverride.chan1_raw <= 1900) ? float(1500-channelsOverride.chan1_raw)/400.0 : 0;
                throttle = (channelsOverride.chan3_raw >= 1100 && channelsOverride.chan3_raw <= 1900) ? float(1500-channelsOverride.chan3_raw)/400.0 : 0;
                LOG_DEBUG("Got an RC control command. Turn value: " << channelsOverride.chan1_raw << " (" << turnAngle << ")" << ". Throttle value: " << channelsOverride.chan3_raw << " (" << throttle << ")");
                break;
            case MAVLINK_MSG_ID_COMMAND_LONG:
                mavlink_command_long_t commandlong;
                mavlink_msg_command_long_decode(&msg, &commandlong);
                LOG_DEBUG("Got a command long message");
                break;
            default:
                LOG_WARN("Unknown message id: " << msg.msgid);
                break;
            }
        }
    }
    if (status.packet_rx_drop_count > 0) {
        m_droppedPackages += status.packet_rx_drop_count;
        emit packageLoss(m_droppedPackages);
    }
}
