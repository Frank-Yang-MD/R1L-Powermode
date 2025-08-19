/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsAppResumeCompleteHandler.h"
#include "IAppResumeCompleteNotification.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdEvent;

VpsAppResumeCompleteHandler::VpsAppResumeCompleteHandler(IAppResumeCompleteNotification& processor)
    : mIAppResumeCompleteNotification{processor}
{
}

void VpsAppResumeCompleteHandler::Process(int, EVpdEvent event)
{
    if (event == EVpdEvent::AppResumeCompleteVps) {
        mIAppResumeCompleteNotification.onAppResumeComplete();
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::VpsAppResumeCompleteHandler_Wrong_event_index, event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
