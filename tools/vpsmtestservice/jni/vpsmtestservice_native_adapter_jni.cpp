/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */
#define LOG_TAG "VPSM_TEST"
#include "vpsmtestservice_jni.h"

#include "ClientCreator.h"
#include "EVpdState.h"

#include "nativehelper/JNIHelp.h"
#include <android/log.h>

using namespace com::mitsubishielectric::ahu::vehiclepwrmgr;
using com::mitsubishielectric::ahu::vehiclepwrmgr::common::EVpdState;

const char* EVpdStateToString(EVpdState value)
{
    switch (value) {
    case EVpdState::APP_START_NA:
        return "APP_START_NA";
    case EVpdState::APP_START_COLDBOOT:
        return "APP_START_COLDBOOT";
    case EVpdState::APP_STOP:
        return "APP_STOP";
    case EVpdState::APP_RESUME:
        return "APP_RESUME";
    case EVpdState::APP_RESTART:
        return "APP_RESTART";
    }

    return "UNKNOWN_STATE";
}

static int getVpdStateNative(JNIEnv*, jobject)
{
    ALOGI("JNI. Native adapter: getVpdStateNative");
    common::EVpdState vpdState = EVpdState::APP_START_NA;
    if (nativeClient) {
        vpdState = nativeClient->GetVPDState();
        ALOGI("JNI. Native adapter: VPD state is %s", EVpdStateToString(vpdState));
    }
    else {
        ALOGW("JNI. Native adapter: Native client is null");
    }
    return static_cast<int>(vpdState);
}

static JNINativeMethod sMethods[] = {
    {"getVpdStateNative", "()I", (int*)getVpdStateNative},
};

int register_vpsmtestservice_native_adapter_jni(JNIEnv* env)
{
    return jniRegisterNativeMethods(
        env, "com/mitsubishielectric/ahu/efw/vehiclepwrmgr/vpmtestservice/VpdNativeAdapter",
        sMethods, NELEM(sMethods));
}
