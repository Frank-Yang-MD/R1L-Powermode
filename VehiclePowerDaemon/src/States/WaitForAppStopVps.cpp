/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForAppStopVps.h"
#include "IVcpuActions.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitBeforeSendingFwStopVps.h"
#include "WaitForFwStopVps.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> WaitForAppStopVps::sInstance = std::unique_ptr<VpdStateBase>();

static const std::string EMERGENCY_COLDBOOT_PROP = "vendor.emerg.coldboot";

void WaitForAppStopVps::initInstance(IVpdStateMachine* sm,
                                     INotifyHelper& notifyHelper,
                                     ITimer& appStopVpsTimer,
                                     ITimer& fwStopVpsTimer,
                                     IVcpuActions& va,
                                     IAndroidInit& androidInit)
{
    sInstance.reset(new (std::nothrow) WaitForAppStopVps(sm, notifyHelper, appStopVpsTimer,
                                                         fwStopVpsTimer, va, androidInit));
}

WaitForAppStopVps::WaitForAppStopVps(IVpdStateMachine* sm,
                                     INotifyHelper& notifyHelper,
                                     ITimer& appStopVpsTimer,
                                     ITimer& fwStopVpsTimer,
                                     IVcpuActions& va,
                                     IAndroidInit& androidInit)
    : VpdStateBase{sm}
    , mNotifyHelper{notifyHelper}
    , mAppStopVpsTimer{appStopVpsTimer}
    , mFwStopVpsTimer{fwStopVpsTimer}
    , mVcpuActions{va}
    , mAndroidInit{androidInit}
{
}

void WaitForAppStopVps::onEnter()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForAppStopVps_onEnter);
    mNotifyHelper.notifyVps(common::EVpdState::APP_STOP);

    checkForColdboot();

    // Setup timeout
    mAppStopVpsTimer.start();
    mFwStopVpsTimer.start();
}

void WaitForAppStopVps::onTimeout()
{
    MLOGI(LogFuncId::VPD, WaitForAppStopVps_onTimeout);

    if (mAppStopVpsTimer.isExpired()) {
        MLOGW(LogFuncId::VPD, LogId::WaitForAppStopVps_Timer_Is_Expired);
        mNotifyHelper.notifyVps(common::EVpdNotifications::TimeOutError);
        mVcpuActions.sendSelfRebootTrue();
        transitionTo(WaitBeforeSendingFwStopVps::getInstance());
    }
}

void WaitForAppStopVps::onAppStopCompleteVps()
{
    MLOGI(LogFuncId::VPD, WaitForAppStopVps_onAppStopCompleteVps);

    transitionTo(WaitForFwStopVps::getInstance());
}

void WaitForAppStopVps::checkForColdboot() const
{
    std::string coldBootRequested;
    mAndroidInit.propertyGet(EMERGENCY_COLDBOOT_PROP, coldBootRequested);
    if (!coldBootRequested.empty()) {
        MLOGW(LogFuncId::VPD, LogId::WaitForAppStopVps_coldboot_requested,
              coldBootRequested.c_str());
        mVcpuActions.sendSelfRebootTrue();
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
