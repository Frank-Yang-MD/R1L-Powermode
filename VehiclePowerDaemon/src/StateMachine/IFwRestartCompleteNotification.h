/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IFWRESTARTCOMPLETENOTIFICATION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IFWRESTARTCOMPLETENOTIFICATION_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * IFwRestartCompleteNotification interface provide internal api
 * for process notification from VPS that fw restart complete.
 */
class IFwRestartCompleteNotification {
public:
    // LCOV_EXCL_START
    virtual ~IFwRestartCompleteNotification() = default;
    // LCOV_EXCL_STOP

    virtual void onFwRestartComplete() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IFWRESTARTCOMPLETENOTIFICATION_H_
