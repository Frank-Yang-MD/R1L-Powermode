/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */
#include "WaitForLogSaving.h"
#include "LogDaemonBusy.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> WaitForLogSaving::sInstance = std::unique_ptr<VpdStateBase>();

void WaitForLogSaving::initInstance(IVpdStateMachine* sm, ITimer& timer)
{
    sInstance.reset(new WaitForLogSaving(sm, timer));
}

WaitForLogSaving::WaitForLogSaving(IVpdStateMachine* sm, ITimer& timer)
    : VpdStateBase(sm)
    , mTimer{timer}
{
}

void WaitForLogSaving::onEnter() { mTimer.start(); }

void WaitForLogSaving::onTimeout()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForLogSaving_OnTimeout);
    if (mTimer.isExpired()) {
        MLOGI(LogFuncId::VPD, LogId::WaitForLogSaving_Timer_Is_Expired);
        transitionTo(LogDaemonBusy::getInstance());
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
