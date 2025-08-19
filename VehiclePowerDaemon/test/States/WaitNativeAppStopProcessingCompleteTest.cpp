/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <memory>

#include "LogDaemonBusy.h"
#include "VpdStateMachine.h"
#include "WaitForLogSaving.h"
#include "WaitNativeAppStopProcessingComplete.h"

#include "EventHandlingMocks.h"
#include "MockActions.h"
#include "MockNotifyHelper.h"
#include "MockSM.h"
#include "MockStateHolder.h"
#include "MockTimeOutControl.h"
#include "MockTimer.h"
#include "MockTimerHandler.h"
#include "Timer.h"

using namespace std;
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
class WaitNativeAppStopProcessingCompleteTest : public ::testing::Test {
public:
protected:
    void SetUp()
    {
        EXPECT_CALL(mSubscriptionManager, getNativeSubscribers())
            .WillRepeatedly(Return(mSubscribers));

        LogDaemonBusy::initInstance(&mMockSM, mMockLogDaemonActions, mMockCheckTimer,
                                    mMockStopTimer, mMockVcpuActions);
        WaitForLogSaving::initInstance(&mMockSM, mMockTimer);
        WaitNativeAppStopProcessingComplete::initInstance(&mMockSM, mMockNotifyHelper, mMockTimer,
                                                          mSubscriptionManager, mMockStateHolder,
                                                          mMockVcpuActions);

        ON_CALL(mSubscriptionManager, getVpsID()).WillByDefault(Return(123));
        ON_CALL(mSubscriptionManager, getLogID()).WillByDefault(Return(124));
        ON_CALL(mSubscriptionManager, getBootDaemonID()).WillByDefault(Return(125));
    }

    NiceMock<MockSM> mMockSM;
    NiceMock<MockNotifyHelper> mMockNotifyHelper;
    NiceMock<MockSubscriptionManager> mSubscriptionManager;
    NiceMock<MockTimer> mMockTimer;
    NiceMock<MockStateHolder> mMockStateHolder;
    NiceMock<MockTimer> mMockCheckTimer;
    NiceMock<MockTimer> mMockStopTimer;
    NiceMock<MockLogDaemonActions> mMockLogDaemonActions;
    NiceMock<MockVcpuActions> mMockVcpuActions;

    std::vector<int> mSubscribers = {5, 2, 9, 8, 4};
};

/**
 * Check method "onEnter" from state S7(WaitNativeAppStopProcessingComplete)
 */
TEST_F(WaitNativeAppStopProcessingCompleteTest, onEnter)
{
    auto state = WaitNativeAppStopProcessingComplete::getInstance();

    // notifySystemChange have to be called once when onEnter was called
    EXPECT_CALL(mMockNotifyHelper, notifyAllNative(EVpdState::APP_STOP)).Times(1);

    EXPECT_CALL(mMockTimer, start()).Times(1);
    EXPECT_CALL(mMockStateHolder, setVpdState(EVpdState::APP_STOP)).Times(1);
    EXPECT_CALL(mSubscriptionManager, getVpsID()).Times(0);
    EXPECT_CALL(mSubscriptionManager, getLogID()).Times(0);
    EXPECT_CALL(mSubscriptionManager, getBootDaemonID()).Times(0);

    // Run Test
    state->onEnter();
}

TEST_F(WaitNativeAppStopProcessingCompleteTest, onEnterNoSubscribers)
{
    mSubscribers.clear();
    EXPECT_CALL(mSubscriptionManager, getNativeSubscribers()).WillRepeatedly(Return(mSubscribers));

    auto state = WaitNativeAppStopProcessingComplete::getInstance();

    // notifySystemChange have to be called once when onEnter was called
    EXPECT_CALL(mMockNotifyHelper, notifyAllNative(EVpdState::APP_STOP)).Times(1);

    EXPECT_CALL(mMockStateHolder, setVpdState(EVpdState::APP_STOP)).Times(1);
    EXPECT_CALL(mSubscriptionManager, getVpsID()).Times(0);
    EXPECT_CALL(mSubscriptionManager, getLogID()).Times(0);
    EXPECT_CALL(mSubscriptionManager, getBootDaemonID()).Times(0);

    ASSERT_NE(LogDaemonBusy::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(LogDaemonBusy::getInstance())).Times(1);
    // Run Test
    state->onEnter();
}

/**
 * Check method "onTimeout" from state S7 (WaitNativeAppStopProcessingComplete)
 */
TEST_F(WaitNativeAppStopProcessingCompleteTest, onTimeout)
{
    auto state = WaitNativeAppStopProcessingComplete::getInstance();
    std::vector<int> testVec;

    ON_CALL(mMockTimer, isExpired()).WillByDefault(Return(true));
    ON_CALL(mSubscriptionManager, getNativeSubscribers()).WillByDefault(Return(testVec));
    EXPECT_CALL(mMockTimer, isExpired()).Times(1);
    EXPECT_CALL(mSubscriptionManager, getNativeSubscribers()).Times(1);
    ASSERT_NE(WaitForLogSaving::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitForLogSaving::getInstance())).Times(1);

    // Run Test
    state->onTimeout();
}

/**
 * Check method "onTimeout" from state S7 (WaitNativeAppStopProcessingComplete)
 * negative case
 */
TEST_F(WaitNativeAppStopProcessingCompleteTest, onTimeoutNegative)
{
    auto state = WaitNativeAppStopProcessingComplete::getInstance();

    ON_CALL(mMockTimer, isExpired()).WillByDefault(Return(false));
    EXPECT_CALL(mMockTimer, isExpired()).Times(1);
    ASSERT_NE(WaitForLogSaving::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitForLogSaving::getInstance())).Times(0);

    state->onTimeout();
}

TEST_F(WaitNativeAppStopProcessingCompleteTest,
       must_send_SelfRebootTrue_to_VCPU_when_stopNativeTimer_timer_expires)
{
    auto state = WaitNativeAppStopProcessingComplete::getInstance();
    ON_CALL(mMockTimer, isExpired()).WillByDefault(Return(true));

    EXPECT_CALL(mMockVcpuActions, sendSelfRebootTrue()).Times(1);

    state->onTimeout();
}

/**
 * check transition to state S7(WaitNativeAppStopProcessingComplete)
 */
TEST_F(WaitNativeAppStopProcessingCompleteTest, onStopCompleteNative)
{
    auto state = WaitNativeAppStopProcessingComplete::getInstance();

    // SM has to change state from WaitNativeAppStopProcessingComplete to LogDaemonBusy
    // if 'onStopCompleteNative' method has been called as many times as there are native subscribes
    ASSERT_NE(LogDaemonBusy::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(LogDaemonBusy::getInstance())).Times(1);
    EXPECT_CALL(mSubscriptionManager, getVpsID()).Times(0);
    EXPECT_CALL(mSubscriptionManager, getLogID()).Times(0);
    EXPECT_CALL(mSubscriptionManager, getBootDaemonID()).Times(0);

    // Run Test
    for (unsigned int i = 0; i < mSubscribers.size(); i++) {
        state->onStopCompleteNative(mSubscribers[i]);
    }
}

/**
 * check transition to state S7(WaitNativeAppStopProcessingComplete)
 * negative case
 */
TEST_F(WaitNativeAppStopProcessingCompleteTest, onStopCompleteNativeNegative)
{
    auto state = WaitNativeAppStopProcessingComplete::getInstance();

    // SM should not change state from WaitNativeAppStopProcessingComplete to LogDaemonBusy
    // if not all native subscribers were stopped
    ASSERT_NE(LogDaemonBusy::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(LogDaemonBusy::getInstance())).Times(0);
    EXPECT_CALL(mSubscriptionManager, getVpsID()).Times(0);
    EXPECT_CALL(mSubscriptionManager, getLogID()).Times(0);
    EXPECT_CALL(mSubscriptionManager, getBootDaemonID()).Times(0);

    // Run Test
    for (unsigned int i = 0; i < mSubscribers.size() - 1; i++) {
        state->onStopCompleteNative(mSubscribers[i]);
    }
}

/**
 * check state name
 */
TEST_F(WaitNativeAppStopProcessingCompleteTest, stateName)
{
    auto state = WaitNativeAppStopProcessingComplete::getInstance();

    ASSERT_STREQ(state->stateName().c_str(), "WaitNativeAppStopProcessingComplete");
}

TEST_F(WaitNativeAppStopProcessingCompleteTest, getInstance)
{
    auto state = WaitNativeAppStopProcessingComplete::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitNativeAppStopProcessingCompleteTest, uninitInstance)
{
    WaitNativeAppStopProcessingComplete::uninitInstance();
    auto state = WaitNativeAppStopProcessingComplete::getInstance();
    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
