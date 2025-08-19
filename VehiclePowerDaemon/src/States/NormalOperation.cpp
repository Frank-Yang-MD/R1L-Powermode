/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "NormalOperation.h"
#include "IStateHolder.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForAppStopVps.h"
#include "WaitForVpsStart.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::unique_ptr<VpdStateBase> NormalOperation::sInstance = std::unique_ptr<VpdStateBase>();

void NormalOperation::initInstance(IVpdStateMachine* sm,
                                   IPowerStateHolder& powerStateHolder,
                                   IVpdRestartHolder& vpdRestartHolder,
                                   IStateHolder& stateHolder)
{
    sInstance.reset(new (std::nothrow)
                        NormalOperation{sm, powerStateHolder, vpdRestartHolder, stateHolder});
}

NormalOperation::NormalOperation(IVpdStateMachine* sm,
                                 IPowerStateHolder& powerStateHolder,
                                 IVpdRestartHolder& vpdRestartHolder,
                                 IStateHolder& stateHolder)
    : VpdStateBase{sm}
    , mPowerStateHolder{powerStateHolder}
    , mVpdRestartHolder{vpdRestartHolder}
    , mStateHolder{stateHolder}
{
}

void NormalOperation::onEnter()
{
    mStateHolder.setVpdState(common::EVpdState::APP_START_COLDBOOT);
    mVpdRestartHolder.vpdStarted();
    mPowerStateHolder.onPowerStateChange(PowerState::NORMAL);
}

void NormalOperation::onPowerStateChange(PowerState)
{
    if (mPowerStateHolder.isShutdownState()) {
        transitionTo(WaitForAppStopVps::getInstance());
    }
}

void NormalOperation::onDisconnectVps()
{
    VpdStateBase::onDisconnectVps();
    transitionTo(WaitForVpsStart::getInstance());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
