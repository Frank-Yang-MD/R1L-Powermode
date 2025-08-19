/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_INOTIFICATIONRECEIVER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_INOTIFICATIONRECEIVER_H_

#include "EVpdNotifications.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class IVpdNotificationHandler;

/**
 * INotificationReceiver - is a interface to setup notification receiver.
 */
class INotificationReceiver {
public:
    virtual ~INotificationReceiver() = default;

    /**
     * Add handler for specific VPD notification.
     */
    virtual void addHandler(common::EVpdNotifications, IVpdNotificationHandler*) = 0;

    /**
     * Trigger processing of received but not handled notifications.
     */
    virtual void processReceivedNotifications() = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_INOTIFICATIONRECEIVER_H_
