/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "CommandParser.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

const CpuCommand CommandParser::SHUTDOWN_REQUEST = {0x71, 0x02};
const CpuCommand CommandParser::SHUTDOWN_CANCEL_REQUEST = {0x71, 0x05};

CommandParser::CommandParser(IEventReceiver& eventReceiver)
    : mEventReceiver(eventReceiver)
{
}

void CommandParser::onReceiveCommand(const CpuCommand& command, const std::vector<uint8_t>& data)
{
    MLOGI(LogFuncId::VPD, LogId::CommandParser_onReceiveCommand_cmd_subcmd, command.first,
          command.second);

    common::EVpdEvent mt = parseCommand(command, data);
    if (common::EVpdEvent::Unsupported != mt) {
        mEventReceiver.onEventReceive(0, mt);
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::CommandParser_onReceiveCommand_unsupported_command_received);
    }
}

void CommandParser::onError(const CpuCommand& command, int errorCode)
{
    MLOGW(LogFuncId::VPD, LogId::CommandParser_onError_command_received_with_error_code,
          command.first, command.second, errorCode);
}

common::EVpdEvent CommandParser::parseCommand(const CpuCommand& command,
                                              const std::vector<uint8_t>& data)
{
    if (command == SHUTDOWN_REQUEST) {
        return parseShutdownRequest(data);
    }
    else if (command == SHUTDOWN_CANCEL_REQUEST) {
        return parseShutdownCancelRequest(data);
    }

    return common::EVpdEvent::Unsupported;
}

common::EVpdEvent CommandParser::parseShutdownRequest(const std::vector<uint8_t>& data)
{
    if (data.size() > 0) {
        switch (data[0]) {
        case 0x00:  // shutdown
            return common::EVpdEvent::Shutdown;
        case 0x01:  // suspend
            return common::EVpdEvent::Suspend;
        case 0x02:  // reboot
            return common::EVpdEvent::Reboot;
        case 0x03:  // reboot into recovery
            return common::EVpdEvent::RebootRecovery;
        default:
            break;
        }
    }
    return common::EVpdEvent::Unsupported;
}

common::EVpdEvent CommandParser::parseShutdownCancelRequest(const std::vector<uint8_t>& data)
{
    if (0 == data.size()) {
        return common::EVpdEvent::ShutdownCancel;
    }

    return common::EVpdEvent::Unsupported;
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
