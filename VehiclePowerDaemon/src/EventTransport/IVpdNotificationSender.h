/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVPDNOTIFICATIONSENDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVPDNOTIFICATIONSENDER_H_

#include "EVpdNotifications.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IVpdNotificationSender {
public:
    // LCOV_EXCL_START
    virtual ~IVpdNotificationSender() = default;
    // LCOV_EXCL_STOP

    virtual void sendNotification(int, common::EVpdNotifications) = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVPDNOTIFICATIONSENDER_H_
