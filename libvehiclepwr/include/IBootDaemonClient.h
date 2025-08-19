/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IBOOTDAEMONCLIENT_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IBOOTDAEMONCLIENT_H

#include "IBootDaemonCallback.h"
#include "INativeClient.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

/**
 * Interface for BootDaemon to subscribe for and send HMI events.
 */
class IBootDaemonClient : public virtual INativeClient {
public:
    virtual ~IBootDaemonClient() = default;

    /**
     * Subscribe for events from Vehicle Power Daemon.
     * @param callback This object will get events when they occur.
     */
    virtual void subscribeBootdHmiClient(IBootDaemonCallback* callback) = 0;

    /**
     * Send take over responsibility event to ISM to Vehicle Power Daemon.
     */
    virtual void handOffResponsibility() = 0;
    /**
     * Send take over responsibility to ISM with startup animation to Vehicle Power Daemon.
     */
    virtual void handOffResponsibilityStartupAnimationDisplayed() = 0;
    /**
     * Send take over responsibility to ISM with startup animation/DI to Vehicle Power Daemon.
     */
    virtual void handOffResponsibilityStartupAnimationAndDiDisplayed() = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IBOOTDAEMONCLIENT_H
