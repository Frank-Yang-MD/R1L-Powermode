/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISHUTDOWNACTION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISHUTDOWNACTION_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Interface to Android PowerManager action
 */
class IShutdownAction {
public:
    // LCOV_EXCL_START
    virtual ~IShutdownAction() = default;
    // LCOV_EXCL_STOP

    /**
     * Send request to Android PowerManager
     */
    virtual void performShutdownAction() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISHUTDOWNACTION_H_
