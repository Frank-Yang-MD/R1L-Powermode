/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IWAKELOCKMANAGER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IWAKELOCKMANAGER_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Interface for wakelocks management
 */
class IWakeLockManager {
public:
    // LCOV_EXCL_START
    virtual ~IWakeLockManager() = default;
    // LCOV_EXCL_STOP

    /**
     * @brief acquireWakeLock
     *
     * Acquires partial wake lock.
     */
    virtual void acquireWakeLock() = 0;

    /**
     * @brief releaseWakeLock
     *
     * Releases previously acquired partial wake lock.
     */
    virtual void releaseWakeLock() = 0;

    /**
     * @brief enableWakeLocks
     *
     * Disables wake lock ignoring mode.
     */
    virtual void enableWakeLocks() = 0;

    /**
     * @brief disableWakeLocks
     *
     * Enables wake lock ignoring mode.
     */
    virtual void disableWakeLocks() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IWAKELOCKMANAGER_H_
