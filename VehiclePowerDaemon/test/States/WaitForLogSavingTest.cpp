/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "LogDaemonBusy.h"
#include "VpdStateMachine.h"
#include "WaitForLogSaving.h"

#include "MockActions.h"
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
class WaitForLogSavingTest : public ::testing::Test {
public:
protected:
    void SetUp()
    {
        LogDaemonBusy::initInstance(&mMockSM, mMockLogDaemonActions, mMockCheckTimer,
                                    mMockStopTimer, mMockVcpuActions);
        WaitForLogSaving::initInstance(&mMockSM, mMockTimer);
    }

    NiceMock<MockSM> mMockSM;
    NiceMock<MockTimer> mMockTimer;
    NiceMock<MockTimer> mMockCheckTimer;
    NiceMock<MockTimer> mMockStopTimer;
    NiceMock<MockLogDaemonActions> mMockLogDaemonActions;
    NiceMock<MockVcpuActions> mMockVcpuActions;
};

/**
 * Check method "onEnter" from state S7.1(WaitForLogSaving)
 */
TEST_F(WaitForLogSavingTest, onEnter)
{
    auto state = WaitForLogSaving::getInstance();

    EXPECT_CALL(mMockTimer, start()).Times(1);

    // Run Test
    state->onEnter();
}

/**
 * Check method "onTimeout" from state S7.1 (WaitForLogSaving)
 */
TEST_F(WaitForLogSavingTest, onTimeout)
{
    auto state = WaitForLogSaving::getInstance();

    ON_CALL(mMockTimer, isExpired()).WillByDefault(Return(true));
    EXPECT_CALL(mMockTimer, isExpired()).Times(1);
    ASSERT_NE(LogDaemonBusy::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(LogDaemonBusy::getInstance())).Times(1);

    // Run Test
    state->onTimeout();
}

/**
 * Check method "onTimeout" from state S7.1 (WaitForLogSaving)
 * negative case
 */
TEST_F(WaitForLogSavingTest, onTimeoutNegative)
{
    auto state = WaitForLogSaving::getInstance();

    ON_CALL(mMockTimer, isExpired()).WillByDefault(Return(false));
    EXPECT_CALL(mMockTimer, isExpired()).Times(1);
    ASSERT_NE(LogDaemonBusy::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(LogDaemonBusy::getInstance())).Times(0);

    // Run Test
    state->onTimeout();
}

/**
 * check state name
 */
TEST_F(WaitForLogSavingTest, stateName)
{
    auto state = WaitForLogSaving::getInstance();

    ASSERT_STREQ(state->stateName().c_str(), "WaitForLogSaving");
}

TEST_F(WaitForLogSavingTest, getInstance)
{
    auto state = WaitForLogSaving::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForLogSavingTest, uninitInstance)
{
    WaitForLogSaving::uninitInstance();
    auto state = WaitForLogSaving::getInstance();
    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
