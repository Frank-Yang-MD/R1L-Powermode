/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPDNOTIFICATIONHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPDNOTIFICATIONHANDLER_H_

#include "EVpdNotifications.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

/**
 * IVpdNotificationHandler - interface to handle received notification from VPD.
 */
class IVpdNotificationHandler {
public:
    virtual ~IVpdNotificationHandler() = default;

    /**
     * To be called when VPD notification is received.
     */
    virtual bool onVpdNotification(common::EVpdNotifications) = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPDNOTIFICATIONHANDLER_H_
