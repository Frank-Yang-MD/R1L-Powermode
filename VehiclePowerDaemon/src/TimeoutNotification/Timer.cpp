/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#define LOG_TAG "VPD.Timer"

#include "Timer.h"
#include <utils/Log.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

Timer::Timer(ITimeoutControl& timeoutControl,
             const IClock& clock,
             std::chrono::microseconds timeout)
    : mTimeoutControl{timeoutControl}
    , mClock{clock}
    , mTimeout{timeout}
    , mStartTime{std::chrono::time_point<std::chrono::steady_clock>::max()}
{
}

void Timer::start()
{
    mStartTime = mClock.clock();
    mTimeoutControl.setupTimeout(mTimeout);
}

bool Timer::isExpired() const
{
    if (mStartTime == std::chrono::time_point<std::chrono::steady_clock>::max()) {
        return false;
    }
    return mClock.clock() > mStartTime + mTimeout;
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
