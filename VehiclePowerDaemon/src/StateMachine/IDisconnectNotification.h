/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IDISCONNECTNOTIFICATION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IDISCONNECTNOTIFICATION_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * IDisconnectNotification
 */
class IDisconnectNotification {
public:
    // LCOV_EXCL_START
    virtual ~IDisconnectNotification() = default;
    // LCOV_EXCL_STOP

    /**
     * Notification about VehiclePowerService disconnection
     */
    virtual void onDisconnectVps() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IDISCONNECTNOTIFICATION_H_
