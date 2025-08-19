/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_VPDNOTIFICATIONS_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_VPDNOTIFICATIONS_H_

#include "EVpdNotifications.h"

#include <vector>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

const std::vector<common::EVpdNotifications> ALL_VPD_NOTIFICATIONS = {
    common::EVpdNotifications::AppStartNa,
    common::EVpdNotifications::AppStartColdboot,
    common::EVpdNotifications::AppStop,
    common::EVpdNotifications::AppResume,
    common::EVpdNotifications::AppRestart,
    common::EVpdNotifications::FwResume,
    common::EVpdNotifications::FwRestart,
    common::EVpdNotifications::FwStop,
    common::EVpdNotifications::TimeOutError,
    common::EVpdNotifications::HmiEventVpsReady,
    common::EVpdNotifications::HandOffResponsibility,
    common::EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed,
    common::EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed,
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_VPDNOTIFICATIONS_H_
