/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gtest/gtest.h>

#include "VehiclePowerDaemonLog.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    namespace vpd = com::mitsubishielectric::ahu::vehiclepwrmgr::impl;
    vpd::InitializeVehiclePowerDaemonLogMessages();
    auto result = RUN_ALL_TESTS();
    vpd::TerminateVehiclePowerDaemonLogMessages();

    return result;
}