/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IWAKEUPMANAGER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IWAKEUPMANAGER_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Interface for notify Andorid PowerManager about wake up event
 */
class IWakeUpManager {
public:
    // LCOV_EXCL_START
    virtual ~IWakeUpManager() = default;
    // LCOV_EXCL_STOP

    /**
     * Notify about wake up event
     */
    virtual void wakeUp() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IWAKEUPMANAGER_H_
