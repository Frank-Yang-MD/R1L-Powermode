/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IBOOTDAEMONCALLBACK_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IBOOTDAEMONCALLBACK_H

#include "EHmiEvent.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

/**
 * Callback for notifying BootDaemon on HMI events.
 */
class IBootDaemonCallback {
public:
    virtual ~IBootDaemonCallback() = default;

    /**
     * Notify about HMI events from Vehicle Power Daemon.
     * @param event HMI event from Vehicle Power Daemon.
     */
    virtual void onHMIEvent(common::EHmiEvent event) = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IBOOTDAEMONCALLBACK_H
