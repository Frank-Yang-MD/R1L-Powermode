/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "TimerManager.h"

#include <VehiclePowerDaemonLog.h>

#include <algorithm>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

TimerManager::TimerManager(ITimerCreator& timerCreator)
{
    mTimers[ETimerType::None] = nullptr;
    mTimers[ETimerType::ADTRTimer] = timerCreator.getAbnormalHandler();
    mTimers[ETimerType::WDTRTimer] = timerCreator.getWatchDogHandler();

    mCurrentTimer = mTimers[ETimerType::None];

    std::for_each(mTimers.begin(), mTimers.end(), [](auto& item) {
        if (item.second)
            item.second->start();
    });
}

void TimerManager::changeTimerHandler(ETimerType timerType)
{
    std::lock_guard<std::mutex> lock{mMutex};

    MLOGI(LogFuncId::VPD, LogId::TimerManager_changeTimer, toString(timerType));
    mCurrentTimer = mTimers[timerType];
}

void TimerManager::onTimeout()
{
    std::lock_guard<std::mutex> lock{mMutex};
    if (mCurrentTimer != nullptr) {
        mCurrentTimer->onTimeout();
    }
    else {
        MLOGI(LogFuncId::VPD, LogId::TimerManager_onTimeout_skipEvent);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
