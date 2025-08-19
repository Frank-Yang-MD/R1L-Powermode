/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTATEHOLDER_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTATEHOLDER_H

#include "EVpdState.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Hold vpd current state to notify new native subscribers
 */
class IStateHolder {
public:
    // LCOV_EXCL_START
    virtual ~IStateHolder() = default;
    // LCOV_EXCL_STOP
    /**
     * Get current vpd state
     */
    virtual common::EVpdState getVpdState() const = 0;
    /**
     * Set new vpd state
     */
    virtual void setVpdState(common::EVpdState vpdState) = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTATEHOLDER_H
