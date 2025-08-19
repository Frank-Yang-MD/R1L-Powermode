/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WakeUp.h"

#include "IVcpuActions.h"
#include "IWakeUpManager.h"
#include "ResumeNative.h"
#include <IKernelModulesControl.h>
#include <VehiclePowerDaemonLog.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> WakeUp::sInstance = std::unique_ptr<VpdStateBase>{};

void WakeUp::initInstance(IVpdStateMachine* stateMachine,
                          IKernelModulesControl& kernelModulesControl,
                          IWakeUpManager& wakeUpManager)
{
    sInstance.reset(new WakeUp{stateMachine, kernelModulesControl, wakeUpManager});
}

WakeUp::WakeUp(IVpdStateMachine* stateMachine,
               IKernelModulesControl& kernelModulesControl,
               IWakeUpManager& wakeUpManager)
    : VpdStateBase{stateMachine}
    , mKernelModulesControl{kernelModulesControl}
    , mWakeUpManager{wakeUpManager}
{
}

void WakeUp::onEnter()
{
    mKernelModulesControl.loadModules();
    mWakeUpManager.wakeUp();
    sInstance->transitionTo(ResumeNative::getInstance());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
