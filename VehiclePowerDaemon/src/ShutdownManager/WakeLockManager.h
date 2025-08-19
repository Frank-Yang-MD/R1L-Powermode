/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAKELOCKMANAGER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAKELOCKMANAGER_H_

#include "IWakeLockManager.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * @brief The WakeLockManager class
 *
 * Implementation of wake lock management.
 * This class uses sysfs nodes for wake lock
 * management.
 */
class WakeLockManager final : public IWakeLockManager {
public:
    WakeLockManager() = default;
    virtual ~WakeLockManager() override = default;

    virtual void acquireWakeLock() override;
    virtual void releaseWakeLock() override;

    virtual void enableWakeLocks() override;
    virtual void disableWakeLocks() override;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAKELOCKMANAGER_H_
