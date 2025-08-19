/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "vpd_native_receiver_jni.h"

#include "vehiclepwrmgr_jni.h"
#include "vehiclepwrmgr_jni_log.h"

#include "nativehelper/JNIHelp.h"

#include <functional>

static jobject sObj;

static jmethodID method_onVpdStateChange;
static jmethodID method_onFwRestart;
static jmethodID method_onFwResume;
static jmethodID method_onFwStop;
static jmethodID method_onTimeOutError;
static jmethodID method_requestMount;
static jmethodID method_requestUnmount;
static jmethodID method_handOffResponsibility;
static jmethodID method_handOffResponsibilityStartupAnimationDisplayed;
static jmethodID method_handOffResponsibilityStartupAnimationAndDiDisplayed;

static void threadWrap(std::function<void(JNIEnv*)> fExecute, std::function<void(int)> fErrorLog)
{
    JNIEnv* env = nullptr;

    const bool detached =
        sJvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) == JNI_EDETACHED;
    if (detached) {
        const int rs = sJvm->AttachCurrentThread(&env, nullptr);
        if (rs) {
            fErrorLog(rs);
            return;
        }
    }

    fExecute(env);

    if (detached) {
        sJvm->DetachCurrentThread();
    }
}

void NativeCallback::onSystemStateChange(common::EVpdState vpdState)
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_receiver_onVpdStateChange_VPD_state_changed,
          static_cast<int>(vpdState));
    threadWrap(
        [vpdState](JNIEnv* e) {
            e->CallVoidMethod(sObj, method_onVpdStateChange, static_cast<int>(vpdState));
        },
        [](int rs) {
            MLOGE(LogFuncId::VPS,
                  LogId::vpd_native_receiver_onVpdStateChange_AttachCurrentThread_failed, rs);
        });
}

void NativeCallback::onFwRestart()
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_receiver_onFwRestart);
    threadWrap([](JNIEnv* e) { e->CallVoidMethod(sObj, method_onFwRestart); },
               [](int rs) {
                   MLOGE(LogFuncId::VPS,
                         LogId::vpd_native_receiver_onFwRestart_AttachCurrentThread_failed, rs);
               });
}

void NativeCallback::onFwResume()
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_receiver_onFwResume);
    threadWrap([](JNIEnv* e) { e->CallVoidMethod(sObj, method_onFwResume); },
               [](int rs) {
                   MLOGE(LogFuncId::VPS,
                         LogId::vpd_native_receiver_onFwResume_AttachCurrentThread_failed, rs);
               });
}

void NativeCallback::onFwStop()
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_receiver_onFwStop);
    threadWrap([](JNIEnv* e) { e->CallVoidMethod(sObj, method_onFwStop); },
               [](int rs) {
                   MLOGE(LogFuncId::VPS,
                         LogId::vpd_native_receiver_onFwStop_AttachCurrentThread_failed, rs);
               });
}

void NativeCallback::onTimeOutError()
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_receiver_onTimeOutError);
    threadWrap([](JNIEnv* e) { e->CallVoidMethod(sObj, method_onTimeOutError); },
               [](int rs) {
                   MLOGE(LogFuncId::VPS,
                         LogId::vpd_native_receiver_onTimeOutError_AttachCurrentThread_failed, rs);
               });
}

void NativeCallback::requestMount()
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_receiver_requestMount);
    threadWrap([](JNIEnv* e) { e->CallVoidMethod(sObj, method_requestMount); },
               [](int rs) {
                   MLOGE(LogFuncId::VPS,
                         LogId::vpd_native_receiver_requestMount_AttachCurrentThread_failed, rs);
               });
}

void NativeCallback::requestUnmount()
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_receiver_requestUnmount);

    threadWrap([](JNIEnv* e) { e->CallVoidMethod(sObj, method_requestUnmount); },
               [](int rs) {
                   MLOGE(LogFuncId::VPS,
                         LogId::vpd_native_receiver_requestUnmount_AttachCurrentThread_failed, rs);
               });
}

void NativeCallback::handOffResponsibility()
{
    MLOGD(LogFuncId::VPS, LogId::bootd_native_receiver_handOffResponsibility);
    threadWrap(
        [](JNIEnv* e) { e->CallVoidMethod(sObj, method_handOffResponsibility); },
        [](int rs) {
            MLOGE(LogFuncId::VPS,
                  LogId::bootd_native_receiver_handOffResponsibility_AttachCurrentThread_failed,
                  rs);
        });
}
void NativeCallback::handOffResponsibilityStartupAnimationDisplayed()
{
    MLOGD(LogFuncId::VPS,
          LogId::bootd_native_receiver_handOffResponsibilityStartupAnimationDisplayed);
    threadWrap(
        [](JNIEnv* e) {
            e->CallVoidMethod(sObj, method_handOffResponsibilityStartupAnimationDisplayed);
        },
        [](int rs) {
            MLOGE(
                LogFuncId::VPS,
                LogId::
                    bootd_native_receiver_handOffResponsibilityStartupAnimationDisplayed_AttachCurrentThread_failed,
                rs);
        });
}
void NativeCallback::handOffResponsibilityStartupAnimationAndDiDisplayed()
{
    MLOGD(LogFuncId::VPS,
          LogId::bootd_native_receiver_handOffResponsibilityStartupAnimationAndDiDisplayed);
    threadWrap(
        [](JNIEnv* e) {
            e->CallVoidMethod(sObj, method_handOffResponsibilityStartupAnimationAndDiDisplayed);
        },
        [](int rs) {
            MLOGE(
                LogFuncId::VPS,
                LogId::
                    bootd_native_receiver_handOffResponsibilityStartupAnimationAndDiDisplayed_AttachCurrentThread_failed,
                rs);
        });
}

static void classInitNative(JNIEnv* env, jclass clazz)
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_receiver_classInitNative);
    method_onVpdStateChange = env->GetMethodID(clazz, "onVpdStateChange", "(I)V");
    method_onFwRestart = env->GetMethodID(clazz, "onFwRestart", "()V");
    method_onFwResume = env->GetMethodID(clazz, "onFwResume", "()V");
    method_onFwStop = env->GetMethodID(clazz, "onFwStop", "()V");
    method_onTimeOutError = env->GetMethodID(clazz, "onTimeOutError", "()V");
    method_requestMount = env->GetMethodID(clazz, "requestMount", "()V");
    method_requestUnmount = env->GetMethodID(clazz, "requestUnmount", "()V");
    method_handOffResponsibility = env->GetMethodID(clazz, "handOffResponsibility", "()V");
    method_handOffResponsibilityStartupAnimationDisplayed =
        env->GetMethodID(clazz, "handOffResponsibilityStartupAnimationDisplayed", "()V");
    method_handOffResponsibilityStartupAnimationAndDiDisplayed =
        env->GetMethodID(clazz, "handOffResponsibilityStartupAnimationAndDiDisplayed", "()V");
}

static void initNative(JNIEnv* env, jobject object)
{
    MLOGD(LogFuncId::VPS, LogId::vpd_native_receiver_initNative);
    sObj = env->NewGlobalRef(object);
}

static JNINativeMethod sMethods[] = {
    {"classInitNative", "()V", (void*)classInitNative},
    {"initNative", "()V", (void*)initNative},
};

int register_vpd_native_receiver_jni(JNIEnv* env)
{
    return jniRegisterNativeMethods(
        env, "com/mitsubishielectric/ahu/efw/vehiclepwrmgr/vpdnative/VpdNativeReceiver", sMethods,
        NELEM(sMethods));
}
