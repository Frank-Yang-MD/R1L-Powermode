/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IPOWERSTATECHANGELISTENER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IPOWERSTATECHANGELISTENER_H_

#include "PowerState.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * @brief The IPowerStateChangeListener class is intentended to listen for
 * PowerState changes from VCPU
 */
class IPowerStateChangeListener {
public:
    // LCOV_EXCL_START
    virtual ~IPowerStateChangeListener() = default;
    // LCOV_EXCL_STOP

    /**
     * @brief onPowerStateChange - process new PowerState
     */
    virtual void onPowerStateChange(PowerState) = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IPOWERSTATECHANGELISTENER_H_
