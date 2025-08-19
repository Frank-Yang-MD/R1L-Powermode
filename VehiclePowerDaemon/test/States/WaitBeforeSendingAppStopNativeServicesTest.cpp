/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "VpdStateMachine.h"
#include "WaitBeforeSendingAppStopNativeServices.h"
#include "WaitNativeAppStopProcessingComplete.h"

#include "EventHandlingMocks.h"
#include "MockActions.h"
#include "MockNotifyHelper.h"
#include "MockSM.h"
#include "MockStateHolder.h"
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
class WaitBeforeSendingAppStopNativeServicesTest : public ::testing::Test {
public:
protected:
    void SetUp()
    {
        WaitNativeAppStopProcessingComplete::initInstance(&mMockSM, mMockNotifyHelper, mMockTimer,
                                                          mSubscriptionManager, mMockStateHolder,
                                                          mMockVcpuActions);
        WaitBeforeSendingAppStopNativeServices::initInstance(&mMockSM, mMockTimer);
    }

    NiceMock<MockSM> mMockSM;
    NiceMock<MockNotifyHelper> mMockNotifyHelper;
    NiceMock<MockSubscriptionManager> mSubscriptionManager;
    NiceMock<MockTimer> mMockTimer;
    NiceMock<MockStateHolder> mMockStateHolder;
    NiceMock<MockVcpuActions> mMockVcpuActions;
};

/**
 * Check method "onEnter" from state (WaitBeforeSendingAppStopNativeServices)
 */
TEST_F(WaitBeforeSendingAppStopNativeServicesTest, onEnter)
{
    auto state = WaitBeforeSendingAppStopNativeServices::getInstance();

    EXPECT_CALL(mMockTimer, start()).Times(1);

    // Run Test
    state->onEnter();
}

/**
 * Check method "onTimeout" from state (WaitBeforeSendingAppStopNativeServices)
 */
TEST_F(WaitBeforeSendingAppStopNativeServicesTest, onTimeout)
{
    auto state = WaitBeforeSendingAppStopNativeServices::getInstance();

    ON_CALL(mMockTimer, isExpired()).WillByDefault(Return(true));
    EXPECT_CALL(mMockTimer, isExpired()).Times(1);
    ASSERT_NE(WaitNativeAppStopProcessingComplete::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitNativeAppStopProcessingComplete::getInstance())).Times(1);

    // Run Test
    state->onTimeout();
}

/**
 * Check method "onTimeout" from state (WaitBeforeSendingAppStopNativeServices)
 * negative case
 */
TEST_F(WaitBeforeSendingAppStopNativeServicesTest, onTimeoutNegative)
{
    auto state = WaitBeforeSendingAppStopNativeServices::getInstance();

    ON_CALL(mMockTimer, isExpired()).WillByDefault(Return(false));
    EXPECT_CALL(mMockTimer, isExpired()).Times(1);
    ASSERT_NE(WaitNativeAppStopProcessingComplete::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitNativeAppStopProcessingComplete::getInstance())).Times(0);

    // Run Test
    state->onTimeout();
}

/**
 * check state name
 */
TEST_F(WaitBeforeSendingAppStopNativeServicesTest, stateName)
{
    auto state = WaitBeforeSendingAppStopNativeServices::getInstance();

    ASSERT_STREQ(state->stateName().c_str(), "WaitBeforeSendingAppStopNativeServices");
}

TEST_F(WaitBeforeSendingAppStopNativeServicesTest, getInstance)
{
    auto state = WaitBeforeSendingAppStopNativeServices::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitBeforeSendingAppStopNativeServicesTest, uninitInstance)
{
    WaitBeforeSendingAppStopNativeServices::uninitInstance();
    auto state = WaitBeforeSendingAppStopNativeServices::getInstance();
    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
