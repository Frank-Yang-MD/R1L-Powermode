/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef VPD_NATIVE_RECEIVER_JNI_H
#define VPD_NATIVE_RECEIVER_JNI_H

#include "IVpsClient.h"

#include <jni.h>

using namespace com::mitsubishielectric::ahu::vehiclepwrmgr;

class NativeCallback final
    : public lib::INativeCallback
    , public lib::IVpsCallback {
public:
    virtual void onSystemStateChange(common::EVpdState) override;

    virtual void onFwRestart() override;
    virtual void onFwResume() override;
    virtual void onFwStop() override;
    virtual void onTimeOutError() override;
    virtual void requestMount() override;
    virtual void requestUnmount() override;
    virtual void handOffResponsibility() override;
    virtual void handOffResponsibilityStartupAnimationDisplayed() override;
    virtual void handOffResponsibilityStartupAnimationAndDiDisplayed() override;
};

int register_vpd_native_receiver_jni(JNIEnv* env);

#endif  // VPD_NATIVE_RECEIVER_JNI_H
