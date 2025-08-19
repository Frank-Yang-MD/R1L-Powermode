/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVCPUACTIONS_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVCPUACTIONS_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Actions to VCPU
 */
class IVcpuActions {
public:
    // LCOV_EXCL_START
    virtual ~IVcpuActions() = default;
    // LCOV_EXCL_STOP

    /**
     * Sends 'Abnormal detection timer reset' request to VCPU
     */
    virtual void sendAdtrRequest() = 0;

    /**
     * Sends 'Start complete' notification to VCPU
     */
    virtual void sendStartComplete() = 0;

    /**
     * Sends 'Shutdown cancel accept' message to VCPU
     */
    virtual void sendShutdownCancelAccept() = 0;

    /**
     * Sends 'Shutdown processing start' notification to VCPU
     */
    virtual void sendShutdownProcessingStart() = 0;

    /**
     * Sends 'Shutdown cancel complete' message to VCPU
     */
    virtual void sendShutdownCancelComplete() = 0;

    /**
     * Sends 'Watchdog time reset' request to VCPU
     */
    virtual void sendWdtrRequest() = 0;

    /**
     * Sends 'Self Reboot True - request Cold Boot at the next boot' request to VCPU
     */
    virtual void sendSelfRebootTrue() = 0;

    /**
     * Sends 'Main Start Complete' notification (7187h) to VCPU
     */
    virtual void sendMainStartComplete() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVCPUACTIONS_H_
