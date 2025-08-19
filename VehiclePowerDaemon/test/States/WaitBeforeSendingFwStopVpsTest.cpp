/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "VpdStateMachine.h"
#include "WaitBeforeSendingFwStopVps.h"
#include "WaitForFwStopVps.h"

#include "MockActions.h"
#include "MockNotifyHelper.h"
#include "MockSM.h"
#include "MockTimer.h"

#include "Timer.h"

using namespace testing;

using ::testing::NiceMock;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdState;

/**
 * Test Fixture Object
 */
class WaitBeforeSendingFwStopVpsTest : public ::testing::Test {
public:
protected:
    void SetUp()
    {
        WaitForFwStopVps::initInstance(&mMockSM, mMockNotifyHelper, mStopVpsTimer,
                                       mMockVcpuActions);
        WaitBeforeSendingFwStopVps::initInstance(&mMockSM, mMockTimer);
    }

    NiceMock<MockSM> mMockSM;
    NiceMock<MockTimer> mMockTimer;
    NiceMock<MockTimer> mStopVpsTimer;
    NiceMock<MockNotifyHelper> mMockNotifyHelper;
    NiceMock<MockVcpuActions> mMockVcpuActions;
};

/**
 * Check method "onEnter" from state (WaitBeforeSendingFwStopVps)
 */
TEST_F(WaitBeforeSendingFwStopVpsTest, onEnter)
{
    auto state = WaitBeforeSendingFwStopVps::getInstance();

    EXPECT_CALL(mMockTimer, start()).Times(1);

    // Run Test
    state->onEnter();
}

/**
 * Check method "onTimeout" from state (WaitBeforeSendingFwStopVps)
 */
TEST_F(WaitBeforeSendingFwStopVpsTest, onTimeout)
{
    auto state = WaitBeforeSendingFwStopVps::getInstance();

    ON_CALL(mMockTimer, isExpired()).WillByDefault(Return(true));
    EXPECT_CALL(mMockTimer, isExpired()).Times(1);
    ASSERT_NE(WaitForFwStopVps::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitForFwStopVps::getInstance())).Times(1);

    // Run Test
    state->onTimeout();
}

/**
 * Check method "onTimeout" from state (WaitBeforeSendingFwStopVps)
 * negative case
 */
TEST_F(WaitBeforeSendingFwStopVpsTest, onTimeoutNegative)
{
    auto state = WaitBeforeSendingFwStopVps::getInstance();

    ON_CALL(mMockTimer, isExpired()).WillByDefault(Return(false));
    EXPECT_CALL(mMockTimer, isExpired()).Times(1);
    ASSERT_NE(WaitForFwStopVps::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitForFwStopVps::getInstance())).Times(0);

    // Run Test
    state->onTimeout();
}

/**
 * check state name
 */
TEST_F(WaitBeforeSendingFwStopVpsTest, stateName)
{
    auto state = WaitBeforeSendingFwStopVps::getInstance();

    ASSERT_STREQ(state->stateName().c_str(), "WaitBeforeSendingFwStopVps");
}

TEST_F(WaitBeforeSendingFwStopVpsTest, getInstance)
{
    auto state = WaitBeforeSendingFwStopVps::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitBeforeSendingFwStopVpsTest, uninitInstance)
{
    WaitBeforeSendingFwStopVps::uninitInstance();
    auto state = WaitBeforeSendingFwStopVps::getInstance();
    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
