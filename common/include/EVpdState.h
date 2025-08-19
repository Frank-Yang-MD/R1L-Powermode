/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVPDSTATE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVPDSTATE_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace common {

enum class EVpdState {
    APP_START_NA,
    APP_START_COLDBOOT,
    APP_STOP,
    APP_RESUME,
    APP_RESTART,
};

}  // namespace common
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVPDSTATE_H_
