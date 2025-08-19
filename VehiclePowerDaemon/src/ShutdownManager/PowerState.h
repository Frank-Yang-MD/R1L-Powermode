/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_POWERSTATE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_POWERSTATE_H_

#include <cstdint>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * @brief The PowerState enum holds shutdown action to
 * be performed at some moment.
 */
enum class PowerState : uint8_t {
    NORMAL,
    SUSPEND_CANCEL,
    SUSPEND,
    SHUTDOWN,
    REBOOT,
    REBOOT_RECOVERY,
    WAIT_FOR_CPUCOM_DELIVERY
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_POWERSTATE_H_
