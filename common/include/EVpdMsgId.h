/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_COMMON_EVPDMSGID_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_COMMON_EVPDMSGID_H_

#include <cstdint>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace common {

/**
 * EVpdMsgId
 */
enum class EVpdMsgId : uint8_t { Notification, Event };

}  // namespace common
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_COMMON_EVPDMSGID_H_
