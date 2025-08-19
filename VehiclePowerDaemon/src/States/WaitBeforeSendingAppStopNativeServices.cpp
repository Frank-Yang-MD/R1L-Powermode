/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitBeforeSendingAppStopNativeServices.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitNativeAppStopProcessingComplete.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> WaitBeforeSendingAppStopNativeServices::sInstance =
    std::unique_ptr<VpdStateBase>();

void WaitBeforeSendingAppStopNativeServices::initInstance(IVpdStateMachine* sm, ITimer& timer)
{
    sInstance.reset(new WaitBeforeSendingAppStopNativeServices{sm, timer});
}

WaitBeforeSendingAppStopNativeServices::WaitBeforeSendingAppStopNativeServices(IVpdStateMachine* sm,
                                                                               ITimer& timer)
    : VpdStateBase(sm)
    , mTimer{timer}
{
}

void WaitBeforeSendingAppStopNativeServices::onEnter() { mTimer.start(); }

void WaitBeforeSendingAppStopNativeServices::onTimeout()
{
    MLOGI(LogFuncId::VPD, LogId::WaitBeforeSendingAppStopNativeServices_onTimeout);
    if (mTimer.isExpired()) {
        MLOGI(LogFuncId::VPD, LogId::WaitBeforeSendingAppStopNativeServices_Timer_Is_Expired);
        transitionTo(WaitNativeAppStopProcessingComplete::getInstance());
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
