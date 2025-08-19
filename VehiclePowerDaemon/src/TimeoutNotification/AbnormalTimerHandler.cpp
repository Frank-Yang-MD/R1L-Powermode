/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "AbnormalTimerHandler.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

AbnormalTimerHandler::AbnormalTimerHandler(std::unique_ptr<ITimer> timer, IVcpuActions& vcpuActions)
    : AbstractResetTimerHandler{std::move(timer)}
    , mVcpuActions{vcpuActions}
{
}

void AbnormalTimerHandler::performResetAction()
{
    mVcpuActions.sendAdtrRequest();
    MLOGI(LogFuncId::VPD, LogId::AbnormalTimerHandler_resetRequestSent);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
