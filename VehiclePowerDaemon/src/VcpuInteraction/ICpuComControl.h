/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICPUCOMMCONTROL_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICPUCOMMCONTROL_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Communication with VCPU
 */
class ICpuCommControl {
public:
    // LCOV_EXCL_START
    virtual ~ICpuCommControl() = default;
    // LCOV_EXCL_STOP

    /**
     * Subscribes to commands from VCPU
     * Returns true if subscriptions were successfull
     */
    virtual bool start() = 0;
    virtual void stop() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICPUCOMMCONTROL_H_
