/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MockActions.h"
#include "MockTimeOutControl.h"
#include "TimeoutNotification.h"

#include <chrono>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using namespace std::chrono_literals;

class TimeoutNotificationTest : public ::testing::Test {
protected:
    TimeoutNotificationTest()
        : mTimeoutNotification{mTimeoutControl, mVcpuActions, 1ms, 1ms}
    {
    }

    MockTimeOutControl mTimeoutControl;
    MockVcpuActions mVcpuActions;

    TimeoutNotification mTimeoutNotification;
};

TEST_F(TimeoutNotificationTest, createTimerTest)
{
    auto timer = mTimeoutNotification.createTimer(std::chrono::microseconds(1));
    EXPECT_TRUE(timer != nullptr);
}

TEST_F(TimeoutNotificationTest, getAbnormalHandlerTest)
{
    auto abnormalHandler = mTimeoutNotification.getAbnormalHandler();
    EXPECT_TRUE(abnormalHandler != nullptr);
}

TEST_F(TimeoutNotificationTest, getWatchDogHandlerTest)
{
    auto watchDogHandler = mTimeoutNotification.getWatchDogHandler();
    EXPECT_TRUE(watchDogHandler != nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com