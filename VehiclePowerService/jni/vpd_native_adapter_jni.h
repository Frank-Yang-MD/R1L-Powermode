/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef VPD_NATIVE_ADAPTER_JNI_H
#define VPD_NATIVE_ADAPTER_JNI_H

#include <jni.h>

void subscribeVpd();
void unsubscribeVpd();

int register_vpd_native_adapter_jni(JNIEnv* env);

#endif  // VPD_NATIVE_ADAPTER_JNI_H
