/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "TimeoutNotification.h"
#include "Timer.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

TimeoutNotification::TimeoutNotification(ITimeoutControl& timeoutControl,
                                         IVcpuActions& vcpuActions,
                                         std::chrono::microseconds abnormalPeriod,
                                         std::chrono::microseconds watchdogPeriod)
    : mClock{}
    , mTimeoutControl{timeoutControl}
{
    mAbnormalTimerHandler.reset(new (std::nothrow) AbnormalTimerHandler{
        std::unique_ptr<ITimer>(createTimer(abnormalPeriod)), vcpuActions});

    mWatchDogTimerHandler.reset(new (std::nothrow) WatchDogTimerHandler{
        std::unique_ptr<ITimer>(createTimer(watchdogPeriod)), vcpuActions});
}

ITimer* TimeoutNotification::createTimer(const std::chrono::microseconds& timeout)
{
    return new (std::nothrow) Timer(mTimeoutControl, mClock, timeout);
}

IVcpuResetTimer* TimeoutNotification::getAbnormalHandler() { return mAbnormalTimerHandler.get(); }

IVcpuResetTimer* TimeoutNotification::getWatchDogHandler() { return mWatchDogTimerHandler.get(); }

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
