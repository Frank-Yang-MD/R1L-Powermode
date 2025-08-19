/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WatchDogTimerHandler.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

WatchDogTimerHandler::WatchDogTimerHandler(std::unique_ptr<ITimer> timer, IVcpuActions& vcpuActions)
    : AbstractResetTimerHandler{std::move(timer)}
    , mVcpuActions{vcpuActions}
{
}

void WatchDogTimerHandler::performResetAction()
{
    mVcpuActions.sendWdtrRequest();
    MLOGI(LogFuncId::VPD, LogId::WatchDogTimerHandler_resetRequestSent);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
