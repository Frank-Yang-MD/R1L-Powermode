/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForModulesUnload.h"

#include "SendShutdownProcessingStart.h"
#include "WaitForModulesLoadAfterSuspendCancel.h"

#include <VehiclePowerDaemonLog.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> WaitForModulesUnload::sInstance = std::unique_ptr<VpdStateBase>{};

void WaitForModulesUnload::initInstance(IVpdStateMachine* stateMachine,
                                        ITimer& checkTimer,
                                        ITimer& timeoutTimer,
                                        IKernelModulesControl& kernelModulesControl,
                                        IPowerStateHolder& powerStateHolder)
{
    sInstance.reset(new WaitForModulesUnload{stateMachine, checkTimer, timeoutTimer,
                                             kernelModulesControl, powerStateHolder});
}
WaitForModulesUnload::WaitForModulesUnload(IVpdStateMachine* stateMachine,
                                           ITimer& checkTimer,
                                           ITimer& timeoutTimer,
                                           IKernelModulesControl& kernelModulesControl,
                                           IPowerStateHolder& powerStateHolder)
    : VpdStateBase{stateMachine}
    , mCheckTimer{checkTimer}
    , mTimeout{timeoutTimer}
    , mKernelModulesControl{kernelModulesControl}
    , mPowerStateHolder{powerStateHolder}
{
}

void WaitForModulesUnload::onEnter()
{
    mKernelModulesControl.unloadModules();
    mCheckTimer.start();
    mTimeout.start();
}

void WaitForModulesUnload::onTimeout()
{
    if (mTimeout.isExpired()) {
        MLOGW(LogFuncId::VPD, LogId::WaitForModulesUnload_onTimeout_timeout);
        toNextState();
        return;
    }
    if (mCheckTimer.isExpired()) {
        if (mKernelModulesControl.isModulesUnloaded()) {
            MLOGI(LogFuncId::VPD, LogId::WaitForModulesUnload_onTimeout_success);
            toNextState();
        }
        mCheckTimer.start();
    }
}

void WaitForModulesUnload::toNextState()
{
    if (mPowerStateHolder.isShutdownState()) {
        sInstance->transitionTo(SendShutdownProcessingStart::getInstance());
    }
    else {
        sInstance->transitionTo(WaitForModulesLoadAfterSuspendCancel::getInstance());
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
