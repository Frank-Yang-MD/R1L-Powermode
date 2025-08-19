/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "vehiclepwrmgr_jni.h"

#include "vpd_native_adapter_jni.h"
#include "vpd_native_receiver_jni.h"

#include "vehiclepwrmgr_jni_log.h"

#include "LibVehiclePwrLog.h"

#include "nativehelper/JNIHelp.h"

JavaVM* sJvm;

static void initNative(JNIEnv*, jobject)
{
    MLOGD(LogFuncId::VPS, LogId::vehicle_power_service_initNative);

    subscribeVpd();
}

static void cleanupNative(JNIEnv*, jobject)
{
    MLOGD(LogFuncId::VPS, LogId::vehicle_power_service_cleanupNative);

    unsubscribeVpd();

    TerminateVpsJNILogMessages();
    using com::mitsubishielectric::ahu::vehiclepwrmgr::lib::TerminateLibVehiclePwrLogMessages;
    TerminateLibVehiclePwrLogMessages();
}

static JNINativeMethod sMethods[] = {
    {"initNative", "()V", (void*)initNative},
    {"cleanupNative", "()V", (void*)cleanupNative},
};

int register_vehicle_power_service_jni(JNIEnv* env)
{
    MLOGD(LogFuncId::VPS, LogId::vehicle_power_service_register_vehicle_power_service_jni);
    return jniRegisterNativeMethods(
        env, "com/mitsubishielectric/ahu/efw/vehiclepwrmgr/VehiclePowerService", sMethods,
        NELEM(sMethods));
}

jint JNI_OnLoad(JavaVM* vm, void*)
{
    using com::mitsubishielectric::ahu::vehiclepwrmgr::lib::InitializeLibVehiclePwrLogMessages;
    InitializeLibVehiclePwrLogMessages();
    InitializeVpsJNILogMessages();
    MLOGD(LogFuncId::VPS, LogId::vehicle_power_service_JNI_OnLoad);
    JNIEnv* e;
    int status;

    sJvm = vm;

    if (vm->GetEnv((void**)&e, JNI_VERSION_1_6)) {
        MLOGF(LogFuncId::VPS, LogId::vehicle_power_service_JNI_OnLoad_JNI_version_missmatch_error);
        return JNI_ERR;
    }

    if ((status = register_vehicle_power_service_jni(e)) < 0) {
        MLOGF(
            LogFuncId::VPS,
            LogId::vehicle_power_service_JNI_OnLoad_vehicle_power_service_jni_registration_failure,
            status);
        return JNI_ERR;
    }

    if ((status = register_vpd_native_adapter_jni(e)) < 0) {
        MLOGF(LogFuncId::VPS,
              LogId::vehicle_power_service_JNI_OnLoad_vpd_native_adapter_jni_registration_failure,
              status);
        return JNI_ERR;
    }

    if ((status = register_vpd_native_receiver_jni(e)) < 0) {
        MLOGF(LogFuncId::VPS,
              LogId::vehicle_power_service_JNI_OnLoad_vpd_native_receiver_jni_registration_failure,
              status);
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}
