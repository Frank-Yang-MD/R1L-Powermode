/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTOPCOMPLETEVPS_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTOPCOMPLETEVPS_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IStopCompleteVps {
public:
    // LCOV_EXCL_START
    virtual ~IStopCompleteVps() = default;
    // LCOV_EXCL_STOP

    /**
     * Handles 'AppStop complete' notification from VehiclePowerService
     */
    virtual void onAppStopCompleteVps() = 0;
    /**
     * Handles 'FwStop complete' notification from VehiclePowerService
     */
    virtual void onFwStopCompleteVps() = 0;
    /**
     * Handles 'Stop failed' notification from VehiclePowerService
     */
    virtual void onStopFailedVps() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTOPCOMPLETEVPS_H_
