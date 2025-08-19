/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "Log.h"
#include "VehiclePowerDaemonLog.h"

#include <iostream>

int main()
{
    using com::mitsubishielectric::ahu::vehiclepwrmgr::impl::
        InitializeVehiclePowerDaemonLogMessages;
    InitializeVehiclePowerDaemonLogMessages();

    using com::mitsubishielectric::ahu::common::DumpToCsvConfig;
    using com::mitsubishielectric::ahu::vehiclepwrmgr::impl::LogFuncId::VPD;
    DumpToCsvConfig(VPD, std::cout);

    using com::mitsubishielectric::ahu::vehiclepwrmgr::impl::TerminateVehiclePowerDaemonLogMessages;
    TerminateVehiclePowerDaemonLogMessages();

    return 0;
}
