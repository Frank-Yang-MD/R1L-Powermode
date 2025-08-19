/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitBeforeSendingFwStopVps.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForFwStopVps.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> WaitBeforeSendingFwStopVps::sInstance =
    std::unique_ptr<VpdStateBase>();

void WaitBeforeSendingFwStopVps::initInstance(IVpdStateMachine* sm, ITimer& timer)
{
    sInstance.reset(new WaitBeforeSendingFwStopVps{sm, timer});
}

WaitBeforeSendingFwStopVps::WaitBeforeSendingFwStopVps(IVpdStateMachine* sm, ITimer& timer)
    : VpdStateBase(sm)
    , mTimer{timer}
{
}

void WaitBeforeSendingFwStopVps::onEnter() { mTimer.start(); }

void WaitBeforeSendingFwStopVps::onTimeout()
{
    MLOGI(LogFuncId::VPD, LogId::WaitBeforeSendingFwStopVps_onTimeout);
    if (mTimer.isExpired()) {
        MLOGI(LogFuncId::VPD, LogId::WaitBeforeSendingFwStopVps_Timer_Is_Expired);
        transitionTo(WaitForFwStopVps::getInstance());
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
