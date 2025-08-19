/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_TIMER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_TIMER_H_

#include "IClock.h"
#include "ITimeoutControl.h"
#include "ITimer.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class Timer final : public ITimer {
public:
    Timer(ITimeoutControl& timeoutControl, const IClock& clock, std::chrono::microseconds timeout);
    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;

    void start() override;
    bool isExpired() const override;

private:
    ITimeoutControl& mTimeoutControl;
    const IClock& mClock;
    std::chrono::microseconds mTimeout;
    std::chrono::time_point<std::chrono::steady_clock> mStartTime;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_TIMER_H_
