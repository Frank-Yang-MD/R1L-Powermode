/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <memory>

#include "VpdStateMachine.h"
#include "WaitBeforeSendingAppStopNativeServices.h"
#include "WaitForFwStopVps.h"
#include "WaitNativeAppStopProcessingComplete.h"

#include "EVpdState.h"

#include "MockActions.h"
#include "MockNotifyHelper.h"
#include "MockSM.h"
#include "MockStateHolder.h"
#include "MockStateSender.h"
#include "MockTimeOutControl.h"
#include "MockTimer.h"
#include "MockTimerHandler.h"
#include "MockVpdNotificationSender.h"
#include "MockWatchDogTimerHandler.h"

using ::testing::NiceMock;
using ::testing::Return;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Test Fixture Object
 */
class WaitForFwStopVpsTest : public ::testing::Test {
public:
    WaitForFwStopVpsTest()
    {
        WaitForFwStopVps::initInstance(&mMockSM, mMockNotifyHelper, mStopVpsTimer,
                                       mMockVcpuActions);
        WaitBeforeSendingAppStopNativeServices::initInstance(&mMockSM, mMockTimer);
        WaitNativeAppStopProcessingComplete::initInstance(&mMockSM, mMockNotifyHelper, mMockTimer,
                                                          mSubscriptionManager, mMockStateHolder,
                                                          mMockVcpuActions);
    }

    ~WaitForFwStopVpsTest();

    NiceMock<MockNotifyHelper> mMockNotifyHelper;
    NiceMock<MockSM> mMockSM;
    NiceMock<MockTimer> mStopVpsTimer;
    NiceMock<MockSubscriptionManager> mSubscriptionManager;
    NiceMock<MockTimer> mMockTimer;
    NiceMock<MockStateHolder> mMockStateHolder;
    NiceMock<MockVcpuActions> mMockVcpuActions;
};

WaitForFwStopVpsTest::~WaitForFwStopVpsTest() {
}  // define empty desctructor in order to supress "out-of-line" definition warning

/**
 * Check method "onEnter" from state S7(WaitForFwStopVps)
 */
TEST_F(WaitForFwStopVpsTest, onEnter)
{
    auto state = WaitForFwStopVps::getInstance();

    // notifySystemChange method has to be called once when onEnter was called
    EXPECT_CALL(mMockNotifyHelper, notifyVps(common::EVpdNotifications::FwStop)).Times(1);

    // Run Test
    state->onEnter();
}

/**
 * Check Method "onTimeout" from state(WaitForFwStopVps)
 */
TEST_F(WaitForFwStopVpsTest, onTimeout)
{
    auto state = WaitForFwStopVps::getInstance();

    ON_CALL(mStopVpsTimer, isExpired()).WillByDefault(Return(true));
    EXPECT_CALL(mStopVpsTimer, isExpired()).Times(1);
    ASSERT_NE(WaitBeforeSendingAppStopNativeServices::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitBeforeSendingAppStopNativeServices::getInstance()))
        .Times(1);

    state->onTimeout();
}

/**
 * Check Method "onTimeout" from state(WaitForFwStopVps)
 * Negative case
 */
TEST_F(WaitForFwStopVpsTest, onTimeout_negative)
{
    auto state = WaitForFwStopVps::getInstance();

    ON_CALL(mStopVpsTimer, isExpired()).WillByDefault(Return(false));
    EXPECT_CALL(mStopVpsTimer, isExpired()).Times(1);
    ASSERT_NE(WaitBeforeSendingAppStopNativeServices::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitBeforeSendingAppStopNativeServices::getInstance()))
        .Times(0);

    state->onTimeout();
}

TEST_F(WaitForFwStopVpsTest, must_send_SelfRebootTrue_to_VCPU_when_stopVpsTimer_timer_expires)
{
    auto state = WaitForFwStopVps::getInstance();
    ON_CALL(mStopVpsTimer, isExpired()).WillByDefault(Return(true));

    EXPECT_CALL(mMockVcpuActions, sendSelfRebootTrue()).Times(1);

    state->onTimeout();
}

TEST_F(WaitForFwStopVpsTest, must_send_SelfRebootTrue_to_VCPU_in_onStopFailedVps_method)
{
    auto state = WaitForFwStopVps::getInstance();

    EXPECT_CALL(mMockVcpuActions, sendSelfRebootTrue()).Times(1);
    EXPECT_CALL(mMockSM, transitionTo(WaitNativeAppStopProcessingComplete::getInstance())).Times(1);

    state->onStopFailedVps();
}

/**
 * check transition to state S8(WaitNativeAppStopProcessingComplete)
 */
TEST_F(WaitForFwStopVpsTest, transitionTo_WaitNativeAppStopProcessingComplete)
{
    auto state = WaitForFwStopVps::getInstance();

    // SM have to change state from WaitForStopVps to WaitNativeAppStopProcessingComplete
    // when onStopCompleteVps was called
    ASSERT_NE(WaitNativeAppStopProcessingComplete::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitNativeAppStopProcessingComplete::getInstance())).Times(1);

    // Run Test
    state->onFwStopCompleteVps();
}

/**
 * Check State Name
 */
TEST_F(WaitForFwStopVpsTest, stateName)
{
    auto state = WaitForFwStopVps::getInstance();

    ASSERT_STREQ(state->stateName().c_str(), "WaitForFwStopVps");
}

TEST_F(WaitForFwStopVpsTest, getInstance)
{
    auto state = WaitForFwStopVps::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForFwStopVpsTest, uninitInstance)
{
    WaitForFwStopVps::uninitInstance();
    auto state = WaitForFwStopVps::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
