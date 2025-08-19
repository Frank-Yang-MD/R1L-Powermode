/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_VPDEVENTS_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_VPDEVENTS_H_

#include "EVpdEvent.h"

#include <vector>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

const std::vector<common : EVpdEvent> ALL_VPD_EVENTS = {
    Unsupported = 0x00,
    SubscribeNative,
    UnsubscribeNative,
    StopCompleteNative,
    CpuComStarted,
    SubscribeVps,
    UnsubscribeVps,
    VpsStarted,
    StartFailedVps,
    AppStopCompleteVps,
    FwResumeCompleteVps,
    FwRestartCompleteVps,
    AppResumeCompleteVps,
    AppRestartCompleteVps,
    FwStopCompleteVps,
    StopFailedVps,
    VpsReady,
    FullHmiDisplaying,
    SubscribeBootDaemon,
    UnsubscribeBootDaemon,
    handOffResponsibility,
    handOffResponsibilityStartupAnimationDisplayed,
    handOffResponsibilityDiDisplayed,
    handOffResponsibilityStartupAnimationAndDiDisplayed,
    SubscribeLogdog,
    UnsubscribeLogdog,
    StopCompleteLogdog,
    Timeout,
    Disconnect,
    Shutdown,
    Suspend,
    Reboot,
    RebootRecovery,
    ShutdownCancel,
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_VPDEVENTS_H_
