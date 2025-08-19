/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_TIMERMANAGER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_TIMERMANAGER_H_

#include "ETimerType.h"
#include "ITimeoutNotification.h"
#include "ITimerCreator.h"
#include "ITimerManager.h"
#include "IVcpuResetTimer.h"

#include <map>
#include <mutex>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Implementation of the ITimerManager interface
 */
class TimerManager
    : public ITimerManager
    , public ITimeoutNotification {
public:
    explicit TimerManager(ITimerCreator& timerCreator);

    void changeTimerHandler(ETimerType timerType) override;
    void onTimeout() override;

private:
    std::map<ETimerType, IVcpuResetTimer*> mTimers;
    IVcpuResetTimer* mCurrentTimer;

    std::mutex mMutex;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_TIMERMANAGER_H_
