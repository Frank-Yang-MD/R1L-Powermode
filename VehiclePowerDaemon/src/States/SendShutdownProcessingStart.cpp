/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "SendShutdownProcessingStart.h"
#include "IPowerStateHolder.h"
#include "IVcpuActions.h"
#include "IVpdStateMachine.h"
#include "PollingWakeUp.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> SendShutdownProcessingStart::sInstance =
    std::unique_ptr<VpdStateBase>();

void SendShutdownProcessingStart::initInstance(IVpdStateMachine* sm,
                                               IVcpuActions& cpuComActions,
                                               IPowerStateHolder& powerStateHolder,
                                               ITimerManager& timerManager)
{
    sInstance.reset(new (std::nothrow) SendShutdownProcessingStart{sm, cpuComActions,
                                                                   powerStateHolder, timerManager});
}

SendShutdownProcessingStart::SendShutdownProcessingStart(IVpdStateMachine* sm,
                                                         IVcpuActions& cpuComActions,
                                                         IPowerStateHolder& powerStateHolder,
                                                         ITimerManager& timerManager)
    : VpdStateBase{sm}
    , mVcpuActions{cpuComActions}
    , mPowerStateHolder{powerStateHolder}
    , mTimerManager{timerManager}
    , mPreviousState{PowerState::NORMAL}
{
}

void SendShutdownProcessingStart::onEnter()
{
    mPreviousState = mPowerStateHolder.getPowerState();
    mPowerStateHolder.onPowerStateChange(PowerState::WAIT_FOR_CPUCOM_DELIVERY);
    mTimerManager.changeTimerHandler(ETimerType::None);
    mVcpuActions.sendShutdownProcessingStart();
}

void SendShutdownProcessingStart::onExit() { mPowerStateHolder.onPowerStateChange(mPreviousState); }

void SendShutdownProcessingStart::onShutdownProcessingStartSent()
{
    sInstance->transitionTo(PollingWakeUp::getInstance());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
