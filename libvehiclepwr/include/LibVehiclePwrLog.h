/* COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_LIBVEHICLEPWRLOG_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_LIBVEHICLEPWRLOG_H_

#include "Log.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

using com::mitsubishielectric::ahu::common::MLOGD;
using com::mitsubishielectric::ahu::common::MLOGE;
using com::mitsubishielectric::ahu::common::MLOGF;
using com::mitsubishielectric::ahu::common::MLOGI;
using com::mitsubishielectric::ahu::common::MLOGV;
using com::mitsubishielectric::ahu::common::MLOGW;

namespace LogFuncId {
const auto VPL = com::mitsubishielectric::ahu::common::FunctionID::vehicle_pwr_lib;
}

enum LogId {
    InitializeLibVehiclePwrLogMessages_InitMessage = 1,
    CpuComClient_CpuComStarted_common_EVpdEvent_CpuComStarted_event_sent,
    LogdogClient_stopProcessingComplete_StopCompleteLogdog_event_sent,
    LogdogClient_subscribe_SubscribeLogdog_event_sent,
    LogdogClient_unsubscribe_UnsubscribeLogdog_event_sent,
    LogdogClient_onConnect_SubscribeLogdog_event_sent,
    NativeClient_onConnect_func,
    NativeClient_onConnect_SubscribeNative_event_sent,
    NativeClient_stopProcessingComplete_StopCompleteNative_event_sent,
    NativeClientNotificationHandler_onVpdNotification_callback_is_null,
    NativeClientNotificationHandler_onVpdNotification_invalidNotification,
    NotificationReceiver_onConnect_Native_callback_is_not_set,
    NotificationReceiver_onDisconnect_Native_callback_is_not_set,
    NotificationReceiver_onNotificationReceivedInternal_not_processed,
    VpsClient_subscribeVps_SubscribeVps_event_sent,
    VpsClient_startProcessingComplete_VpsStarted_event_sent,
    VpsClient_appStopProcessingComplete_AppStopCompleteVps_event_sent,
    VpsClient_unsubscribe_UnsubscribeVps_event_sent,
    VpsClient_onConnect_SubscribeVps_event_sent,
    VpsClient_stopProcessingFailed_StopFailedVps_event_sent,
    VpsClient_startProcessingFailed_StartFailedVps_event_sent,
    VpsClient_fwStopProcessingComplete_FwStopCompleteVps_event_sent,
    NativeClient_subscribeClientPid,
    NativeClient_unsubscribeClientPid,
    VpsClient_fwRestartProcessingComplete_FwRestartCompleteVps_event_sent,
    VpsClient_fwResumeProcessingComplete_FwResumeCompleteVps_event_sent,
    VpsClient_appRestartProcessingComplete_AppRestartCompleteVps_event_sent,
    VpsClient_appResumeProcessingComplete_AppResumeCompleteVps_event_sent,
    VpsNotificationHandler_onVpdNotification_callback_is_null,
    VpdMessageClient_could_not_connect,
    BootDaemonClient_subscribeClientPid,
    BootDaemonClient_unsubscribeClientPid,
    BootDaemonClient_handOffResponsibility,
    BootDaemonClient_handOffResponsibilityStartupAnimationDisplayed,
    BootDaemonClient_handOffResponsibilityStartupAnimationAndDiDisplayed,
    BootDaemonClient_onConnect_SubscribeBootDaemon_event_sent,
    BootdNotificationHandler_callback_is_null,
    VpsClient_vpsReady_VpsReady_event_sent,
    VpsClient_endEarlyHmi_EndEarlyHmi_event_sent,
    VpsClient_forcedCompletionEarlyServices_ForcedCompletionEarlyServices_event_sent,
    VpsClient_unmountComplete_UnmountComplete_event_sent,
    VpsNotificationHandler_HandOff_Called,
    BootdNotificationHandler_onVpdNotification_Event_Sent,

    NotificationReceiver_onNotificationReceived,
    NotificationReceiver_onNotificationReceivedInternal,
    NotificationReceiver_onNotificationReceivedInternal_processed,
    NotificationReceiver_processReceivedNotifications,

    BootDaemonClient_subscribeClientPid_done,
    NativeClient_subscribeClientPid_done,
    VpsClient_subscribeVps_SubscribeVps_event_sent_done,
};

void InitializeLibVehiclePwrLogMessages();
void TerminateLibVehiclePwrLogMessages();

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_LIBVEHICLEPWRLOG_H_
