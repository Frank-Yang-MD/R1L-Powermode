/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsFwResumeCompleteHandler.h"
#include "IFwResumeCompleteNotification.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdEvent;

VpsFwResumeCompleteHandler::VpsFwResumeCompleteHandler(IFwResumeCompleteNotification& processor)
    : mIFwResumeCompleteNotification{processor}
{
}

void VpsFwResumeCompleteHandler::Process(int, EVpdEvent event)
{
    if (event == EVpdEvent::FwResumeCompleteVps) {
        mIFwResumeCompleteNotification.onFwResumeComplete();
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::VpsFwResumeCompleteHandler_Wrong_event_index, event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
