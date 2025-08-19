/* COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED */

// LCOV_EXCL_START
// This is excluded from a unit test coverage report because this needed solely for logging purposes
// and has no logic.

#include "LibVehiclePwrLog.h"
#include "Log.h"
#include <unistd.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

void InitializeLibVehiclePwrLogMessages()
{
    using com::mitsubishielectric::ahu::common::LogMessageFormats;
    using common::DisplayTypeDecInt32;
    const LogMessageFormats libvehiclepwrLogMessages = {
        {LogId::CpuComClient_CpuComStarted_common_EVpdEvent_CpuComStarted_event_sent,
         "common::EVpdEvent::CpuComStarted event sent"},
        {LogId::LogdogClient_stopProcessingComplete_StopCompleteLogdog_event_sent,
         "LogdogClient::stopProcessingComplete: StopCompleteLogdog event sent"},
        {LogId::LogdogClient_subscribe_SubscribeLogdog_event_sent,
         "LogdogClient::subscribe: SubscribeLogdog event sent"},
        {LogId::LogdogClient_unsubscribe_UnsubscribeLogdog_event_sent,
         "LogdogClient::unsubscribe: UnsubscribeLogdog event sent"},
        {LogId::LogdogClient_onConnect_SubscribeLogdog_event_sent,
         "LogdogClient::onConnect: SubscribeLogdog event sent"},
        {LogId::NativeClient_onConnect_func,
         "NativeClient::onConnect. Client PID: %d",
         {DisplayTypeDecInt32("Client PID")}},
        {LogId::NativeClient_onConnect_SubscribeNative_event_sent,
         "NativeClient::onConnect: SubscribeNative event sent. Client PID: %d",
         {DisplayTypeDecInt32("Client PID")}},
        {LogId::NativeClient_stopProcessingComplete_StopCompleteNative_event_sent,
         "NativeClient::stopProcessingComplete: StopCompleteNative event sent. Client PID: %d",
         {DisplayTypeDecInt32("Client PID")}},
        {LogId::NativeClientNotificationHandler_onVpdNotification_callback_is_null,
         "[%d]NativeClientNotificationHandler::onVpdNotification: Callback is null!",
         {DisplayTypeDecInt32("pid")}},
        {LogId::NativeClientNotificationHandler_onVpdNotification_invalidNotification,
         "NativeClientNotificationHandler::onVpdNotification: Invalid notification received: %d",
         {DisplayTypeDecInt32("vpdNotification")}},
        {LogId::NotificationReceiver_onConnect_Native_callback_is_not_set,
         "NotificationReceiver::onConnect: Native callback is not set"},
        {LogId::NotificationReceiver_onDisconnect_Native_callback_is_not_set,
         "NotificationReceiver::onDisconnect: Native callback is not set"},
        {LogId::NotificationReceiver_onNotificationReceivedInternal_not_processed,
         "[%d]NotificationReceiver::onNotificationReceivedInternal: Notification %d from VPD has "
         "been skipped",
         {DisplayTypeDecInt32{"pid"}, DisplayTypeDecInt32{"vpdNotification"}}},
        {LogId::VpsClient_subscribeVps_SubscribeVps_event_sent,
         "[%d]VpsClient::subscribeVps",
         {DisplayTypeDecInt32("pid")}},
        {LogId::VpsClient_startProcessingComplete_VpsStarted_event_sent,
         "VpsClient::startProcessingComplete: VpsStarted event sent"},
        {LogId::VpsClient_appStopProcessingComplete_AppStopCompleteVps_event_sent,
         "VpsClient::appStopProcessingComplete: AppStopCompleteVps event sent"},
        {LogId::VpsClient_fwStopProcessingComplete_FwStopCompleteVps_event_sent,
         "VpsClient::fwStopProcessingComplete: FwStopCompleteVps event sent"},
        {LogId::VpsClient_unsubscribe_UnsubscribeVps_event_sent,
         "VpsClient::unsubscribe: UnsubscribeVps event sent"},
        {LogId::VpsClient_onConnect_SubscribeVps_event_sent,
         "VpsClient::onConnect: SubscribeVps event sent"},
        {LogId::VpsClient_stopProcessingFailed_StopFailedVps_event_sent,
         "VpsClient::stopProcessingFailed: StopFailedVps event sent"},
        {LogId::VpsClient_startProcessingFailed_StartFailedVps_event_sent,
         "VpsClient::startProcessingFailed: StartFailedVps event sent"},
        {LogId::NativeClient_subscribeClientPid,
         "[%d]NativeClient::subscribe",
         {DisplayTypeDecInt32("pid")}},
        {LogId::NativeClient_unsubscribeClientPid,
         "NativeClient::unsubscribe: Client PID: %d",
         {DisplayTypeDecInt32("Client PID")}},
        {LogId::InitializeLibVehiclePwrLogMessages_InitMessage,
         "VPL. InitializeLibVehiclePwrLogMessages: Client PID: %d",
         {DisplayTypeDecInt32("Client PID")}},
        {LogId::VpsClient_fwRestartProcessingComplete_FwRestartCompleteVps_event_sent,
         "VpsClient::fwRestartProcessingComplete: FwRestartCompleteVps event sent"},
        {LogId::VpsClient_fwResumeProcessingComplete_FwResumeCompleteVps_event_sent,
         "VpsClient::fwResumeProcessingComplete: FwResumeCompleteVps event sent"},
        {LogId::VpsClient_appResumeProcessingComplete_AppResumeCompleteVps_event_sent,
         "VpsClient::appResumeProcessingComplete: AppResumeCompleteVps event sent"},
        {LogId::VpsClient_appRestartProcessingComplete_AppRestartCompleteVps_event_sent,
         "VpsClient::appRestartProcessingComplete: AppRestartCompleteVps event sent"},
        {LogId::VpdMessageClient_could_not_connect,
         "VpdMessageClient::connect: Could not connect to message server!"},
        {LogId::BootDaemonClient_subscribeClientPid,
         "[%d]BootDaemonClient::subscribe",
         {DisplayTypeDecInt32("pid")}},
        {LogId::BootDaemonClient_unsubscribeClientPid,
         "BootDaemonClient::unsubscribe: Client PID: %d",
         {DisplayTypeDecInt32("Client PID")}},
        {LogId::BootDaemonClient_handOffResponsibility,
         "BootDaemonClient::handOffResponsibility event is sent"},
        {LogId::BootDaemonClient_handOffResponsibilityStartupAnimationDisplayed,
         "BootDaemonClient::handOffResponsibilityStartupAnimationDisplayed event is sent"},
        {LogId::BootDaemonClient_handOffResponsibilityStartupAnimationAndDiDisplayed,
         "BootDaemonClient::handOffResponsibilityStartupAnimationAndDiDisplayed event is sent"},
        {LogId::BootDaemonClient_onConnect_SubscribeBootDaemon_event_sent,
         "BootDaemonClient::onConnect: SubscribeBootDaemon event sent. Client PID: %d",
         {DisplayTypeDecInt32("Client PID")}},
        {LogId::BootdNotificationHandler_callback_is_null,
         "BootdNotificationHandler::onVpdNotification: Callback is null!"},
        {LogId::VpsClient_vpsReady_VpsReady_event_sent, "VpsClient::vpsReady: VpsReady event sent"},
        {LogId::VpsClient_endEarlyHmi_EndEarlyHmi_event_sent,
         "VpsClient::endEarlyHmi: EndEarlyHmi event sent"},
        {LogId::VpsClient_forcedCompletionEarlyServices_ForcedCompletionEarlyServices_event_sent,
         "VpsClient::forcedCompletionEarlyServices: ForcedCompletionEarlyServices event sent"},
        {LogId::VpsClient_unmountComplete_UnmountComplete_event_sent,
         "VpsClient::unmountComplete: UnmountComplete event sent"},
        {LogId::VpsNotificationHandler_HandOff_Called,
         "VpsNotificationHandler.VpsNotificationHandler::onVpdNotification: handoff '%s'"},
        {LogId::BootdNotificationHandler_onVpdNotification_Event_Sent,
         "BootDaemonClient.BootdNotificationHandler::onVpdNotification: send event '%s'"},
        {LogId::VpsNotificationHandler_onVpdNotification_callback_is_null,
         "VpsNotificationHandler::onVpdNotification: Callback is null!"},

        {LogId::NotificationReceiver_onNotificationReceived,
         "[%d]NotificationReceiver::onNotificationReceived: Received notification %d from VPD",
         {DisplayTypeDecInt32{"pid"}, DisplayTypeDecInt32{"vpdNotification"}}},
        {LogId::NotificationReceiver_onNotificationReceivedInternal,
         "[%d]NotificationReceiver::onNotificationReceivedInternal: Internal %d, received "
         "notification %d from VPD",
         {DisplayTypeDecInt32{"pid"}, DisplayTypeDecInt32{"internal"},
          DisplayTypeDecInt32{"vpdNotification"}}},
        {LogId::NotificationReceiver_onNotificationReceivedInternal_processed,
         "[%d]NotificationReceiver::onNotificationReceivedInternal: Processed notification %d "
         "from VPD",
         {DisplayTypeDecInt32{"pid"}, DisplayTypeDecInt32{"vpdNotification"}}},
        {LogId::NotificationReceiver_processReceivedNotifications,
         "[%d]NotificationReceiver::processReceivedNotifications: Processing skipped notifications",
         {DisplayTypeDecInt32{"pid"}}},

        {LogId::BootDaemonClient_subscribeClientPid_done,
         "[%d]BootDaemonClient::subscribe: done!",
         {DisplayTypeDecInt32("pid")}},
        {LogId::NativeClient_subscribeClientPid_done,
         "[%d]NativeClient::subscribe: done!",
         {DisplayTypeDecInt32("pid")}},
        {LogId::VpsClient_subscribeVps_SubscribeVps_event_sent_done,
         "[%d]VpsClient::subscribeVps: done!",
         {DisplayTypeDecInt32("pid")}},
    };
    using com::mitsubishielectric::ahu::common::InitializeFunctionID;
    InitializeFunctionID(LogFuncId::VPL, libvehiclepwrLogMessages);

#ifndef HOST
    MLOGI(LogFuncId::VPL, LogId::InitializeLibVehiclePwrLogMessages_InitMessage, getpid());
#endif
}

void TerminateLibVehiclePwrLogMessages()
{
    using com::mitsubishielectric::ahu::common::TerminateFunctionID;
    TerminateFunctionID(LogFuncId::VPL);
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
