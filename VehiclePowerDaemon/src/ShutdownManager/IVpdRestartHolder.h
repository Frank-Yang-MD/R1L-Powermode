/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IVPDRESTARTHOLDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IVPDRESTARTHOLDER_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * @brief The IVpdRestartHolder is an interface to a storage of VPD
 * restart information.
 */
class IVpdRestartHolder {
public:
    // LCOV_EXCL_START
    virtual ~IVpdRestartHolder() = default;
    // LCOV_EXCL_STOP

    /**
     * @brief vpdStarted - to be called when VPD completely started.
     */
    virtual void vpdStarted() = 0;

    /**
     * @brief isVpdRestarted - indicates whether VPD restarted or not.
     * @return - true if if VPD restarted, false otherwise.
     */
    virtual bool isVpdRestarted() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IVPDRESTARTHOLDER_H_
