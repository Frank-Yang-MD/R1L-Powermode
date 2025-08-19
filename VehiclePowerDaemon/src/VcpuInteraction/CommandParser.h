/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_COMMANDPARSER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_COMMANDPARSER_H_

#include <memory>

#include "CpuCom.h"
#include "EVpdEvent.h"
#include "IEventReceiver.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using CpuCommand = ::com::mitsubishielectric::ahu::common::CpuCommand;

class CommandParser
    : public cpucom::ICpuCommandListener
    , public cpucom::ICpuCommandErrorListener {
public:
    static const CpuCommand SHUTDOWN_REQUEST;
    static const CpuCommand SHUTDOWN_CANCEL_REQUEST;

    CommandParser(IEventReceiver& eventReceiver);

    virtual void onReceiveCommand(const CpuCommand& command,
                                  const std::vector<uint8_t>& data) override;
    virtual void onError(const CpuCommand& command, int errorCode) override;

private:
    common::EVpdEvent parseCommand(const CpuCommand& command, const std::vector<uint8_t>& data);
    common::EVpdEvent parseShutdownRequest(const std::vector<uint8_t>& data);
    common::EVpdEvent parseShutdownCancelRequest(const std::vector<uint8_t>& data);

    IEventReceiver& mEventReceiver;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_COMMANDPARSER_H_
