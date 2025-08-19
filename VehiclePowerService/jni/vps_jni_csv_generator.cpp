/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "Log.h"
#include "vehiclepwrmgr_jni_log.h"

#include <iostream>

int main()
{
    InitializeVpsJNILogMessages();

    using ::com::mitsubishielectric::ahu::common::DumpToCsvConfig;
    DumpToCsvConfig(LogFuncId::VPS, std::cout);

    TerminateVpsJNILogMessages();

    return 0;
}
