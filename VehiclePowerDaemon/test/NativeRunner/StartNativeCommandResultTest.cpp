/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "StartNativeCommandResult.h"

#include "MockAndroidInit.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class StartNativeCommandResultTest : public ::testing::Test {
public:
    const std::string mServiceRunningState{"running"};
    const std::vector<std::string> mServicesEmpty;
    const std::vector<std::string> mServices{"service_1", "service_2", "service_3"};
    MockAndroidInit mMockAndroidInit;
};

TEST_F(StartNativeCommandResultTest, no_native_services_to_wait_for)
{
    StartNativeCommandResult startNativeCommandResult(mServicesEmpty, mMockAndroidInit);

    EXPECT_CALL(mMockAndroidInit, propertyGet(::testing::_, ::testing::_)).Times(0);
    EXPECT_TRUE(startNativeCommandResult.isDone());
}

TEST_F(StartNativeCommandResultTest, propertyGet_is_called_once_for_every_service)
{
    StartNativeCommandResult startNativeCommandResult(mServices, mMockAndroidInit);

    EXPECT_CALL(mMockAndroidInit, propertyGet(::testing::_, ::testing::_))
        .Times(static_cast<int>(mServices.size()));
    EXPECT_FALSE(startNativeCommandResult.isDone());
}

TEST_F(StartNativeCommandResultTest, propertyGet_returns_with_different_length)
{
    StartNativeCommandResult startNativeCommandResult(mServices, mMockAndroidInit);

    EXPECT_CALL(mMockAndroidInit, propertyGet(::testing::_, ::testing::_))
        .WillOnce(::testing::DoAll(::testing::Return(mServiceRunningState.length())))
        .WillRepeatedly(::testing::Return(0));
    EXPECT_FALSE(startNativeCommandResult.isDone());
}

TEST_F(StartNativeCommandResultTest, all_services_are_running)
{
    StartNativeCommandResult startNativeCommandResult(mServices, mMockAndroidInit);

    for (auto const& service : mServices) {
        EXPECT_CALL(mMockAndroidInit, propertyGet(::testing::StrEq(service), ::testing::_))
            .WillOnce(::testing::DoAll(::testing::SetArgReferee<1>(mServiceRunningState),
                                       ::testing::Return(mServiceRunningState.length())));
    }
    EXPECT_TRUE(startNativeCommandResult.isDone());
}

TEST_F(StartNativeCommandResultTest, services_are_not_running)
{
    StartNativeCommandResult startNativeCommandResult(mServices, mMockAndroidInit);

    EXPECT_CALL(mMockAndroidInit, propertyGet(::testing::_, ::testing::_))
        .Times(static_cast<int>(mServices.size()));
    EXPECT_FALSE(startNativeCommandResult.isDone());
}

TEST_F(StartNativeCommandResultTest, some_services_are_running_and_some_not)
{
    StartNativeCommandResult startNativeCommandResult(mServices, mMockAndroidInit);

    EXPECT_CALL(mMockAndroidInit, propertyGet(::testing::_, ::testing::_))
        .WillOnce(::testing::DoAll(::testing::SetArgReferee<1>(mServiceRunningState),
                                   ::testing::Return(mServiceRunningState.length())))
        .WillRepeatedly(::testing::Return(0));
    EXPECT_FALSE(startNativeCommandResult.isDone());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
