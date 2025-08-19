/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPSCALLBACK_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPSCALLBACK_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class IVpsCallback {
public:
    virtual ~IVpsCallback() = default;

    /**
     * Method to be called when VPD requests VPS to restart FW services.
     */
    virtual void onFwRestart() = 0;

    /**
     * Method to be called when VPD requests VPS to resume FW services.
     */
    virtual void onFwResume() = 0;

    /**
     * Method to be called when VPD requests VPS to stop FW services.
     */
    virtual void onFwStop() = 0;

    /**
     * Method to be called when VPD notifies VPS on TimeOutError.
     */
    virtual void onTimeOutError() = 0;

    /**
     * Method to be called when VPD notifies VPS on requestMount.
     */
    virtual void requestMount() = 0;

    /**
     * Method to be called when VPD notifies VPS on requestUnmount.
     */
    virtual void requestUnmount() = 0;

    /**
     * Notifies, that BootDaemon has finished its work
     * and startup animation or disclamier wasn't displayed.
     */
    virtual void handOffResponsibility() = 0;

    /**
     * Notifies, that BootDaemon
     * has finished its work and only startup animation was displayed.
     */
    virtual void handOffResponsibilityStartupAnimationDisplayed() = 0;

    /**
     * Notifies, that BootDaemon has
     * finished its work and startup animation and disclaimer was displayed.
     */
    virtual void handOffResponsibilityStartupAnimationAndDiDisplayed() = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPSCALLBACK_H_
