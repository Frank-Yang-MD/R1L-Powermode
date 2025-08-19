/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_INOTIFYHELPER_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_INOTIFYHELPER_H

#include "EVpdNotifications.h"
#include "EVpdState.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Send notification about system state change
 */
class INotifyHelper {
public:
    // LCOV_EXCL_START
    virtual ~INotifyHelper() = default;
    // LCOV_EXCL_STOP

    /**
     * Notify native apps/services about system state change
     */
    virtual void notifyAllNative(common::EVpdState vpdState) = 0;
    /**
     * Notify Vehicle power service about system state change
     */
    virtual void notifyVps(common::EVpdState vpdState) = 0;
    /**
     * Send VPD notification to VehiclePowerService
     */
    virtual void notifyVps(common::EVpdNotifications vpdNotification) = 0;
    /**
     * Notify Logdog about system state change
     */
    virtual void notifyLog(common::EVpdState vpdState) = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_INOTIFYHELPER_H
