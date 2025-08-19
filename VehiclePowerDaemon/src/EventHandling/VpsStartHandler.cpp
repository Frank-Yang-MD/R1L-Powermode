/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsStartHandler.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

VpsStartHandler::VpsStartHandler(IStartCompleteVps& inIStartCompleteVps)
    : mIStartCompleteVps(inIStartCompleteVps)
{
}

void VpsStartHandler::Process(int, common::EVpdEvent event)
{
    if (event == common::EVpdEvent::VpsStarted) {
        mIStartCompleteVps.onStartCompleteVps();
    }
    // This is Temporary Code.
    else if (event == common::EVpdEvent::StartFailedVps) {
        mIStartCompleteVps.onStartFailedVps();
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::VpsStartHandler_Process_Wrong_event_index, event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
