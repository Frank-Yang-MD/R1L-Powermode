/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_INATIVECLIENT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_INATIVECLIENT_H_

#include "INativeCallback.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class INativeClient {
public:
    virtual ~INativeClient() = default;

    /**
     * Obtain current state of Vehicle Power Daemon.
     * @return Current state of Vehicle Power Daemon.
     */
    virtual common::EVpdState GetVPDState() = 0;
    /**
     * Send native services/apps stop complete notification to Vehicle Power Daemon.
     */
    virtual void stopProcessingComplete() = 0;
    /**
     * Subscribe for events of a state change from Vehicle Power Daemon.
     * @param callback This object will get events when they occur.
     */
    virtual void subscribe(INativeCallback* callback) = 0;
    /**
     * Unsubscribe from events of a state change from Vehicle Power Daemon.
     */
    virtual void unsubscribe() = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_INATIVECLIENT_H_
