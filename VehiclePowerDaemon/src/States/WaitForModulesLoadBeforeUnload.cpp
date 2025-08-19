/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForModulesLoadBeforeUnload.h"

#include "WaitForModulesUnload.h"

#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> WaitForModulesLoadBeforeUnload::sInstance =
    std::unique_ptr<VpdStateBase>{};

void WaitForModulesLoadBeforeUnload::initInstance(IVpdStateMachine* stateMachine,
                                                  ITimer& checkTimer,
                                                  ITimer& timeout,
                                                  IKernelModulesControl& kernelModulesControl)
{
    sInstance.reset(new WaitForModulesLoadBeforeUnload{stateMachine, checkTimer, timeout,
                                                       kernelModulesControl});
}

WaitForModulesLoadBeforeUnload::WaitForModulesLoadBeforeUnload(
    IVpdStateMachine* stateMachine,
    ITimer& checkTimer,
    ITimer& timeout,
    IKernelModulesControl& kernelModulesControl)
    : VpdStateBase{stateMachine}
    , mCheckTimer{checkTimer}
    , mTimeout{timeout}
    , mKernelModulesControl{kernelModulesControl}
{
}

void WaitForModulesLoadBeforeUnload::onEnter()
{
    if (mKernelModulesControl.skipModulesUnloadedCheck() ||
        mKernelModulesControl.isModulesLoaded()) {
        toNextState();
    }
    else {
        mCheckTimer.start();
        mTimeout.start();
    }
}

void WaitForModulesLoadBeforeUnload::onTimeout()
{
    if (mTimeout.isExpired()) {
        MLOGW(LogFuncId::VPD, LogId::WaitForModulesLoadBeforeUnload_onTimeout_timeout);
        toNextState();
    }
    else if (mCheckTimer.isExpired()) {
        if (mKernelModulesControl.isModulesLoaded()) {
            MLOGW(LogFuncId::VPD, LogId::WaitForModulesLoadBeforeUnload_onTimeout_success);
            toNextState();
        }
        else {
            mCheckTimer.start();
        }
    }
    else {
        // no need to handle other timeouts
    }
}

void WaitForModulesLoadBeforeUnload::toNextState()
{
    sInstance->transitionTo(WaitForModulesUnload::getInstance());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
