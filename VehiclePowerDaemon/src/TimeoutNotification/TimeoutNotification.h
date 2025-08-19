/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_TIMEOUTNOTIFICATION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_TIMEOUTNOTIFICATION_H_

#include "AbnormalTimerHandler.h"
#include "ITimerCreator.h"
#include "TimeoutControl.h"
#include "VpdClock.h"
#include "WatchDogTimerHandler.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class TimeoutNotification : public ITimerCreator {
public:
    TimeoutNotification(ITimeoutControl& timeoutControl,
                        IVcpuActions& vcpuActions,
                        std::chrono::microseconds abnormalPeriod,
                        std::chrono::microseconds watchdogPeriod);

    virtual ITimer* createTimer(const std::chrono::microseconds&) override;
    virtual IVcpuResetTimer* getAbnormalHandler() override;
    virtual IVcpuResetTimer* getWatchDogHandler() override;

private:
    VpdClock mClock;
    ITimeoutControl& mTimeoutControl;

    std::unique_ptr<AbnormalTimerHandler> mAbnormalTimerHandler;
    std::unique_ptr<WatchDogTimerHandler> mWatchDogTimerHandler;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_TIMEOUTNOTIFICATION_H_
