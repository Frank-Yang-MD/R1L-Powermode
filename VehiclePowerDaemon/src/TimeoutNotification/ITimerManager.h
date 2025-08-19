/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ITIMERMANAGER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ITIMERMANAGER_H_

#include "ETimerType.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * This interface is responsible for storing and changing onTimeout event handlers
 */
class ITimerManager {
public:
    // LCOV_EXCL_START
    virtual ~ITimerManager() = default;
    // LCOV_EXCL_STOP

    /**
     * Call to change the current onTimeout handler
     */
    virtual void changeTimerHandler(ETimerType timer) = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ITIMERMANAGER_H_