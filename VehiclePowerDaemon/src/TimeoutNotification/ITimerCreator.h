/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ITIMERCREATOR_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ITIMERCREATOR_H_

#include "AbnormalTimerHandler.h"
#include "ITimer.h"
#include "IVcpuResetTimer.h"
#include "WatchDogTimerHandler.h"

#include <chrono>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class ITimerCreator {
public:
    // LCOV_EXCL_START
    virtual ~ITimerCreator() = default;
    // LCOV_EXCL_STOP

    virtual ITimer* createTimer(const std::chrono::microseconds&) = 0;

    virtual IVcpuResetTimer* getAbnormalHandler() = 0;
    virtual IVcpuResetTimer* getWatchDogHandler() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ITIMERCREATOR_H_
