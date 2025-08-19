/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "LogDaemonBusy.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForLogDaemonStop.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> LogDaemonBusy::sInstance = std::unique_ptr<VpdStateBase>();

void LogDaemonBusy::initInstance(IVpdStateMachine* sm,
                                 ILogDaemonActions& logDogSavingState,
                                 ITimer& logdogCheckSavingStateTimer,
                                 ITimer& logdogSaveStateTimer,
                                 IVcpuActions& vcpuActions)
{
    sInstance.reset(new LogDaemonBusy(sm, logDogSavingState, logdogCheckSavingStateTimer,
                                      logdogSaveStateTimer, vcpuActions));
}

LogDaemonBusy::LogDaemonBusy(IVpdStateMachine* sm,
                             ILogDaemonActions& logDogSavingState,
                             ITimer& logdogCheckSavingStateTimer,
                             ITimer& logdogSaveStateTimer,
                             IVcpuActions& vcpuActions)
    : VpdStateBase{sm}
    , mLogDogSavingState{logDogSavingState}
    , mLogdogCheckSavingStateTimer{logdogCheckSavingStateTimer}
    , mLogdogSaveStateTimer{logdogSaveStateTimer}
    , mVcpuActions{vcpuActions}
{
}

void LogDaemonBusy::onEnter()
{
    MLOGD(LogFuncId::VPD, LogId::LogDaemonBusy_onEnter);
    if (mLogDogSavingState.isLogSavingComplete()) {
        transitionTo(WaitForLogDaemonStop::getInstance());
    }
    mLogdogCheckSavingStateTimer.start();
    mLogdogSaveStateTimer.start();
}

void LogDaemonBusy::onTimeout()
{
    MLOGI(LogFuncId::VPD, LogId::LogDaemonBusy_onTimeout);
    bool transition = false;
    if (mLogdogCheckSavingStateTimer.isExpired()) {
        MLOGI(LogFuncId::VPD,
              LogId::LogDaemonBusy_onTimeout_mLogdogCheckSavingStateTimer_is_expired);
        if (mLogDogSavingState.isLogSavingComplete()) {
            transition = true;
        }
        else {
            mLogdogCheckSavingStateTimer.start();
        }
    }
    if (mLogdogSaveStateTimer.isExpired()) {
        MLOGW(LogFuncId::VPD, LogId::LogDaemonBusy_onTimeout_mLogdogSaveStateTimer_is_expired);
        mVcpuActions.sendSelfRebootTrue();
        transition = true;
    }
    if (transition) {
        transitionTo(WaitForLogDaemonStop::getInstance());
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
