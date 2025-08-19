/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "PollingWakeUp.h"
#include "IShutdownAction.h"
#include "ResumeProcessingStartComplete.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> PollingWakeUp::sInstance = std::unique_ptr<VpdStateBase>();

void PollingWakeUp::initInstance(IVpdStateMachine* sm,
                                 IWakeUpEventDetector& up,
                                 IWakeLockManager& lm,
                                 IShutdownAction& sa)
{
    sInstance.reset(new (std::nothrow) PollingWakeUp(sm, up, lm, sa));
}

PollingWakeUp::PollingWakeUp(IVpdStateMachine* sm,
                             IWakeUpEventDetector& up,
                             IWakeLockManager& lm,
                             IShutdownAction& sa)

    : VpdStateBase(sm)
    , mWakeUpEventDetector(up)
    , mWakeLockManager{lm}
    , mShutdownAction{sa}
{
}

void PollingWakeUp::onEnter()
{
    mWakeLockManager.releaseWakeLock();
    mWakeLockManager.disableWakeLocks();
    mShutdownAction.performShutdownAction();
}

void PollingWakeUp::onWakeUp()
{
    sInstance->transitionTo(ResumeProcessingStartComplete::getInstance());
}

void PollingWakeUp::onExit()
{
    mWakeLockManager.enableWakeLocks();
    mWakeLockManager.acquireWakeLock();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
