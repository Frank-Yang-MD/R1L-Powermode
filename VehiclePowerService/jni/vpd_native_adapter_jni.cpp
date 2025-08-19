/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "vpd_native_adapter_jni.h"

#include "vpd_native_receiver_jni.h"

#include "vehiclepwrmgr_jni_log.h"

#include "ClientCreator.h"

#include "jni.h"
#include "nativehelper/JNIHelp.h"

using namespace com::mitsubishielectric::ahu::vehiclepwrmgr;

lib::IVpsClient* vpsClient;
NativeCallback nativeCallback;

void subscribeVpd()
{
    vpsClient = lib::ClientCreator::CreateVpsClient();
    if (vpsClient) {
        vpsClient->subscribe(&nativeCallback);
        vpsClient->subscribeVps(&nativeCallback);
    }
    else {
        MLOGF(LogFuncId::VPS, LogId::vehicle_power_service_initNative_vpsClient_is_null);
    }
}

void unsubscribeVpd()
{
    if (vpsClient) {
        vpsClient->unsubscribe();
    }
    else {
        MLOGF(LogFuncId::VPS, LogId::vehicle_power_service_cleanupNative_vpsClient_is_null);
    }
}

static void startProcessingCompleteNative(JNIEnv*, jobject)
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_adapter_startProcessingCompleteNative);
    if (vpsClient) {
        vpsClient->startProcessingComplete();
    }
    else {
        MLOGF(LogFuncId::VPS,
              LogId::vpd_native_adapter_startProcessingCompleteNative_vpsClient_is_null);
    }
}

static void startProcessingFailedNative(JNIEnv*, jobject)
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_adapter_startProcessingFailedNative);
    if (vpsClient) {
        vpsClient->startProcessingFailed();
    }
    else {
        MLOGF(LogFuncId::VPS,
              LogId::vpd_native_adapter_startProcessingFailedNative_vpsClient_is_null);
    }
}

static void appStopProcessingCompleteNative(JNIEnv*, jobject)
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_adapter_appStopProcessingCompleteNative);
    if (vpsClient) {
        vpsClient->appStopProcessingComplete();
    }
    else {
        MLOGF(LogFuncId::VPS,
              LogId::vpd_native_adapter_appStopProcessingCompleteNative_vpsClient_is_null);
    }
}

static void fwRestartProcessingCompleteNative(JNIEnv*, jobject)
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_adapter_fwRestartProcessingCompleteNative);
    if (vpsClient) {
        vpsClient->fwRestartProcessingComplete();
    }
    else {
        MLOGF(LogFuncId::VPS,
              LogId::vpd_native_adapter_fwRestartProcessingCompleteNative_vpsClient_is_null);
    }
}

static void fwResumeProcessingCompleteNative(JNIEnv*, jobject)
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_adapter_fwResumeProcessingCompleteNative);
    if (vpsClient) {
        vpsClient->fwResumeProcessingComplete();
    }
    else {
        MLOGF(LogFuncId::VPS,
              LogId::vpd_native_adapter_fwResumeProcessingCompleteNative_vpsClient_is_null);
    }
}

static void appRestartProcessingCompleteNative(JNIEnv*, jobject)
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_adapter_appRestartProcessingCompleteNative);
    if (vpsClient) {
        vpsClient->appRestartProcessingComplete();
    }
    else {
        MLOGF(LogFuncId::VPS,
              LogId::vpd_native_adapter_appRestartProcessingCompleteNative_vpsClient_is_null);
    }
}

static void appResumeProcessingCompleteNative(JNIEnv*, jobject)
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_adapter_appResumeProcessingCompleteNative);
    if (vpsClient) {
        vpsClient->appResumeProcessingComplete();
    }
    else {
        MLOGF(LogFuncId::VPS,
              LogId::vpd_native_adapter_appResumeProcessingCompleteNative_vpsClient_is_null);
    }
}

static void fwStopProcessingCompleteNative(JNIEnv*, jobject)
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_adapter_fwStopProcessingCompleteNative);
    if (vpsClient) {
        vpsClient->fwStopProcessingComplete();
    }
    else {
        MLOGF(LogFuncId::VPS,
              LogId::vpd_native_adapter_fwStopProcessingCompleteNative_vpsClient_is_null);
    }
}

static void stopProcessingFailedNative(JNIEnv*, jobject)
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_adapter_stopProcessingFailedNative);
    if (vpsClient) {
        vpsClient->stopProcessingFailed();
    }
    else {
        MLOGF(LogFuncId::VPS,
              LogId::vpd_native_adapter_stopProcessingFailedNative_vpsClient_is_null);
    }
}

static void hmiIsReadyNative(JNIEnv*, jobject)
{
    MLOGD(LogFuncId::VPS, LogId::bootd_native_adapter_hmiIsReadyNative);
    if (vpsClient) {
        vpsClient->vpsReady();
    }
    else {
        MLOGF(LogFuncId::VPS, LogId::bootd_native_adapter_hmiIsReadyNative_bootdClient_is_null);
    }
}

static void unmountCompleteNative(JNIEnv*, jobject)
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_adapter_unmountCompleteNative);
    if (vpsClient) {
        vpsClient->unmountComplete();
    }
    else {
        MLOGF(LogFuncId::VPS, LogId::vpd_native_adapter_unmountCompleteNative_vpsClient_is_null);
    }
}

static void endEarlyHmiNative(JNIEnv*, jobject)
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_adapter_endEarlyHmiNative);
    if (vpsClient) {
        vpsClient->endEarlyHmi();
    }
    else {
        MLOGF(LogFuncId::VPS, LogId::vpd_native_adapter_endEarlyHmiNative_vpsClient_is_null);
    }
}

static void forcedCompletionEarlyServicesNative(JNIEnv*, jobject)
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_adapter_forcedCompletionEarlyServicesNative);
    if (vpsClient) {
        vpsClient->forcedCompletionEarlyServices();
    }
    else {
        MLOGF(LogFuncId::VPS,
              LogId::vpd_native_adapter_forcedCompletionEarlyServicesNative_vpsClient_is_null);
    }
}

static JNINativeMethod sMethods[] = {
    {"startProcessingCompleteNative", "()V", (void*)startProcessingCompleteNative},
    {"startProcessingFailedNative", "()V", (void*)startProcessingFailedNative},
    {"appStopProcessingCompleteNative", "()V", (void*)appStopProcessingCompleteNative},
    {"fwRestartProcessingCompleteNative", "()V", (void*)fwRestartProcessingCompleteNative},
    {"fwResumeProcessingCompleteNative", "()V", (void*)fwResumeProcessingCompleteNative},
    {"appRestartProcessingCompleteNative", "()V", (void*)appRestartProcessingCompleteNative},
    {"appResumeProcessingCompleteNative", "()V", (void*)appResumeProcessingCompleteNative},
    {"fwStopProcessingCompleteNative", "()V", (void*)fwStopProcessingCompleteNative},
    {"stopProcessingFailedNative", "()V", (void*)stopProcessingFailedNative},
    {"hmiIsReadyNative", "()V", (void*)hmiIsReadyNative},
    {"unmountCompleteNative", "()V", (void*)unmountCompleteNative},
    {"endEarlyHmiNative", "()V", (void*)endEarlyHmiNative},
    {"forcedCompletionEarlyServicesNative", "()V", (void*)forcedCompletionEarlyServicesNative},
};

int register_vpd_native_adapter_jni(JNIEnv* env)
{
    return jniRegisterNativeMethods(
        env, "com/mitsubishielectric/ahu/efw/vehiclepwrmgr/vpdnative/VpdNativeAdapter", sMethods,
        NELEM(sMethods));
}
