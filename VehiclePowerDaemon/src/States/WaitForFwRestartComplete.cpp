/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForFwRestartComplete.h"
#include "INotifyHelper.h"
#include "ITimer.h"
#include "IVcpuActions.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForAppRestartComplete.h"
#include "WaitForVpsStart.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdNotifications;

std::unique_ptr<VpdStateBase> WaitForFwRestartComplete::sInstance = std::unique_ptr<VpdStateBase>();

void WaitForFwRestartComplete::initInstance(IVpdStateMachine* sm,
                                            INotifyHelper& nh,
                                            ITimer& rt,
                                            IVcpuActions& va)
{
    sInstance.reset(new WaitForFwRestartComplete(sm, nh, rt, va));
}

void WaitForFwRestartComplete::onEnter()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForFwRestartComplete_onEnter);
    mAppRestartEfwTimer.start();
    mNotifyHelper.notifyVps(EVpdNotifications::FwRestart);
}

void WaitForFwRestartComplete::onFwRestartComplete()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForFwRestartComplete_onFwRestartComplete);
    transitionTo(WaitForAppRestartComplete::getInstance());
}

void WaitForFwRestartComplete::onTimeout()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForFwRestartComplete_onTimeout);

    if (mAppRestartEfwTimer.isExpired()) {
        MLOGW(LogFuncId::VPD, LogId::WaitForFwRestartComplete_timerIsExpired);
        mNotifyHelper.notifyVps(EVpdNotifications::TimeOutError);
        mCpuActions.sendSelfRebootTrue();
        transitionTo(WaitForAppRestartComplete::getInstance());
    }
}

void WaitForFwRestartComplete::onDisconnectVps()
{
    VpdStateBase::onDisconnectVps();
    transitionTo(WaitForVpsStart::getInstance());
}

WaitForFwRestartComplete::WaitForFwRestartComplete(IVpdStateMachine* sm,
                                                   INotifyHelper& nh,
                                                   ITimer& rt,
                                                   IVcpuActions& va)
    : VpdStateBase{sm}
    , mNotifyHelper{nh}
    , mAppRestartEfwTimer{rt}
    , mCpuActions{va}
{
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
