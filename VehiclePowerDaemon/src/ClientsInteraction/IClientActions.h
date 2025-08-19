/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICLIENTACTIONS_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICLIENTACTIONS_H_

#include "EVpdState.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IClientActions {
public:
    // LCOV_EXCL_START
    virtual ~IClientActions() = default;
    // LCOV_EXCL_STOP

    /**
     * Notify about system state change
     */
    virtual void notifySystemStateChange(int clientID, common::EVpdState state) = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICLIENTACTIONS_H_
