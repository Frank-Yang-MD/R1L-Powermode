/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */
#define LOG_TAG "VPSM_TEST"
#include "vpsmtestservice_jni.h"
#include <android/log.h>

JNIEnv* jEnv;
JavaVM* sJvm;
lib::INativeClient* nativeClient;

static void initNative(JNIEnv*, jobject)
{
    ALOGI("JNI. VpmsTestService: Init native");
    nativeClient = lib::ClientCreator::CreateNativeClient();
    if (nativeClient) {
        ALOGI("JNI. VpmsTestService: Native client has been created");
    }
    else {
        ALOGW("JNI. VpmsTestService: Failed to create Native client");
    }
}

static void cleanupNative(JNIEnv*, jobject)
{
    ALOGI("JNI. VpmsTestService: Cleanup native");

    using com::mitsubishielectric::ahu::vehiclepwrmgr::lib::TerminateLibVehiclePwrLogMessages;
    TerminateLibVehiclePwrLogMessages();
}

static JNINativeMethod sMethods[] = {
    {"initNative", "()V", (void*)initNative},
    {"cleanupNative", "()V", (void*)cleanupNative},
};

int register_vehicle_power_service_jni(JNIEnv* env)
{
    ALOGI("JNI. VpmsTestService: Register vpsmtestservice JNI");
    return jniRegisterNativeMethods(
        env, "com/mitsubishielectric/ahu/efw/vehiclepwrmgr/vpmtestservice/VpmsTestService",
        sMethods, NELEM(sMethods));
}

jint JNI_OnLoad(JavaVM* vm, void*)
{
    using com::mitsubishielectric::ahu::vehiclepwrmgr::lib::InitializeLibVehiclePwrLogMessages;
    InitializeLibVehiclePwrLogMessages();
    ALOGI("JNI. VpmsTestService: JNIonLoad");
    JNIEnv* e;
    int status;

    sJvm = vm;

    if (vm->GetEnv((void**)&e, JNI_VERSION_1_6)) {
        ALOGI("JNI. VpmsTestService: JNIonLoad: JNI version mismatch error");
        return JNI_ERR;
    }

    if ((status = register_vehicle_power_service_jni(e)) < 0) {
        ALOGI("JNI. VpmsTestService: JNIonLoad: JNI registration failure %d", status);
        return JNI_ERR;
    }

    if ((status = register_vpsmtestservice_native_adapter_jni(e)) < 0) {
        ALOGI("JNI. VpmsTestService: JNIonLoad: VPD native adapter JNI registration failure %d",
              status);
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}
