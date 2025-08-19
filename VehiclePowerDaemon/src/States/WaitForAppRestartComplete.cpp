/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForAppRestartComplete.h"
#include "INotifyHelper.h"
#include "IPowerStateHolder.h"
#include "ITimer.h"
#include "IVcpuActions.h"
#include "NormalOperation.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForAppStopVps.h"
#include "WaitForVpsStart.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdNotifications;

std::unique_ptr<VpdStateBase> WaitForAppRestartComplete::sInstance =
    std::unique_ptr<VpdStateBase>();

void WaitForAppRestartComplete::initInstance(IVpdStateMachine* sm,
                                             INotifyHelper& nh,
                                             ITimer& rt,
                                             IVcpuActions& va,
                                             IPowerStateHolder& psh)
{
    sInstance.reset(new (std::nothrow) WaitForAppRestartComplete(sm, nh, rt, va, psh));
}

void WaitForAppRestartComplete::onEnter()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForAppRestartComplete_onEnter);
    mAppRestartAppTimer.start();
    mNotifyHelper.notifyVps(EVpdNotifications::AppRestart);
}

void WaitForAppRestartComplete::toNextState()
{
    if (mPowerStateHolder.isShutdownState()) {
        transitionTo(WaitForAppStopVps::getInstance());
    }
    else {
        transitionTo(NormalOperation::getInstance());
    }
}

void WaitForAppRestartComplete::onAppRestartComplete()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForAppRestartComplete_onAppRestartComplete);
    toNextState();
}

void WaitForAppRestartComplete::onTimeout()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForAppRestartComplete_onTimeout);

    if (mAppRestartAppTimer.isExpired()) {
        MLOGW(LogFuncId::VPD, LogId::WaitForAppRestartComplete_timerIsExpired);
        mNotifyHelper.notifyVps(EVpdNotifications::TimeOutError);
        mCpuActions.sendSelfRebootTrue();
        toNextState();
    }
}

void WaitForAppRestartComplete::onExit()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForAppRestartComplete_onExit);
    mCpuActions.sendShutdownCancelComplete();
}

void WaitForAppRestartComplete::onDisconnectVps()
{
    VpdStateBase::onDisconnectVps();
    transitionTo(WaitForVpsStart::getInstance());
}

WaitForAppRestartComplete::WaitForAppRestartComplete(IVpdStateMachine* sm,
                                                     INotifyHelper& nh,
                                                     ITimer& rt,
                                                     IVcpuActions& va,
                                                     IPowerStateHolder& psh)
    : VpdStateBase{sm}
    , mNotifyHelper{nh}
    , mAppRestartAppTimer{rt}
    , mCpuActions{va}
    , mPowerStateHolder{psh}
{
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
