/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVPDNOTIFICATIONS_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVPDNOTIFICATIONS_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace common {

enum class EVpdNotifications {
    // DEFAULT,
    AppStartNa,
    AppStartColdboot,
    AppStop,
    AppResume,
    AppRestart,
    FwResume,
    FwRestart,
    FwStop,
    TimeOutError,
    HmiEventVpsReady,
    RequestUnmount,
    RequestMount,
    HandOffResponsibility,
    HandOffResponsibilityStartupAnimationDisplayed,
    HandOffResponsibilityStartupAnimationAndDiDisplayed,
    EndEarlyHmi,
    ForcedCompletionEarlyServices,
};

}  // namespace common
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVPDNOTIFICATIONS_H_
