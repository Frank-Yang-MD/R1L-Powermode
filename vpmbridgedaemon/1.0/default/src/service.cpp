/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <hidl/HidlTransportSupport.h>

#include "AndroidPmAdapter.h"
#include "VpmBridgeDaemon.h"
#include "VpmBridgeDaemonLog.h"

using android::sp;

using android::hardware::configureRpcThreadpool;
using android::hardware::joinRpcThreadpool;

using android::OK;
using android::status_t;

using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::vpmbridgedaemon;

using V1_0::IVpmBridgeDaemon;
using V1_0::implementation::VpmBridgeDaemon;

int main()
{
    InitializeLogMessages();
    AndroidPmAdapter androidPmAdapter;
    android::sp<IVpmBridgeDaemon> service = new VpmBridgeDaemon{androidPmAdapter};

    configureRpcThreadpool(1U, true /*callerWillJoin*/);
    const status_t status = service->registerAsService();

    if (status == OK) {
        MLOGI(LogFuncId::VPBD, LogId::Main_Started_successfully);
        joinRpcThreadpool();
    }

    MLOGF(LogFuncId::VPBD, LogId::Main_Cannot_register_service);

    TerminateLogMessages();

    return 1;
}
