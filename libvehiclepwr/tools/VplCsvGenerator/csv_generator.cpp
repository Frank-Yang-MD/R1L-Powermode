/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "LibVehiclePwrLog.h"
#include "Log.h"

#include <iostream>

int main()
{
    using com::mitsubishielectric::ahu::vehiclepwrmgr::lib::InitializeLibVehiclePwrLogMessages;
    InitializeLibVehiclePwrLogMessages();

    using com::mitsubishielectric::ahu::common::DumpToCsvConfig;
    using com::mitsubishielectric::ahu::vehiclepwrmgr::lib::LogFuncId::VPL;
    DumpToCsvConfig(VPL, std::cout);

    using com::mitsubishielectric::ahu::vehiclepwrmgr::lib::TerminateLibVehiclePwrLogMessages;
    TerminateLibVehiclePwrLogMessages();

    return 0;
}
