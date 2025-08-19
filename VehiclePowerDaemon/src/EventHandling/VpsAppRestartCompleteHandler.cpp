/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsAppRestartCompleteHandler.h"
#include "IAppRestartCompleteNotification.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdEvent;

VpsAppRestartCompleteHandler::VpsAppRestartCompleteHandler(
    IAppRestartCompleteNotification& processor)
    : mIAppRestartCompleteNotification{processor}
{
}

void VpsAppRestartCompleteHandler::Process(int, EVpdEvent event)
{
    if (event == EVpdEvent::AppRestartCompleteVps) {
        mIAppRestartCompleteNotification.onAppRestartComplete();
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::VpsAppRestartCompleteHandler_Wrong_event_index, event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
