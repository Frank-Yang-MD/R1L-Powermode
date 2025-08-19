/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "Log.h"
#include "VpmBridgeDaemonLog.h"

#include <iostream>

int main()
{
    using com::mitsubishielectric::ahu::vehiclepwrmgr::vpmbridgedaemon::InitializeLogMessages;
    InitializeLogMessages();

    using com::mitsubishielectric::ahu::common::DumpToCsvConfig;
    using com::mitsubishielectric::ahu::vehiclepwrmgr::vpmbridgedaemon::LogFuncId::VPBD;
    DumpToCsvConfig(VPBD, std::cout);

    using com::mitsubishielectric::ahu::vehiclepwrmgr::vpmbridgedaemon::TerminateLogMessages;
    TerminateLogMessages();

    return 0;
}
