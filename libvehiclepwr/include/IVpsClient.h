/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPSCLIENT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPSCLIENT_H_

#include "INativeClient.h"
#include "IVpsCallback.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class IVpsClient : public virtual INativeClient {
public:
    virtual ~IVpsClient() = default;

    /**
     * Send Vehicle Power Service start complete notification to Vehicle Power Daemon.
     */
    virtual void startProcessingComplete() = 0;
    /**
     * Send Vehicle Power Service start failed notification to Vehicle Power Daemon.
     */
    virtual void startProcessingFailed() = 0;
    /**
     * Send notification about all App services stopped event to VPD.
     */
    virtual void appStopProcessingComplete() = 0;
    /**
     * Send notification about all FW services restart event to VPD.
     */
    virtual void fwRestartProcessingComplete() = 0;
    /**
     * Send notification about all FW services resume event to VPD.
     */
    virtual void fwResumeProcessingComplete() = 0;
    /**
     * Send notification about all FW services stopped event to VPD.
     */
    /**
     * Send notification about all app services restart event to VPD.
     */
    virtual void appRestartProcessingComplete() = 0;
    /**
     * Send notification about all app services resume event to VPD.
     */
    virtual void appResumeProcessingComplete() = 0;
    /**
     * Send notification about all FW services stopped event to VPD.
     */
    virtual void fwStopProcessingComplete() = 0;
    /**
     * Send stop failed notification to Vehicle Power Daemon.
     */
    virtual void stopProcessingFailed() = 0;

    /**
     * Send unmount complete notification to Vehicle Power Daemon.
     */
    virtual void unmountComplete() = 0;

    /**
     * Subscribe for VPS specific events from VehiclePowerDaemon.
     * @param vpsCallback This object will get events when they occur.
     */
    virtual void subscribeVps(IVpsCallback* vpsCallback) = 0;

    /**
     * @brief vpsReady - notify that VPS is ready to take over responsibility
     * from VPD.
     */
    virtual void vpsReady() = 0;

    /**
     * Send end early HMI notification to Boot Daemon via VPD
     */
    virtual void endEarlyHmi() = 0;

    /**
     * Send forced completion of Early Services notification to Boot Daemon via VPD
     */
    virtual void forcedCompletionEarlyServices() = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPSCLIENT_H_
