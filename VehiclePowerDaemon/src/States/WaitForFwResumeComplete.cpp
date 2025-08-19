/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForFwResumeComplete.h"
#include "INotifyHelper.h"
#include "ITimer.h"
#include "IVcpuActions.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForAppResumeComplete.h"
#include "WaitForVpsStart.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdNotifications;

std::unique_ptr<VpdStateBase> WaitForFwResumeComplete::sInstance = std::unique_ptr<VpdStateBase>();

void WaitForFwResumeComplete::initInstance(IVpdStateMachine* sm,
                                           INotifyHelper& notifyHelper,
                                           ITimer& appResumeEfwTimer,
                                           IVcpuActions& cpuComActions)
{
    sInstance.reset(
        new WaitForFwResumeComplete(sm, notifyHelper, appResumeEfwTimer, cpuComActions));
}

void WaitForFwResumeComplete::onEnter()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForFwResumeComplete_onEnter);

    mAppResumeEfwTimer.start();

    mNotifyHelper.notifyVps(EVpdNotifications::FwResume);
}

void WaitForFwResumeComplete::onFwResumeComplete()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForFwResumeComplete_onFwResumeComplete);
    transitionTo(WaitForAppResumeComplete::getInstance());
}

void WaitForFwResumeComplete::onTimeout()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForFwResumeComplete_onTimeout);

    if (mAppResumeEfwTimer.isExpired()) {
        MLOGW(LogFuncId::VPD, LogId::WaitForFwResumeComplete_timerIsExpired);
        mNotifyHelper.notifyVps(EVpdNotifications::TimeOutError);
        mCpuActions.sendSelfRebootTrue();
        transitionTo(WaitForAppResumeComplete::getInstance());
    }
}

void WaitForFwResumeComplete::onDisconnectVps()
{
    VpdStateBase::onDisconnectVps();
    transitionTo(WaitForVpsStart::getInstance());
}

WaitForFwResumeComplete::WaitForFwResumeComplete(IVpdStateMachine* sm,
                                                 INotifyHelper& notifyHelper,
                                                 ITimer& appResumeEfwTimer,
                                                 IVcpuActions& cpuComActions)
    : VpdStateBase{sm}
    , mNotifyHelper{notifyHelper}
    , mAppResumeEfwTimer{appResumeEfwTimer}
    , mCpuActions{cpuComActions}
{
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
