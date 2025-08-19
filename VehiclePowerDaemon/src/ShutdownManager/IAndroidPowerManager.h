/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IANDROIDPOWERMANAGER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IANDROIDPOWERMANAGER_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Android PowerManager adapter
 */
class IAndroidPowerManager {
public:
    // LCOV_EXCL_START
    virtual ~IAndroidPowerManager() = default;
    // LCOV_EXCL_STOP

    virtual void shutdown() = 0;
    virtual void suspend() = 0;
    virtual void reboot() = 0;
    virtual void rebootRecovery() = 0;
    virtual void wakeUp() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IANDROIDPOWERMANAGER_H_
