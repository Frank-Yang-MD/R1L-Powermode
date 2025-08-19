/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForModulesLoadAfterSuspendCancel.h"

#include "WaitForMount.h"

#include <VehiclePowerDaemonLog.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> WaitForModulesLoadAfterSuspendCancel::sInstance =
    std::unique_ptr<VpdStateBase>{};

void WaitForModulesLoadAfterSuspendCancel::initInstance(IVpdStateMachine* stateMachine,
                                                        IKernelModulesControl& kernelModulesControl)
{
    sInstance.reset(new WaitForModulesLoadAfterSuspendCancel{stateMachine, kernelModulesControl});
}

WaitForModulesLoadAfterSuspendCancel::WaitForModulesLoadAfterSuspendCancel(
    IVpdStateMachine* stateMachine,
    IKernelModulesControl& kernelModulesControl)
    : VpdStateBase{stateMachine}
    , mKernelModulesControl{kernelModulesControl}
{
}

void WaitForModulesLoadAfterSuspendCancel::onEnter()
{
    mKernelModulesControl.loadModules();
    sInstance->transitionTo(WaitForMount::getInstance());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
