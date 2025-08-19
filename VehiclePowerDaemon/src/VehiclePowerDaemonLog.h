/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

// LCOV_EXCL_START
// This is excluded from a unit test coverage report because this needed solely for logging purposes
// and has no logic.

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VEHICLEPOWERDAEMONLOG_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VEHICLEPOWERDAEMONLOG_H_

#include "Log.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using com::mitsubishielectric::ahu::common::MLOGD;
using com::mitsubishielectric::ahu::common::MLOGE;
using com::mitsubishielectric::ahu::common::MLOGF;
using com::mitsubishielectric::ahu::common::MLOGI;
using com::mitsubishielectric::ahu::common::MLOGV;
using com::mitsubishielectric::ahu::common::MLOGW;

namespace LogFuncId {
const auto VPD = com::mitsubishielectric::ahu::common::FunctionID::vehicle_pwr_d;
}

enum LogId {
    VpdStateMachine_transitionTo_State_machine_is_going_to_empty_state = 1,
    VpdStateMachine_transitionTo_Transition,
    VpdStateMachine_onSubscribeNative_id_state,
    VpdStateMachine_onSubscribeVps_id_state,
    VpdStateMachine_onStartCompleteCpuCom_state,
    VpdStateMachine_onStartCompleteVps_state,
    VpdStateMachine_onStopCompleteNative_id_state,
    VpdStateMachine_onStopCompleteLog_state,
    VpdStateMachine_onAppStopCompleteVps_state,
    VpdStateMachine_onFwStopCompleteVps_state,
    SubscriptionNotificationDecorator_onSubscribeVps_client_id,
    SubscriptionNotificationDecorator_onSubscribeNative_client_id,
    StateHolder_ctor_create_instance_Default_state,
    StateHolder_getVpdState_state,
    StateHolder_setVpdState_VPD_state_change_from_to,
    StateSender_sendState_socket_id_state,
    StateSender_sendState_incoming_VPD_state_is_undefined_VPD_state,
    ClientActions_notifySystemStateChange_client_id_state,
    VehicleConfigProvider_ctor_Cant_read_javaTimeout_parameter,
    VehicleConfigProvider_ctor_Cant_read_logdogTimeout_parameter,
    VehicleConfigProvider_ctor_Cant_read_nativeTimeout_parameter,
    VehicleConfigProvider_ctor_Cant_read_appResumeTimeout_parameter,
    VehicleConfigProvider_ctor_Cant_read_logSaveTimeout_parameter,
    VehicleConfigProvider_getInitString_Cant_read_nativeInitConfig_parameter,
    WaitForAppStopVps_onEnter,
    WaitForAppStopVps_onTimeout,
    WaitForAppStopVps_onAppStopCompleteVps,
    WaitForAppStopVps_Timer_Is_Expired,
    WaitForAppStopVps_coldboot_requested,
    WaitForFwStopVps_onEnter,
    WaitForFwStopVps_onTimeout,
    WaitForFwStopVps_onFwStopCompleteVps,
    WaitForFwStopVps_Timer_Is_Expired,
    WaitForFwStopVps_onStopFailedVps,
    WaitForVpsStart_onEnter,
    WaitForVpsStart_onSubscribeVps,
    WaitForVpsStartComplete_onEnter,
    WaitForVpsStartComplete_onStartCompleteVps,
    NotifyHelper_notifyAllNative_client_id_state,
    NotifyRestart_onEnter,
    NotifyRestart_onExit,
    WaitNativeAppStopProcessingComplete_onEnter_Number_of_subscribers,
    WaitNativeAppStopProcessingComplete_onTimeout,
    WaitNativeAppStopProcessingComplete_onStopCompleteNative_Number_of_stopped_subscribers,
    WaitForLogDaemonStop_onEnter,
    WaitForLogDaemonStop_onTimeout,
    WaitForLogDaemonStop_onStopCompleteLog,
    LogDaemonBusy_onEnter,
    LogDaemonBusy_onTimeout,
    LogDaemonBusy_onTimeout_mLogdogCheckSavingStateTimer_is_expired,
    LogDaemonBusy_onTimeout_mLogdogSaveStateTimer_is_expired,
    WaitForStartCpuCom_onEnter,
    WaitForStartCpuCom_onStartCompleteCpuCom,
    InitialState_onTimeout,
    Stop_onEnter,
    NativeRunnerManager_run_Cant_get_command_from_line,
    NativeRunnerManager_run_Cant_get_command_result_from_line,
    StartNativeCommandResult_isDone_service_state,
    StartNativeCommand_execute_Cant_set_property_to_value,
    NativeRunnerManagerSequential_run_Config_is_empty,
    NativeRunnerManagerSequential_run_Cant_get_command_from_line,
    NativeRunnerManagerSequential_run_List_with_commands_to_execute_is_empty,
    NativeRunnerManagerSequential_executeCmd_Run_command,
    NativeRunnerManagerSequential_executeCmd_Cant_get_command_result_from_line,
    ConfigScriptCmdCreator_createCommand,
    TimeoutHandler_Process_Wrong_event_index,
    LogdogStopCompleteHandler_Process_Wrong_event_index,
    VpsSubscriptionHandler_Process_Wrong_event_index,
    UnsubscriptionHandlerLogdog_Process_Wrong_event_index,
    UnsubscriptionHandler_Process_Wrong_event_index,
    DisconnectHandler_Process_Wrong_event_index,
    SubscriptionHandler_Process_Wrong_event_index,
    EventHandling_run_mEventQueueHandler_is_nullptr,
    EventHandling_stop_mEventQueueHandler_is_nullptr,
    CpuStartHandler_Process_Wrong_event_index,
    SubscriptionHandlerLogdog_Process_Wrong_event_index,
    VpsUnsubscriptionHandler_Process_Wrong_event_index,
    VpsStopCompleteHandler_Process_Wrong_event_index,
    VpsStartHandler_Process_Wrong_event_index,
    EventQueueHandler_run_No_event_handler_found_for_event_index,
    EventQueueHandler_run_Handler_for_event_is_null,
    EventQueueHandler_setEventHandler_Event_handler_for_event_index_already_exists,
    NativeStopCompleteHandler_Process_Wrong_event_index,
    CommandParser_onReceiveCommand_unsupported_command_received,
    CommandParser_onError_command_received_with_error_code,
    CpuComClient_start_CpuCom_is_not_initialized,
    CpuComClient_stop_CpuCom_is_not_initialized,
    CpuComClient_sendAdtrRequest_CpuCom_is_not_initialized,
    CpuComClient_sendStartComplete_CpuCom_is_not_initialized,
    CpuComClient_sendShutdownCancelAccept_CpuCom_is_not_initialized,
    CpuComClient_sendShutdownProcessingStart_CpuCom_is_not_initialized,
    CpuComClient_sendShutdownCancelComplete_is_not_initialized,
    CpuComClient_sendWdtrRequest_CpuCom_is_not_initialized,
    CpuComClient_dtor_CpuCom_is_not_initialized,
    SubscriptionManager_add_Adding_new_subscriber_client_id,
    SubscriptionManager_add_Subscription_Notification_is_not_set,
    SubscriptionManager_remove_Removing_subscriber_client_id,
    SubscriptionManager_setVpsID_Set_VPS_id_to,
    SubscriptionManager_setVpsID_VPS_subscription_was_already_set,
    SubscriptionManager_setVpsID_Subscription_Notification_is_not_set,
    SubscriptionManager_setLogID_Logdog_subscription_was_already_set,
    SubscriptionManager_setLogID_Bootdaemon_subscription_was_already_set,
    SubscriptionManager_disconnect_Subscriber_has_disconnected_client_id,
    WakeUpEventDetector_cannotOpenNode,
    WakeUpEventDetector_pollError,
    WakeUpEventDetector_valueFromNode,
    WakeUpEventDetector_wakeUpEventDetected,
    WakeUpEventDetector_pollError_retryExceeded,
    LogDaemonActions_isLogSavingComplete,
    LogDaemonActions_isLogSavingComplete_mLogDog_instantiated,
    LogDaemonActions_isLogSavingComplete_LogDogs_log_saving_state_is,
    LogDaemonActions_serviceDied_Logdog_service_died,
    LogDaemonActions_isLogSavingComplete_service_unavailable,
    ShutdownManager_performShutdownAction,
    ShutdownManager_performShutdownAction_Undefined_shutdown_action,
    AbstractResetTimerHandler_start_mTimer_is_nullptr,
    AbstractResetTimerHandler_onTimeout_mTimer_is_nullptr,
    AbnormalTimerHandler_resetRequestSent,
    WatchDogTimerHandler_resetRequestSent,
    WakeUpManager_wakeUp,
    ParamByteEqualCondition_ctor_Cant_read_Byte_parameter_from_config,
    WakeLockManager_acquireWakeLock,
    WakeLockManager_acquireWakeLock_Wake_lock_acquired,
    WakeLockManager_acquireWakeLock_Cant_aquire_wakelock_error_code,
    WakeLockManager_releaseWakeLock,
    WakeLockManager_releaseWakeLock_Wake_lock_released,
    WakeLockManager_releaseWakeLock_Cant_release_wakelock_error_code,
    WakeLockManager_acquireWakeLock_Cant_aquire_wakelock_errno,
    WakeLockManager_releaseWakeLock_Cant_release_wakelock_errno,
    WakeLockManager_enableWakeLocks,
    WakeLockManager_enableWakeLocks_Cant_enable_wakelocks,
    WakeLockManager_disableWakeLocks,
    WakeLockManager_disableWakeLocks_Cant_disable_wakelocks,
    VpmBridgeDaemonAdapter_Cannot_acquire_VpmBridgeDaemon_instance,
    VpmBridgeDaemonAdapter_Request_to_VpmBridgeDaemon,
    WaitForModulesUnload_onTimeout_timeout,
    WaitForModulesUnload_onTimeout_success,
    WaitForModulesLoadBeforeUnload_onTimeout_timeout,
    WaitForModulesLoadBeforeUnload_onTimeout_success,
    WaitNativeAppStopProcessingComplete_Timer_Is_Expired,
    VpdStateMachine_onStopFailedVps_state,
    CpuComClient_sendSelfRebootTrue_CpuCom_is_not_initialized,
    StartNativeCmdConditional_execute_Condition_is_not_satisfied_cmd_not_executed,
    ParamByteEqualCondition_ctor_Condition_is_not_satisfied_expected_actual,
    CommandParser_onReceiveCommand_cmd_subcmd,
    CpuComClient_start,
    CpuComClient_stop,
    CpuComClient_sendStartComplete,
    CpuComClient_sendShutdownCancelAccept,
    CpuComClient_sendShutdownProcessingStart,
    CpuComClient_sendShutdownCancelComplete,
    CpuComClient_sendSelfRebootTrue,
    VpdStateMachine_onPowerStateChange_state,
    PowerStateHolder_onPowerStateChange,
    VcpuPowerStateChangeHandler_Invalid_Vpd_state,
    WaitForVpsStartComplete_onStartFailedVps,
    VpdStateMachine_onStartFailedVps_state,
    InitialState_onTimeout_vpdRestarted,
    NotifyHelper_noNativeSubscribers,
    NotifyHelper_vpsNotSubscribed,
    NotifyHelper_logdogNotSubscribed,
    ParamStringEqualCondition_ctor_Cant_read_String_parameter_from_config,
    ParamStringEqualCondition_ctor_Condition_is_not_satisfied_expected_actual,
    VpsFwResumeCompleteHandler_Wrong_event_index,
    VpdStateMachine_onFwResumeComplete_state,
    ResumeNative_onEnter,
    WaitForFwResumeComplete_onEnter,
    WaitForFwResumeComplete_onFwResumeComplete,
    WaitForFwResumeComplete_onTimeout,
    WaitForFwResumeComplete_timerIsExpired,
    WaitForFwResumeComplete_onExit,
    VpsFwRestartCompleteHandler_Wrong_event_index,
    VpdStateMachine_onFwRestartComplete_state,
    RestartNative_onEnter,
    WaitForFwRestartComplete_onEnter,
    WaitForFwRestartComplete_onFwRestartComplete,
    CpuComClient_sendMainStartComplete,
    CpuComClient_sendMainStartComplete_CpuCom_is_not_initialized,
    VehicleConfigProvider_ctor_Cant_read_appResumeAppTimeout_parameter,

    WaitForAppResumeComplete_onEnter,
    WaitForAppResumeComplete_onAppResumeComplete,
    WaitForAppResumeComplete_onTimeout,
    WaitForAppResumeComplete_timerIsExpired,
    WaitForAppResumeComplete_onExit,

    WaitForFwRestartComplete_onTimeout,
    WaitForFwRestartComplete_timerIsExpired,

    WaitForAppRestartComplete_onEnter,
    WaitForAppRestartComplete_onExit,
    WaitForAppRestartComplete_onAppRestartComplete,
    WaitForAppRestartComplete_onTimeout,
    WaitForAppRestartComplete_timerIsExpired,

    VpsAppResumeCompleteHandler_Wrong_event_index,
    VpsAppRestartCompleteHandler_Wrong_event_index,

    VpdStateMachine_onAppResumeComplete_state,
    VpdStateMachine_onAppRestartComplete_state,
    AndroidInit_propertySet_Setting_property,

    VpdMessageServer_start_failed,
    VpdMessageServer_unknown_pid,
    WaitNativeAppStopProcessingComplete_onTimeout_NoStopCompleteFrom,

    HmiEventsHandler_Process_Wrong_event_index,
    BootDaemonEventHandler_processBootDaemonEvent_Wrong_event_index,
    BootDaemonEventHandler_Process_Wrong_ClientId,
    HmiEventsHandler_processVpsEvent_Wrong_event_index,
    VpdStateMachine_onVpsUnmountComplete_state,
    VpsMountHandler_Process_Wrong_event_index,
    WaitForUnmount_onEnter,
    WaitForUnmount_onVpsUnmountComplete,
    WaitForUnmount_onTimeout,
    WaitForUnmount_timerIsExpired,
    WaitForMount_onEnter,

    WaitForLogSaving_OnTimeout,
    WaitForLogSaving_Timer_Is_Expired,

    WaitBeforeSendingFwStopVps_onTimeout,
    WaitBeforeSendingFwStopVps_Timer_Is_Expired,

    WaitBeforeSendingAppStopNativeServices_onTimeout,
    WaitBeforeSendingAppStopNativeServices_Timer_Is_Expired,

    BootDaemonEventHandler_Process,
    HmiEventsHandler_processVpsEvent_sendNotification,
    ShutdownProcessingStartSentHandler_Process_Wrong_event_index,
    VpdStateMachine_onShutdownProcessingStartSent_state,
    ShutdownProcessingStartStatusReceiver_delivered_false,

    ResumeProcessingStartCompleteSentHandler_Process_Wrong_event_index,
    VpdStateMachine_onResumeProcessingStartCompleteSentSuccess_state,
    VpdStateMachine_onResumeProcessingStartCompleteSentFailure_state,
    VpdStateMachine_onWakeUp_state,

    TimerManager_changeTimer,
    TimerManager_onTimeout_skipEvent,

    SystemPowerStateListener_onPowerStateChange_sendShutdownCancelAccept,

    VpdStateMachine_onDisconnectVps_state,

};

void InitializeVehiclePowerDaemonLogMessages();
void TerminateVehiclePowerDaemonLogMessages();

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

// LCOV_EXCL_STOP

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VEHICLEPOWERDAEMONLOG_H_
