#include <gtest/gtest.h>

#include "log.h"

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    // globalArgc = argc;
    // globalArgv = argv;

    namespace vpd = com::mitsubishielectric::ahu::vehiclepwrmgr::impl;
    vpd::LogInitializer _;
    return RUN_ALL_TESTS();

    return 0;
}