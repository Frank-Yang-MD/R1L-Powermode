/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "LogDaemonBusy.h"
#include "VpdStateMachine.h"
#include "WaitForLogDaemonStop.h"

#include "MockActions.h"
#include "MockNotifyHelper.h"
#include "MockPowerStateHolder.h"
#include "MockSM.h"
#include "MockStateHolder.h"
#include "MockTimeOutControl.h"
#include "MockTimer.h"
#include "MockTimerHandler.h"
#include "VpdClock.h"

using namespace testing;

using ::testing::NiceMock;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class LogDaemonBusyTest : public ::testing::Test {
public:
    LogDaemonBusyTest()
    {
        LogDaemonBusy::initInstance(&mMockSM, mMockLogDaemonActions, mMockCheckTimer,
                                    mMockStopTimer, mMockVcpuActions);
        WaitForLogDaemonStop::initInstance(&mMockSM, mMockNotifyHelper, mMockTimer,
                                           mMockPowerStateHolder, mMockVcpuActions);
    }

    NiceMock<MockSM> mMockSM;
    NiceMock<MockTimer> mMockCheckTimer;
    NiceMock<MockTimer> mMockStopTimer;
    NiceMock<MockLogDaemonActions> mMockLogDaemonActions;
    NiceMock<MockNotifyHelper> mMockNotifyHelper;
    NiceMock<MockTimer> mMockTimer;
    NiceMock<MockTimerHandling> mMockTimerHandling;
    NiceMock<MockVcpuActions> mMockVcpuActions;
    NiceMock<MockStateHolder> mMockStateHolder;
    NiceMock<MockPowerStateHolder> mMockPowerStateHolder;
};

/**
 * Check Transition To state S10(WaitForLogDaemonStop)
 * This should happen only if 'isLogSavingComplete' method returns true, meaning logs are not being
 * saved
 */
TEST_F(LogDaemonBusyTest, onEnter_success)
{
    auto state = LogDaemonBusy::getInstance();

    ON_CALL(mMockLogDaemonActions, isLogSavingComplete()).WillByDefault(Return(true));
    EXPECT_CALL(mMockLogDaemonActions, isLogSavingComplete()).Times(1);
    EXPECT_CALL(mMockCheckTimer, start()).Times(1);
    EXPECT_CALL(mMockStopTimer, start()).Times(1);
    ASSERT_NE(WaitForLogDaemonStop::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitForLogDaemonStop::getInstance())).Times(1);

    state->onEnter();
}

TEST_F(LogDaemonBusyTest, onEnter_fail)
{
    auto state = LogDaemonBusy::getInstance();

    ON_CALL(mMockLogDaemonActions, isLogSavingComplete()).WillByDefault(Return(false));
    EXPECT_CALL(mMockCheckTimer, start()).Times(1);
    EXPECT_CALL(mMockStopTimer, start()).Times(1);
    EXPECT_CALL(mMockLogDaemonActions, isLogSavingComplete()).Times(1);
    ASSERT_NE(WaitForLogDaemonStop::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitForLogDaemonStop::getInstance())).Times(0);

    state->onEnter();
}

TEST_F(LogDaemonBusyTest, onTimeout_timerIsExpired)
{
    auto state = LogDaemonBusy::getInstance();

    ON_CALL(mMockStopTimer, isExpired()).WillByDefault(Return(true));
    EXPECT_CALL(mMockStopTimer, isExpired()).Times(1);

    ASSERT_NE(WaitForLogDaemonStop::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitForLogDaemonStop::getInstance())).Times(1);
    state->onTimeout();
}

TEST_F(LogDaemonBusyTest, must_send_SelfRebootTrue_to_VCPU_when_LogdogSaveState_timer_expires)
{
    auto state = LogDaemonBusy::getInstance();
    ON_CALL(mMockStopTimer, isExpired()).WillByDefault(Return(true));

    EXPECT_CALL(mMockVcpuActions, sendSelfRebootTrue()).Times(1);

    state->onTimeout();
}

TEST_F(LogDaemonBusyTest, onTimeout_logSavingComplete)
{
    auto state = LogDaemonBusy::getInstance();

    ON_CALL(mMockStopTimer, isExpired()).WillByDefault(Return(false));
    EXPECT_CALL(mMockStopTimer, isExpired()).Times(1);

    ON_CALL(mMockCheckTimer, isExpired()).WillByDefault(Return(true));
    EXPECT_CALL(mMockCheckTimer, isExpired()).Times(1);

    ON_CALL(mMockLogDaemonActions, isLogSavingComplete()).WillByDefault(Return(true));
    EXPECT_CALL(mMockLogDaemonActions, isLogSavingComplete()).Times(1);

    ASSERT_NE(WaitForLogDaemonStop::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitForLogDaemonStop::getInstance())).Times(1);
    state->onTimeout();
}

TEST_F(LogDaemonBusyTest, onTimeout_logSavingIncomplete)
{
    auto state = LogDaemonBusy::getInstance();

    ON_CALL(mMockStopTimer, isExpired()).WillByDefault(Return(false));
    EXPECT_CALL(mMockStopTimer, isExpired()).Times(1);

    ON_CALL(mMockCheckTimer, isExpired()).WillByDefault(Return(true));
    EXPECT_CALL(mMockCheckTimer, isExpired()).Times(1);

    ON_CALL(mMockLogDaemonActions, isLogSavingComplete()).WillByDefault(Return(false));
    EXPECT_CALL(mMockLogDaemonActions, isLogSavingComplete()).Times(1);

    ASSERT_NE(WaitForLogDaemonStop::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitForLogDaemonStop::getInstance())).Times(0);
    EXPECT_CALL(mMockCheckTimer, start()).Times(1);
    state->onTimeout();
}

TEST_F(LogDaemonBusyTest, getInstance)
{
    auto state = LogDaemonBusy::getInstance();

    ASSERT_NE(state, nullptr);
}

TEST_F(LogDaemonBusyTest, uninitInstance)
{
    LogDaemonBusy::uninitInstance();
    auto state = LogDaemonBusy::getInstance();

    ASSERT_EQ(state, nullptr);
}

/**
 * Check State Name
 */
TEST_F(LogDaemonBusyTest, stateName)
{
    auto state = LogDaemonBusy::getInstance();

    ASSERT_STREQ(state->stateName().c_str(), "LogDaemonBusy");
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
