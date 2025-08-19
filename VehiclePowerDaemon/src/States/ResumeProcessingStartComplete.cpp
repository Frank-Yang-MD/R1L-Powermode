/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ResumeProcessingStartComplete.h"

#include "IVcpuActions.h"
#include "IVpdStateMachine.h"
#include "WakeUp.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> ResumeProcessingStartComplete::sInstance =
    std::unique_ptr<VpdStateBase>();

void ResumeProcessingStartComplete::initInstance(IVpdStateMachine* sm,
                                                 IVcpuActions& cpuComActions,
                                                 IPowerStateHolder& powerStateHolder,
                                                 ITimerManager& timerManager)
{
    sInstance.reset(new (std::nothrow) ResumeProcessingStartComplete{
        sm, cpuComActions, powerStateHolder, timerManager});
}

ResumeProcessingStartComplete::ResumeProcessingStartComplete(IVpdStateMachine* sm,
                                                             IVcpuActions& cpuComActions,
                                                             IPowerStateHolder& powerStateHolder,
                                                             ITimerManager& timerManager)
    : VpdStateBase{sm}
    , mVcpuActions{cpuComActions}
    , mPowerStateHolder{powerStateHolder}
    , mTimerManager{timerManager}
{
}

void ResumeProcessingStartComplete::onEnter() { mVcpuActions.sendStartComplete(); }

void ResumeProcessingStartComplete::onExit()
{
    mTimerManager.changeTimerHandler(ETimerType::WDTRTimer);
}

void ResumeProcessingStartComplete::onResumeProcessingStartCompleteSentSuccess()
{
    mPowerStateHolder.onPowerStateChange(PowerState::NORMAL);
    sInstance->transitionTo(WakeUp::getInstance());
}

void ResumeProcessingStartComplete::onResumeProcessingStartCompleteSentFailure()
{
    mVcpuActions.sendStartComplete();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
