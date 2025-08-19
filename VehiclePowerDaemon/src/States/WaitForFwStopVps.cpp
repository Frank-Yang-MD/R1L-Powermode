/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForFwStopVps.h"
#include "IVcpuActions.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitBeforeSendingAppStopNativeServices.h"
#include "WaitNativeAppStopProcessingComplete.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> WaitForFwStopVps::sInstance = std::unique_ptr<VpdStateBase>();

void WaitForFwStopVps::initInstance(IVpdStateMachine* sm,
                                    INotifyHelper& notifyHelper,
                                    ITimer& fwStopVpsTimer,
                                    IVcpuActions& vcpuActions)
{
    sInstance.reset(new WaitForFwStopVps(sm, notifyHelper, fwStopVpsTimer, vcpuActions));
}

WaitForFwStopVps::WaitForFwStopVps(IVpdStateMachine* sm,
                                   INotifyHelper& notifyHelper,
                                   ITimer& fwStopVpsTimer,
                                   IVcpuActions& vcpuActions)
    : VpdStateBase{sm}
    , mNotifyHelper{notifyHelper}
    , mFwStopVpsTimer{fwStopVpsTimer}
    , mVcpuActions{vcpuActions}
{
}

void WaitForFwStopVps::onEnter()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForFwStopVps_onEnter);
    mNotifyHelper.notifyVps(common::EVpdNotifications::FwStop);
}

void WaitForFwStopVps::onTimeout()
{
    MLOGI(LogFuncId::VPD, WaitForFwStopVps_onTimeout);

    if (mFwStopVpsTimer.isExpired()) {
        MLOGW(LogFuncId::VPD, LogId::WaitForFwStopVps_Timer_Is_Expired);
        mNotifyHelper.notifyVps(common::EVpdNotifications::TimeOutError);
        mVcpuActions.sendSelfRebootTrue();
        transitionTo(WaitBeforeSendingAppStopNativeServices::getInstance());
    }
}

void WaitForFwStopVps::onFwStopCompleteVps()
{
    MLOGI(LogFuncId::VPD, WaitForFwStopVps_onFwStopCompleteVps);

    transitionTo(WaitNativeAppStopProcessingComplete::getInstance());
}

void WaitForFwStopVps::onStopFailedVps()
{
    MLOGW(LogFuncId::VPD, WaitForFwStopVps_onStopFailedVps);
    mVcpuActions.sendSelfRebootTrue();
    transitionTo(WaitNativeAppStopProcessingComplete::getInstance());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
