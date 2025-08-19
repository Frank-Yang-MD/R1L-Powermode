/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IAPPRESTARTCOMPLETENOTIFICATION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IAPPRESTARTCOMPLETENOTIFICATION_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * IAppRestartCompleteNotification interface provide internal api
 * for process notification from VPS that app restart complete.
 */
class IAppRestartCompleteNotification {
public:
    // LCOV_EXCL_START
    virtual ~IAppRestartCompleteNotification() = default;
    // LCOV_EXCL_STOP

    virtual void onAppRestartComplete() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IAPPRESTARTCOMPLETENOTIFICATION_H_
