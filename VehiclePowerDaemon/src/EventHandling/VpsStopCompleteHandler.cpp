/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsStopCompleteHandler.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

VpsStopCompleteHandler::VpsStopCompleteHandler(IStopCompleteVps& inIStopCompleteVps)
    : mIStopCompleteVps(inIStopCompleteVps)
{
}

void VpsStopCompleteHandler::Process(int, common::EVpdEvent event)
{
    switch (event) {
    case common::EVpdEvent::AppStopCompleteVps:
        mIStopCompleteVps.onAppStopCompleteVps();
        break;
    case common::EVpdEvent::FwStopCompleteVps:
        mIStopCompleteVps.onFwStopCompleteVps();
        break;
    case common::EVpdEvent::StopFailedVps:
        mIStopCompleteVps.onStopFailedVps();
        break;
    default:
        MLOGW(LogFuncId::VPD, LogId::VpsStopCompleteHandler_Process_Wrong_event_index, event);
        break;
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
