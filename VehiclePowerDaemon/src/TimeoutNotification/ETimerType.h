/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ETIMERTYPE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ETIMERTYPE_H_

#include <cstdint>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Timer handler enum
 */
enum ETimerType : uint8_t {
    None = 0,
    ADTRTimer,
    WDTRTimer,
};

const char* toString(const ETimerType timerType);

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ETIMERTYPE_H_
