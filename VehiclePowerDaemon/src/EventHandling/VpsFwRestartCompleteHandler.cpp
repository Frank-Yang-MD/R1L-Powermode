/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsFwRestartCompleteHandler.h"
#include "IFwRestartCompleteNotification.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdEvent;

VpsFwRestartCompleteHandler::VpsFwRestartCompleteHandler(IFwRestartCompleteNotification& processor)
    : mIFwRestartCompleteNotification{processor}
{
}

void VpsFwRestartCompleteHandler::Process(int, EVpdEvent event)
{
    if (event == EVpdEvent::FwRestartCompleteVps) {
        mIFwRestartCompleteNotification.onFwRestartComplete();
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::VpsFwRestartCompleteHandler_Wrong_event_index, event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
