/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "vehiclepwrmgr_jni_log.h"

void InitializeVpsJNILogMessages()
{
    using namespace com::mitsubishielectric::ahu::common;

    const LogMessageFormats vpsJNILogMessages = {
        {NativeCallback_onSystemStateChange_System_state_changed,
         "JNI: NativeCallback::onSystemChangeState: System state changed to '%d'",
         {DisplayTypeDecInt32{"state"}}},
        {vehicle_power_service_initNative, "JNI: vehicle_power_service::initNative"},
        {vehicle_power_service_initNative_vpsClient_is_null,
         "JNI: vehicle_power_service::initNative: vpsClient is null"},
        {vehicle_power_service_cleanupNative, "JNI: vehicle_power_service::cleanupNative"},
        {vehicle_power_service_cleanupNative_vpsClient_is_null,
         "JNI: vehicle_power_service::cleanupNative: vpsClient is null"},
        {vehicle_power_service_register_vehicle_power_service_jni,
         "JNI: vehicle_power_service::register_vehicle_power_service_jni,"},
        {vehicle_power_service_JNI_OnLoad, "JNI: vehicle_power_service::JNI_OnLoad"},
        {vehicle_power_service_JNI_OnLoad_JNI_version_missmatch_error,
         "JNI: vehicle_power_service::JNI_OnLoad: JNI_version_missmatch_error"},
        {vehicle_power_service_JNI_OnLoad_vehicle_power_service_jni_registration_failure,
         "JNI: vehicle_power_service::JNI_OnLoad: vehicle_power_service_jni registation failure, "
         "status '%d'",
         {DisplayTypeDecInt32{"status"}}},
        {vehicle_power_service_JNI_OnLoad_vpd_native_adapter_jni_registration_failure,
         "JNI: vehicle_power_service::JNI_OnLoad: vpd_native_adapter_jni registation failure, "
         "status '%d'",
         {DisplayTypeDecInt32{"status"}}},
        {vehicle_power_service_JNI_OnLoad_vpd_native_receiver_jni_registration_failure,
         "JNI: vehicle_power_service::JNI_OnLoad: vpd_native_receiver_jni registration failure, "
         "status '%d'",
         {DisplayTypeDecInt32{"status"}}},
        {vpd_native_adapter_startProcessingCompleteNative,
         "JNI: vpd_native_adapter::startProcessingCompleteNative"},
        {vpd_native_adapter_startProcessingCompleteNative_vpsClient_is_null,
         "JNI: vpd_native_adapter::startProcessingCompleteNative: vpsClient is null"},
        {vpd_native_adapter_appStopProcessingCompleteNative,
         "JNI: vpd_native_adapter::appStopProcessingCompleteNative"},
        {vpd_native_adapter_appStopProcessingCompleteNative_vpsClient_is_null,
         "JNI: vpd_native_adapter::appStopProcessingCompleteNative: vpsClient is null"},
        {vpd_native_adapter_fwRestartProcessingCompleteNative,
         "JNI: vpd_native_adapter::fwRestartProcessingCompleteNative"},
        {vpd_native_adapter_fwRestartProcessingCompleteNative_vpsClient_is_null,
         "JNI: vpd_native_adapter::fwRestartProcessingCompleteNative: vpsClient is null"},
        {vpd_native_adapter_fwResumeProcessingCompleteNative,
         "JNI: vpd_native_adapter::fwResumeProcessingCompleteNative"},
        {vpd_native_adapter_fwResumeProcessingCompleteNative_vpsClient_is_null,
         "JNI: vpd_native_adapter::fwResumeProcessingCompleteNative: vpsClient is null"},
        {vpd_native_adapter_fwStopProcessingCompleteNative,
         "JNI: vpd_native_adapter::fwStopProcessingCompleteNative"},
        {vpd_native_adapter_fwStopProcessingCompleteNative_vpsClient_is_null,
         "JNI: vpd_native_adapter::fwStopProcessingCompleteNative: vpsClient is null"},
        {vpd_native_receiver_AttachCurrentThread_failed,
         "JNI: vpd_native_receiver: AttachCurrentThread failed, status '%d'",
         {DisplayTypeDecInt32{"status"}}},
        {vpd_native_receiver_onVpdStateChange_VPD_state_changed,
         "JNI: vpd_native_receiver::onVpdStateChange: VPD state changed to '%d'",
         {DisplayTypeDecInt32{"state"}}},
        {vpd_native_receiver_onFwRestart, "JNI: vpd_native_receiver::onFwRestart"},
        {vpd_native_receiver_onFwResume, "JNI: vpd_native_receiver::onFwResume"},
        {vpd_native_receiver_onFwStop, "JNI: vpd_native_receiver::onFwStop"},
        {vpd_native_receiver_classInitNative, "JNI: vpd_native_receiver::classInitNative"},
        {vpd_native_receiver_initNative, "JNI: vpd_native_receiver::initNative"},
        {vpd_native_adapter_stopProcessingFailedNative,
         "JNI: vpd_native_adapter::stopProcessingFailedNative"},
        {vpd_native_adapter_stopProcessingFailedNative_vpsClient_is_null,
         "JNI: vpd_native_adapter::stopProcessingFailedNative: vpsClient is null"},
        {vpd_native_adapter_startProcessingFailedNative,
         "JNI: vpd_native_adapter::stopProcessingFailedNative"},
        {vpd_native_adapter_startProcessingFailedNative_vpsClient_is_null,
         "JNI: vpd_native_adapter::stopProcessingFailedNative: vpsClient is null"},
        {vehicle_power_service_JNI_OnLoad_bootd_native_adapter_jni_registration_failure,
         "JNI: vehicle_power_service::JNI_OnLoad: bootd_native_adapter_jni registation failure, "
         "status '%d'",
         {DisplayTypeDecInt32{"status"}}},
        {vehicle_power_service_JNI_OnLoad_bootd_native_receiver_jni_registration_failure,
         "JNI: vehicle_power_service::JNI_OnLoad: bootd_native_receiver_jni registration failure, "
         "status '%d'",
         {DisplayTypeDecInt32{"status"}}},
        {bootd_native_adapter_hmiIsReadyNative, "JNI: bootd_native_adapter::hmiIsReadyNative"},
        {bootd_native_adapter_hmiIsReadyNative_bootdClient_is_null,
         "JNI: bootd_native_adapter::hmiIsReadtNative: bootdClient is null"},
        {bootd_native_receiver_classInitNative, "JNI: bootd_native_receiver::classInitNative"},
        {bootd_native_receiver_initNative, "JNI: bootd_native_receiver::initNative"},

        {vpd_native_adapter_appRestartProcessingCompleteNative,
         "JNI: vpd_native_adapter::appRestartProcessingCompleteNative"},
        {vpd_native_adapter_appRestartProcessingCompleteNative_vpsClient_is_null,
         "JNI: vpd_native_adapter::appRestartProcessingCompleteNative: vpsClient is null"},
        {vpd_native_adapter_appResumeProcessingCompleteNative,
         "JNI: vpd_native_adapter::appResumeProcessingCompleteNative"},
        {vpd_native_adapter_appResumeProcessingCompleteNative_vpsClient_is_null,
         "JNI: vpd_native_adapter::appResumeProcessingCompleteNative: vpsClient is null"},

        {vpd_native_receiver_requestMount, "JNI: vpd_native_receiver::requestMount"},
        {vpd_native_receiver_requestMount_AttachCurrentThread_failed,
         "JNI: vpd_native_receiver::requestMount: AttachCurrentThread failed, status "
         "'%d'",
         {DisplayTypeDecInt32{"status"}}},
        {vpd_native_adapter_unmountCompleteNative,
         "JNI: vpd_native_adapter::unmountCompleteNative"},
        {vpd_native_adapter_unmountCompleteNative_vpsClient_is_null,
         "JNI: vpd_native_adapter::unmountCompleteNative: vpsClient is null"},
        {vpd_native_adapter_endEarlyHmiNative, "JNI: vpd_native_adapter::endEarlyHmiNative"},
        {vpd_native_adapter_endEarlyHmiNative_vpsClient_is_null,
         "JNI: vpd_native_adapter::endEarlyHmiNative: vpsClient is null"},
        {vpd_native_adapter_forcedCompletionEarlyServicesNative,
         "JNI: vpd_native_adapter::forcedCompletionEarlyServices"},
        {vpd_native_adapter_forcedCompletionEarlyServicesNative_vpsClient_is_null,
         "JNI: vpd_native_adapter::forcedCompletionEarlyServices: vpsClient is null"},

        {bootd_native_receiver_handOffResponsibility,
         "JNI:vpd_native_receiver::handOffResponsibility called"},
        {bootd_native_receiver_handOffResponsibilityStartupAnimationDisplayed,
         "JNI:vpd_native_receiver::handOffResponsibilityStartupAnimationDisplayed called"},
        {bootd_native_receiver_handOffResponsibilityStartupAnimationAndDiDisplayed,
         "JNI:vpd_native_receiver::handOffResponsibilityStartupAnimationAndDiDisplayed called"},

        {vpd_native_receiver_onVpdStateChange_AttachCurrentThread_failed,
         "JNI:vpd_native_receiver_onVpdStateChange_AttachCurrentThread_failed"},
        {vpd_native_receiver_onFwRestart_AttachCurrentThread_failed,
         "JNI:vpd_native_receiver_onFwRestart_AttachCurrentThread_failed"},
        {vpd_native_receiver_onFwResume_AttachCurrentThread_failed,
         "JNI:vpd_native_receiver_onFwResume_AttachCurrentThread_failed"},
        {vpd_native_receiver_onFwStop_AttachCurrentThread_failed,
         "JNI:vpd_native_receiver_onFwStop_AttachCurrentThread_failed"},
        {vpd_native_receiver_onTimeOutError_AttachCurrentThread_failed,
         "JNI:vpd_native_receiver_onTimeOutError_AttachCurrentThread_failed"},
        {bootd_native_receiver_handOffResponsibilityStartupAnimationDisplayed_AttachCurrentThread_failed,
         "JNI:bootd_native_receiver_handOffResponsibilityStartupAnimationDisplayed_"
         "AttachCurrentThread_"
         "failed"},
        {bootd_native_receiver_handOffResponsibilityStartupAnimationAndDiDisplayed_AttachCurrentThread_failed,
         "JNI:bootd_native_receiver_handOffResponsibilityStartupAnimationAndDiDisplayed_"
         "AttachCurrentThread_failed"},
    };

    using com::mitsubishielectric::ahu::common::InitializeFunctionID;
    InitializeFunctionID(LogFuncId::VPS, vpsJNILogMessages);
}

void TerminateVpsJNILogMessages()
{
    using com::mitsubishielectric::ahu::common::TerminateFunctionID;
    TerminateFunctionID(LogFuncId::VPS);
}
