/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "StateHolder.h"
#include "EVpdState.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdState;

StateHolder::StateHolder()
    : mCurrentState{EVpdState::APP_START_NA}
{
    MLOGI(LogFuncId::VPD, LogId::StateHolder_ctor_create_instance_Default_state, currentStateInt());
}

EVpdState StateHolder::getVpdState() const
{
    MLOGI(LogFuncId::VPD, LogId::StateHolder_getVpdState_state, currentStateInt());

    return mCurrentState;
}

void StateHolder::setVpdState(EVpdState vpdState)
{
    auto oldState = currentStateInt();
    mCurrentState = vpdState;

    MLOGI(LogFuncId::VPD, LogId::StateHolder_setVpdState_VPD_state_change_from_to, oldState,
          currentStateInt());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
