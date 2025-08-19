/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "TimeoutHandler.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

TimeoutHandler::TimeoutHandler(ITimeoutNotification& stateMachine,
                               ITimeoutNotification& resetTimerHandler)
    : mStateMachine{stateMachine}
    , mResetTimerHandler{resetTimerHandler}
{
}

void TimeoutHandler::Process(int /* clientId */, common::EVpdEvent event)
{
    if (event == common::EVpdEvent::Timeout) {
        mStateMachine.onTimeout();
        mResetTimerHandler.onTimeout();
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::TimeoutHandler_Process_Wrong_event_index, event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
