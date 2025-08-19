/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "AbstractResetTimerHandler.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

AbstractResetTimerHandler::AbstractResetTimerHandler(std::unique_ptr<ITimer> timer)
    : mTimer{std::move(timer)}
    , mIsTimerStarted{false}
{
}

void AbstractResetTimerHandler::start()
{
    if (nullptr == mTimer) {
        MLOGW(LogFuncId::VPD, LogId::AbstractResetTimerHandler_start_mTimer_is_nullptr);
        return;
    }

    // Check if timer expired before start in order to prevent
    // timer restart during internal state transition
    if (mTimer->isExpired() || !mIsTimerStarted) {
        mIsTimerStarted = true;
        mTimer->start();
    }
}

void AbstractResetTimerHandler::onTimeout()
{
    if (nullptr == mTimer) {
        MLOGW(LogFuncId::VPD, LogId::AbstractResetTimerHandler_onTimeout_mTimer_is_nullptr);
        return;
    }

    if (mTimer->isExpired()) {
        performResetAction();
        mTimer->start();
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
