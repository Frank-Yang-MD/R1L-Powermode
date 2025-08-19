/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForAppResumeComplete.h"
#include "ITimer.h"
#include "IVcpuActions.h"
#include "Resume.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForVpsStart.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdNotifications;

std::unique_ptr<VpdStateBase> WaitForAppResumeComplete::sInstance = std::unique_ptr<VpdStateBase>();

void WaitForAppResumeComplete::initInstance(IVpdStateMachine* sm,
                                            INotifyHelper& notifyHelper,
                                            ITimer& appResumeEfwTimer,
                                            IVcpuActions& cpuComActions)
{
    sInstance.reset(
        new WaitForAppResumeComplete(sm, notifyHelper, appResumeEfwTimer, cpuComActions));
}

void WaitForAppResumeComplete::onEnter()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForAppResumeComplete_onEnter);

    mAppResumeEfwTimer.start();

    mNotifyHelper.notifyVps(EVpdNotifications::AppResume);
    mCpuActions.sendMainStartComplete();
}

void WaitForAppResumeComplete::onAppResumeComplete()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForAppResumeComplete_onAppResumeComplete);
    transitionTo(Resume::getInstance());
}

void WaitForAppResumeComplete::onTimeout()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForAppResumeComplete_onTimeout);

    if (mAppResumeEfwTimer.isExpired()) {
        MLOGW(LogFuncId::VPD, LogId::WaitForAppResumeComplete_timerIsExpired);
        mNotifyHelper.notifyVps(EVpdNotifications::TimeOutError);
        mCpuActions.sendSelfRebootTrue();
        transitionTo(Resume::getInstance());
    }
}

void WaitForAppResumeComplete::onExit()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForAppResumeComplete_onExit);
}

void WaitForAppResumeComplete::onDisconnectVps()
{
    VpdStateBase::onDisconnectVps();
    transitionTo(WaitForVpsStart::getInstance());
}

WaitForAppResumeComplete::WaitForAppResumeComplete(IVpdStateMachine* sm,
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
