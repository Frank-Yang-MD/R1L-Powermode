/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ILOGDAEMONACTIONS_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ILOGDAEMONACTIONS_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Actions to LogDaemon
 */
class ILogDaemonActions {
public:
    // LCOV_EXCL_START
    virtual ~ILogDaemonActions() = default;
    // LCOV_EXCL_STOP

    /**
     * Notify log daemon about system state change
     */
    virtual bool isLogSavingComplete() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ILOGDAEMONACTIONS_H_
