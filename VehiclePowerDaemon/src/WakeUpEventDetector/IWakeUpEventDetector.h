/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IWKUP_DT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IWKUP_DT_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Interface to start polling for wake up event.
 */
class IWakeUpEventDetector {
public:
    // LCOV_EXCL_START
    virtual ~IWakeUpEventDetector() = default;
    // LCOV_EXCL_STOP

    /**
     * Start polling for wake up.
     */
    virtual void start() = 0;

    /**
     * Stop polling for wake up.
     */
    virtual void stop() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IWKUP_DT_H_
