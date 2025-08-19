/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVPSMOUNT_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVPSMOUNT_H

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IVpsMount {
public:
    // LCOV_EXCL_START
    virtual ~IVpsMount() = default;
    // LCOV_EXCL_STOP

    /**
     * Handles 'Unmount complete' notification from VehiclePowerService
     */
    virtual void onVpsUnmountComplete() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVPSMOUNT_H
