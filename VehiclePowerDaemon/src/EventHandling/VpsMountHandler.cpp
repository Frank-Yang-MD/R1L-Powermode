/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpsMountHandler.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

VpsMountHandler::VpsMountHandler(IVpsMount& inIVpsMount)
    : mIVpsMount(inIVpsMount)
{
}

void VpsMountHandler::Process(int, common::EVpdEvent event)
{
    switch (event) {
    case common::EVpdEvent::UnmountComplete:
        mIVpsMount.onVpsUnmountComplete();
        break;
    default:
        MLOGW(LogFuncId::VPD, LogId::VpsMountHandler_Process_Wrong_event_index, event);
        break;
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
