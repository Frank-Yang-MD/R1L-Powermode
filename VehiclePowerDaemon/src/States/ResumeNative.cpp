/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ResumeNative.h"
#include "NotifyHelper.h"
#include "StateHolder.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForFwResumeComplete.h"
#include "WaitForVpsStart.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdState;

std::unique_ptr<VpdStateBase> ResumeNative::sInstance = std::unique_ptr<VpdStateBase>();

void ResumeNative::initInstance(IVpdStateMachine* sm, INotifyHelper& nh, IStateHolder& sh)
{
    sInstance.reset(new ResumeNative{sm, nh, sh});
}

void ResumeNative::onEnter()
{
    MLOGI(LogFuncId::VPD, LogId::ResumeNative_onEnter);

    mStateHolder.setVpdState(EVpdState::APP_RESUME);

    mNotifyHelper.notifyLog(EVpdState::APP_RESUME);
    mNotifyHelper.notifyAllNative(EVpdState::APP_RESUME);

    if (mStateMachine->context().isVpsDisconnected()) {
        sInstance->transitionTo(WaitForVpsStart::getInstance());
    }
    else {
        sInstance->transitionTo(WaitForFwResumeComplete::getInstance());
    }
}

ResumeNative::ResumeNative(IVpdStateMachine* sm, INotifyHelper& nh, IStateHolder& sh)
    : VpdStateBase{sm}
    , mNotifyHelper{nh}
    , mStateHolder{sh}
{
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
