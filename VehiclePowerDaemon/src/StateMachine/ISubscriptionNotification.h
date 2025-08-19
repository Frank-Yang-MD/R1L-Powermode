/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISUBSCRIPTIONNOTIFICATION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISUBSCRIPTIONNOTIFICATION_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class ISubscriptionNotification {
public:
    // LCOV_EXCL_START
    virtual ~ISubscriptionNotification() = default;
    // LCOV_EXCL_STOP

    /**
     * Subscription notification from native clients
     */
    virtual void onSubscribeNative(int) = 0;

    /**
     * Subscription notification from VehiclePowerService
     */
    virtual void onSubscribeVps(int) = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISUBSCRIPTIONNOTIFICATION_H_
