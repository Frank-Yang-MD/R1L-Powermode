/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "RestartNative.h"
#include "VpdStateMachine.h"
#include "WaitForLogDaemonStop.h"
#include "WaitForUnmount.h"

#include "EVpdState.h"

#include "MockActions.h"
#include "MockNotifyHelper.h"
#include "MockPowerStateHolder.h"
#include "MockSM.h"
#include "MockTimer.h"
#include <MockVcpuInteraction.h>

using ::testing::NiceMock;
using ::testing::Return;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class WaitForLogDaemonStopTest : public ::testing::Test {
public:
    WaitForLogDaemonStopTest()
    {
        WaitForLogDaemonStop::initInstance(&mMockSM, mMockNotifyHelper, mMockTimer,
                                           mMockPowerStateHolder, mMockVcpuActions);

        WaitForUnmount::initInstance(&mMockSM, mMockNotifyHelper, mMockTimer, mMockVcpuInteration);
    }

    NiceMock<MockSM> mMockSM;
    NiceMock<MockNotifyHelper> mMockNotifyHelper;
    NiceMock<MockTimer> mMockTimer;
    NiceMock<MockVcpuActions> mMockVcpuActions;
    MockVcpuInteraction mMockVcpuInteration;
    NiceMock<MockPowerStateHolder> mMockPowerStateHolder;
};

/**
 * Check if 'notifyLog' method is called on 'WaitForLogDaemonStop' state enter
 */
TEST_F(WaitForLogDaemonStopTest, onEnter)
{
    auto state = WaitForLogDaemonStop::getInstance();

    EXPECT_CALL(mMockNotifyHelper, notifyLog(common::EVpdState::APP_STOP)).Times(1);
    EXPECT_CALL(mMockTimer, start()).Times(1);

    // Run Test
    state->onEnter();
}

/**
 * Check transition to state S11(Stop) after receiving 'Stop complete' notification from
 * LogDaemon
 */
TEST_F(WaitForLogDaemonStopTest, onStopCompleteLogShutdownState)
{
    auto state = WaitForLogDaemonStop::getInstance();

    // SM have to change state from WaitForLogDaemonStop to Stop
    // when onStopCompleteLog was called
    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(mMockSM, transitionTo(WaitForUnmount::getInstance())).Times(1);

    state->onStopCompleteLog();
}

TEST_F(WaitForLogDaemonStopTest, onStopCompleteLogNotShutdownState)
{
    auto state = WaitForLogDaemonStop::getInstance();

    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).Times(1).WillOnce(Return(false));
    EXPECT_CALL(mMockSM, transitionTo(RestartNative::getInstance())).Times(1);

    state->onStopCompleteLog();
}

TEST_F(WaitForLogDaemonStopTest, onTimeoutSuccessShutdownState)
{
    auto state = WaitForLogDaemonStop::getInstance();

    ON_CALL(mMockTimer, isExpired()).WillByDefault(Return(true));

    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(mMockTimer, isExpired()).Times(1);
    EXPECT_CALL(mMockSM, transitionTo(WaitForUnmount::getInstance())).Times(1);
    EXPECT_CALL(mMockVcpuActions, sendSelfRebootTrue()).Times(1);

    state->onTimeout();
}

TEST_F(WaitForLogDaemonStopTest, onTimeoutSuccessNotShutdownState)
{
    auto state = WaitForLogDaemonStop::getInstance();

    ON_CALL(mMockTimer, isExpired()).WillByDefault(Return(true));

    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).Times(1).WillOnce(Return(false));
    EXPECT_CALL(mMockTimer, isExpired()).Times(1);
    EXPECT_CALL(mMockSM, transitionTo(RestartNative::getInstance())).Times(1);
    EXPECT_CALL(mMockVcpuActions, sendSelfRebootTrue()).Times(1);

    state->onTimeout();
}

TEST_F(WaitForLogDaemonStopTest, onTimeoutFail)
{
    auto state = WaitForLogDaemonStop::getInstance();

    ON_CALL(mMockTimer, isExpired()).WillByDefault(Return(false));
    EXPECT_CALL(mMockTimer, isExpired()).Times(1);
    EXPECT_CALL(mMockSM, transitionTo(WaitForUnmount::getInstance())).Times(0);

    state->onTimeout();
}

TEST_F(WaitForLogDaemonStopTest,
       must_send_SelfRebootTrue_to_VCPU_when_logdogStopTimer_timer_expires)
{
    auto state = WaitForLogDaemonStop::getInstance();
    ON_CALL(mMockTimer, isExpired()).WillByDefault(Return(true));

    EXPECT_CALL(mMockVcpuActions, sendSelfRebootTrue()).Times(1);

    state->onTimeout();
}

/**
 * Check State Name
 */
TEST_F(WaitForLogDaemonStopTest, stateName)
{
    auto state = WaitForLogDaemonStop::getInstance();

    ASSERT_STREQ(state->stateName().c_str(), "WaitForLogDaemonStop");
}

TEST_F(WaitForLogDaemonStopTest, getInstance)
{
    auto state = WaitForLogDaemonStop::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForLogDaemonStopTest, uninitInstance)
{
    WaitForLogDaemonStop::uninitInstance();
    auto state = WaitForLogDaemonStop::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
