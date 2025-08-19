/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForLogDaemonStop.h"
#include "RestartNative.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForUnmount.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> WaitForLogDaemonStop::sInstance = std::unique_ptr<VpdStateBase>();

void WaitForLogDaemonStop::initInstance(IVpdStateMachine* sm,
                                        INotifyHelper& notifyHelper,
                                        ITimer& logdogStopTimer,
                                        IPowerStateHolder& powerStateHolder,
                                        IVcpuActions& vcpuAction)
{
    sInstance.reset(
        new WaitForLogDaemonStop(sm, notifyHelper, logdogStopTimer, powerStateHolder, vcpuAction));
}

WaitForLogDaemonStop::WaitForLogDaemonStop(IVpdStateMachine* sm,
                                           INotifyHelper& notifyHelper,
                                           ITimer& logdogStopTimer,
                                           IPowerStateHolder& powerStateHolder,
                                           IVcpuActions& vcpuAction)
    : VpdStateBase{sm}
    , mNotifyHelper{notifyHelper}
    , mLogdogStopTimer{logdogStopTimer}
    , mPowerStateHolder{powerStateHolder}
    , mVcpuAction{vcpuAction}
{
}

void WaitForLogDaemonStop::onEnter()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForLogDaemonStop_onEnter);
    mNotifyHelper.notifyLog(common::EVpdState::APP_STOP);
    mLogdogStopTimer.start();
}

void WaitForLogDaemonStop::onTimeout()
{
    MLOGI(LogFuncId::VPD, LogId::WaitForLogDaemonStop_onTimeout);
    if (mLogdogStopTimer.isExpired()) {
        mVcpuAction.sendSelfRebootTrue();
        transitionToNextState();
    }
}

void WaitForLogDaemonStop::onStopCompleteLog()
{
    MLOGD(LogFuncId::VPD, LogId::WaitForLogDaemonStop_onStopCompleteLog);
    transitionToNextState();
}

void WaitForLogDaemonStop::transitionToNextState()
{
    if (mPowerStateHolder.isShutdownState()) {
        transitionTo(WaitForUnmount::getInstance());
    }
    else {
        transitionTo(RestartNative::getInstance());
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
