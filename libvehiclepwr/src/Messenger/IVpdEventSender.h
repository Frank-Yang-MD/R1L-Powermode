/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPDEVENTSENDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPDEVENTSENDER_H_

#include "EVpdEvent.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class IVpdEventSender {
public:
    virtual ~IVpdEventSender() = default;

    /**
     * Send EVpdEvent to Vehicle Power Daemon.
     * @param inEVpdEvent EVpdEvent to be sent.
     */
    virtual void sendVpdEvent(common::EVpdEvent inEVpdEvent) = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPDEVENTSENDER_H_
