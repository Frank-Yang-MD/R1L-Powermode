/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <chrono>
#include <memory>

#include "VpdStateMachine.h"
#include "WaitBeforeSendingFwStopVps.h"
#include "WaitForAppStopVps.h"
#include "WaitForFwStopVps.h"

#include "EVpdState.h"

#include "MockActions.h"
#include "MockAndroidInit.h"
#include "MockNotifyHelper.h"
#include "MockSM.h"
#include "MockStateHolder.h"
#include "MockStateSender.h"
#include "MockTimeOutControl.h"
#include "MockTimer.h"
#include "MockTimerHandler.h"
#include "MockVpdNotificationSender.h"
#include "MockWatchDogTimerHandler.h"

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SetArgReferee;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Test Fixture Object
 */
class WaitForAppStopVpsTest : public ::testing::Test {
public:
    WaitForAppStopVpsTest()
    {
        WaitForAppStopVps::initInstance(&mMockSM, mMockNotifyHelper, mAppStopTimer, mFwStopTimer,
                                        mMockVcpuActions, mMockAndroidInit);
        WaitBeforeSendingFwStopVps::initInstance(&mMockSM, mMockTimer);
        WaitForFwStopVps::initInstance(&mMockSM, mMockNotifyHelper, mAppStopTimer,
                                       mMockVcpuActions);
    }

    ~WaitForAppStopVpsTest();

    NiceMock<MockNotifyHelper> mMockNotifyHelper;
    NiceMock<MockSM> mMockSM;
    NiceMock<MockTimer> mAppStopTimer;
    NiceMock<MockTimer> mFwStopTimer;
    NiceMock<MockTimer> mMockTimer;
    NiceMock<MockWatchDogTimerHandler> mMockWDT;
    NiceMock<MockVcpuActions> mMockVcpuActions;
    NiceMock<MockAndroidInit> mMockAndroidInit;
};

WaitForAppStopVpsTest::~WaitForAppStopVpsTest() {
}  // define empty desctructor in order to supress "out-of-line" definition warning

/**
 * Check method "onEnter" from state S7(WaitForAppStopVps)
 */
TEST_F(WaitForAppStopVpsTest, onEnter)
{
    auto state = WaitForAppStopVps::getInstance();

    // notifySystemChange method has to be called once when onEnter was called
    EXPECT_CALL(mMockNotifyHelper, notifyVps(common::EVpdState::APP_STOP)).Times(1);
    EXPECT_CALL(mAppStopTimer, start()).Times(1);
    EXPECT_CALL(mFwStopTimer, start()).Times(1);
    EXPECT_CALL(mMockVcpuActions, sendSelfRebootTrue()).Times(0);

    // Run Test
    state->onEnter();
}

TEST_F(WaitForAppStopVpsTest, checkForColdbootTrue)
{
    auto state = WaitForAppStopVps::getInstance();
    const std::string true_str("true");

    EXPECT_CALL(mMockAndroidInit, propertyGet(_, _))
        .WillOnce(DoAll(SetArgReferee<1>(true_str), Return(true_str.length())));
    EXPECT_CALL(mMockVcpuActions, sendSelfRebootTrue()).Times(1);

    state->onEnter();
}

/**
 * Check Method "onTimeout" from state(WaitForAppStopVps)
 */
TEST_F(WaitForAppStopVpsTest, onTimeout)
{
    auto state = WaitForAppStopVps::getInstance();

    ON_CALL(mAppStopTimer, isExpired()).WillByDefault(Return(true));
    EXPECT_CALL(mAppStopTimer, isExpired()).Times(1);
    ASSERT_NE(WaitBeforeSendingFwStopVps::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitBeforeSendingFwStopVps::getInstance())).Times(1);

    state->onTimeout();
}

/**
 * Check Method "onTimeout" from state(WaitForAppStopVps)
 * Negative case
 */
TEST_F(WaitForAppStopVpsTest, onTimeout_negative)
{
    auto state = WaitForAppStopVps::getInstance();

    ON_CALL(mAppStopTimer, isExpired()).WillByDefault(Return(false));
    EXPECT_CALL(mAppStopTimer, isExpired()).Times(1);
    ASSERT_NE(WaitBeforeSendingFwStopVps::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitBeforeSendingFwStopVps::getInstance())).Times(0);

    state->onTimeout();
}

TEST_F(WaitForAppStopVpsTest, must_send_SelfRebootTrue_to_VCPU_when_appStopTimer_timer_expires)
{
    auto state = WaitForAppStopVps::getInstance();
    ON_CALL(mAppStopTimer, isExpired()).WillByDefault(Return(true));

    EXPECT_CALL(mMockVcpuActions, sendSelfRebootTrue()).Times(1);

    state->onTimeout();
}

TEST_F(WaitForAppStopVpsTest, transitionTo_WaitForFwStopVps)
{
    auto state = WaitForAppStopVps::getInstance();

    // SM have to change state from WaitForStopVps to WaitForFwStopVps
    // when onStopCompleteVps was called
    ASSERT_NE(WaitForFwStopVps::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitForFwStopVps::getInstance())).Times(1);

    // Run Test
    state->onAppStopCompleteVps();
}

/**
 * Check State Name
 */
TEST_F(WaitForAppStopVpsTest, stateName)
{
    auto state = WaitForAppStopVps::getInstance();

    ASSERT_STREQ(state->stateName().c_str(), "WaitForAppStopVps");
}

TEST_F(WaitForAppStopVpsTest, getInstance)
{
    auto state = WaitForAppStopVps::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForAppStopVpsTest, uninitInstance)
{
    WaitForAppStopVps::uninitInstance();
    auto state = WaitForAppStopVps::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
