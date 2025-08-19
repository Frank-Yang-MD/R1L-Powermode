/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EHMIEVENT_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EHMIEVENT_H

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace common {

enum class EHmiEvent {
    HmiEventVpsReady,
    EndEarlyHmi,
    ForcedCompletionEarlyServices,
};

}  // namespace common
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EHMIEVENT_H
