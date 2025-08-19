/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef VEHICLEPWRMGR_JNI_LOG_H
#define VEHICLEPWRMGR_JNI_LOG_H

#include <Log.h>

using com::mitsubishielectric::ahu::common::MLOGD;
using com::mitsubishielectric::ahu::common::MLOGE;
using com::mitsubishielectric::ahu::common::MLOGF;
using com::mitsubishielectric::ahu::common::MLOGI;
using com::mitsubishielectric::ahu::common::MLOGV;
using com::mitsubishielectric::ahu::common::MLOGW;

namespace LogFuncId {
const auto VPS = com::mitsubishielectric::ahu::common::FunctionID::vehicle_pwr_svc;
}

enum LogId {
    NativeCallback_onSystemStateChange_System_state_changed = 1,
    vehicle_power_service_initNative,
    vehicle_power_service_initNative_vpsClient_is_null,
    vehicle_power_service_cleanupNative,
    vehicle_power_service_cleanupNative_vpsClient_is_null,
    vehicle_power_service_register_vehicle_power_service_jni,
    vehicle_power_service_JNI_OnLoad,
    vehicle_power_service_JNI_OnLoad_JNI_version_missmatch_error,
    vehicle_power_service_JNI_OnLoad_vehicle_power_service_jni_registration_failure,
    vehicle_power_service_JNI_OnLoad_vpd_native_adapter_jni_registration_failure,
    vehicle_power_service_JNI_OnLoad_vpd_native_receiver_jni_registration_failure,
    vpd_native_adapter_startProcessingCompleteNative,
    vpd_native_adapter_startProcessingCompleteNative_vpsClient_is_null,
    vpd_native_adapter_appStopProcessingCompleteNative,
    vpd_native_adapter_appStopProcessingCompleteNative_vpsClient_is_null,
    vpd_native_receiver_AttachCurrentThread_failed,
    vpd_native_receiver_onVpdStateChange_VPD_state_changed,
    vpd_native_receiver_classInitNative,
    vpd_native_receiver_initNative,
    vpd_native_adapter_stopProcessingFailedNative,
    vpd_native_adapter_stopProcessingFailedNative_vpsClient_is_null,
    vpd_native_adapter_startProcessingFailedNative,
    vpd_native_adapter_startProcessingFailedNative_vpsClient_is_null,
    vpd_native_adapter_fwRestartProcessingCompleteNative,
    vpd_native_adapter_fwRestartProcessingCompleteNative_vpsClient_is_null,
    vpd_native_adapter_fwResumeProcessingCompleteNative,
    vpd_native_adapter_fwResumeProcessingCompleteNative_vpsClient_is_null,
    vpd_native_adapter_fwStopProcessingCompleteNative,
    vpd_native_adapter_fwStopProcessingCompleteNative_vpsClient_is_null,
    vpd_native_receiver_onFwRestart,
    vpd_native_receiver_onFwResume,
    vpd_native_receiver_onFwStop,
    vehicle_power_service_JNI_OnLoad_bootd_native_adapter_jni_registration_failure,
    vehicle_power_service_JNI_OnLoad_bootd_native_receiver_jni_registration_failure,
    bootd_native_adapter_hmiIsReadyNative,
    bootd_native_adapter_hmiIsReadyNative_bootdClient_is_null,
    bootd_native_receiver_classInitNative,
    bootd_native_receiver_initNative,

    vpd_native_receiver_onTimeOutError,

    vpd_native_adapter_appRestartProcessingCompleteNative,
    vpd_native_adapter_appRestartProcessingCompleteNative_vpsClient_is_null,
    vpd_native_adapter_appResumeProcessingCompleteNative,
    vpd_native_adapter_appResumeProcessingCompleteNative_vpsClient_is_null,

    vpd_native_receiver_requestMount,
    vpd_native_receiver_requestMount_AttachCurrentThread_failed,
    vpd_native_receiver_requestUnmount,
    vpd_native_receiver_requestUnmount_AttachCurrentThread_failed,
    vpd_native_adapter_unmountCompleteNative,
    vpd_native_adapter_unmountCompleteNative_vpsClient_is_null,
    vpd_native_adapter_endEarlyHmiNative,
    vpd_native_adapter_endEarlyHmiNative_vpsClient_is_null,
    vpd_native_adapter_forcedCompletionEarlyServicesNative,
    vpd_native_adapter_forcedCompletionEarlyServicesNative_vpsClient_is_null,

    bootd_native_receiver_handOffResponsibility,
    bootd_native_receiver_handOffResponsibility_AttachCurrentThread_failed,
    bootd_native_receiver_handOffResponsibilityStartupAnimationDisplayed,
    bootd_native_receiver_handOffResponsibilityStartupAnimationDisplayed_AttachCurrentThread_failed,
    bootd_native_receiver_handOffResponsibilityStartupAnimationAndDiDisplayed,
    bootd_native_receiver_handOffResponsibilityStartupAnimationAndDiDisplayed_AttachCurrentThread_failed,

    vpd_native_receiver_onVpdStateChange_AttachCurrentThread_failed,
    vpd_native_receiver_onFwRestart_AttachCurrentThread_failed,
    vpd_native_receiver_onFwResume_AttachCurrentThread_failed,
    vpd_native_receiver_onFwStop_AttachCurrentThread_failed,
    vpd_native_receiver_onTimeOutError_AttachCurrentThread_failed,
};

void InitializeVpsJNILogMessages();
void TerminateVpsJNILogMessages();

#endif  // VEHICLEPWRMGR_JNI_LOG_H
