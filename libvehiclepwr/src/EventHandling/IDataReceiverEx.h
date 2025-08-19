/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPOWER_IDATARECEIVEREX_HPP_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPOWER_IDATARECEIVEREX_HPP_

#include "EVpdNotifications.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class IDataReceiverEx {
public:
    virtual ~IDataReceiverEx() = default;

    /**
     * Notifies the client the connection established.
     */
    virtual void onConnect() = 0;

    /**
     * Notifies the client the connection lost.
     */
    virtual void onDisconnect() = 0;

    /**
     * @brief onDataReady - to be called when notification from VPD is received.
     */
    virtual void onNotificationReceived(common::EVpdNotifications) = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif /*COM_MITSUBISHIELECTRIC_AHU_VEHICLEPOWER_IDATARECEIVEREX_HPP_*/
