/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IPOWERSTATEHOLDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IPOWERSTATEHOLDER_H_

#include <IPowerStateChangeListener.h>
#include <ITimeoutNotification.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * @brief The IPowerStateHolder class represents the
 * holder of current power state.
 */
class IPowerStateHolder : public IPowerStateChangeListener {
public:
    // LCOV_EXCL_START
    virtual ~IPowerStateHolder() = default;
    // LCOV_EXCL_STOP

    /**
     * @brief isShutdownState - indicates if current PowerState
     * neither NORMAL nor SUSPEND_CANCEL
     */
    virtual bool isShutdownState() const = 0;

    /**
     * @brief getPowerState - get current power state
     * @return current power state
     */
    virtual PowerState getPowerState() const = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IPOWERSTATEHOLDER_H_
