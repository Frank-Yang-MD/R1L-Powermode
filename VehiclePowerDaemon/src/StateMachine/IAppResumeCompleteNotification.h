/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IAPPRESUMECOMPLETENOTIFICATION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IAPPRESUMECOMPLETENOTIFICATION_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * IAppResumeCompleteNotification interface provide internal api
 * for process notification from VPS that app resume is complete.
 */
class IAppResumeCompleteNotification {
public:
    // LCOV_EXCL_START
    virtual ~IAppResumeCompleteNotification() = default;
    // LCOV_EXCL_STOP

    virtual void onAppResumeComplete() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IAPPRESUMECOMPLETENOTIFICATION_H_
