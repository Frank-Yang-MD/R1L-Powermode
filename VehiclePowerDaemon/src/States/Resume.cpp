/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "Resume.h"
#include "IVpdStateMachine.h"
#include "NormalOperation.h"
#include "WaitForAppStopVps.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> Resume::sInstance = std::unique_ptr<VpdStateBase>();

void Resume::initInstance(IVpdStateMachine* sm, IPowerStateHolder& powerStateHolder)
{
    sInstance.reset(new (std::nothrow) Resume(sm, powerStateHolder));
}

Resume::Resume(IVpdStateMachine* sm, IPowerStateHolder& powerStateHolder)
    : VpdStateBase(sm)
    , mPowerStateHolder{powerStateHolder}
{
}

void Resume::onEnter()
{
    if (mPowerStateHolder.isShutdownState()) {
        sInstance->transitionTo(WaitForAppStopVps::getInstance());
    }
    else {
        sInstance->transitionTo(NormalOperation::getInstance());
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
