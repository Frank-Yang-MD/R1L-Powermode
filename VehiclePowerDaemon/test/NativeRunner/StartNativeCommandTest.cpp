/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "StartNativeCommand.h"
#include "IConfigScriptCommand.h"
#include "MockAndroidInit.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class StartNativeCommandTest : public ::testing::Test {
public:
    StartNativeCommandTest()
        : mStartNativeCommand{mPropName, mServices, mAndroidInit}
    {
    }

    const std::string mPropName{"prop_name"};
    const std::vector<std::string> mServices{"daemon1", "daemon2"};
    const MockAndroidInit mAndroidInit;

    StartNativeCommand mStartNativeCommand;
};

TEST_F(StartNativeCommandTest, check_normal_flow)
{
    const int successRetCode = 0;
    EXPECT_CALL(mAndroidInit, propertySet(mPropName, ::testing::_))
        .Times(1)
        .WillOnce(::testing::Return(successRetCode));

    auto cmdResult = mStartNativeCommand.execute();
    EXPECT_NE(cmdResult, nullptr);
}

TEST_F(StartNativeCommandTest, fail_of_propertySet_method_is_properly_handled)
{
    const int errorRetCode = -1;
    EXPECT_CALL(mAndroidInit, propertySet(mPropName, ::testing::_))
        .Times(1)
        .WillOnce(::testing::Return(errorRetCode));

    auto cmdResult = mStartNativeCommand.execute();
    EXPECT_EQ(cmdResult, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
