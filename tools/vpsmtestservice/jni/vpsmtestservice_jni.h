/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef VPSMTESTSERVICE_JNI_H
#define VPSMTESTSERVICE_JNI_H

#include "ClientCreator.h"
#include "LibVehiclePwrLog.h"
#include "jni.h"
#include "nativehelper/JNIHelp.h"

using namespace com::mitsubishielectric::ahu::vehiclepwrmgr;

extern JNIEnv* jEnv;
extern JavaVM* sJvm;
extern lib::INativeClient* nativeClient;
int register_vpsmtestservice_native_adapter_jni(JNIEnv* env);

#endif  // VPSMTESTSERVICE_JNI_H
