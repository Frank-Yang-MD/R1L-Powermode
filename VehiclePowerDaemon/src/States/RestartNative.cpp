/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "RestartNative.h"
#include "NotifyHelper.h"
#include "StateHolder.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForFwRestartComplete.h"
#include "WaitForVpsStart.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdState;

std::unique_ptr<VpdStateBase> RestartNative::sInstance = std::unique_ptr<VpdStateBase>();

RestartNative::RestartNative(IVpdStateMachine* sm, INotifyHelper& nh, IStateHolder& sh)
    : VpdStateBase{sm}
    , mNotifyHelper{nh}
    , mStateHolder{sh}
{
}

void RestartNative::initInstance(IVpdStateMachine* sm, INotifyHelper& nh, IStateHolder& sh)
{
    sInstance.reset(new RestartNative(sm, nh, sh));
}

void RestartNative::onEnter()
{
    MLOGI(LogFuncId::VPD, LogId::RestartNative_onEnter);

    mStateHolder.setVpdState(EVpdState::APP_RESTART);

    mNotifyHelper.notifyLog(EVpdState::APP_RESTART);
    mNotifyHelper.notifyAllNative(EVpdState::APP_RESTART);

    if (mStateMachine->context().isVpsDisconnected()) {
        transitionTo(WaitForVpsStart::getInstance());
    }
    else {
        transitionTo(WaitForFwRestartComplete::getInstance());
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
