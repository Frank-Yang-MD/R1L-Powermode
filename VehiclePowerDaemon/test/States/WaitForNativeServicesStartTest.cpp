/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <chrono>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <thread>

#include "VpdClock.h"
#include "WaitForNativeServicesStart.h"
#include "WaitForVpsStart.h"

#include "EventHandlingMocks.h"
#include "MockActions.h"
#include "MockNativeRunner.h"
#include "MockNotifyHelper.h"
#include "MockSM.h"
#include "MockStateHolder.h"
#include "MockTimeOutControl.h"
#include "MockTimer.h"
#include "MockTimerManager.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::AtLeast;
using ::testing::InSequence;
using ::testing::Return;

using common::EVpdState;

class WaitForNativeServicesStartTest : public ::testing::Test {
public:
    WaitForNativeServicesStartTest()
    {
        WaitForNativeServicesStart::initInstance(&mMockSM, mMockVcpuActions, mMockNativeRunner,
                                                 mMockCheckServicesTimer, mMockTimerManager,
                                                 mMockNotifyHelper);
        WaitForVpsStart::initInstance(&mMockSM, mMockSubscriptionManager, mMockStateHolder);
    }

    MockSM mMockSM;
    MockVcpuActions mMockVcpuActions;
    MockTimer mMockCheckServicesTimer;
    MockNativeRunner mMockNativeRunner;
    MockSubscriptionManager mMockSubscriptionManager;
    MockNotifyHelper mMockNotifyHelper;
    MockStateHolder mMockStateHolder;
    MockTimerManager mMockTimerManager;
};

/**
    Check transition from 'WaitForNativeServicesStart' state to 'WaitForVpsStart'
    when 'onTimeout' method is called
*/
TEST_F(WaitForNativeServicesStartTest, onTimeoutSuccess)
{
    auto state = WaitForNativeServicesStart::getInstance();

    ON_CALL(mMockCheckServicesTimer, isExpired()).WillByDefault(Return(true));
    ON_CALL(mMockNativeRunner, isDone()).WillByDefault(Return(true));

    EXPECT_CALL(mMockNativeRunner, isDone()).Times(1);
    EXPECT_CALL(mMockCheckServicesTimer, isExpired()).Times(1);
    ASSERT_NE(WaitForVpsStart::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitForVpsStart::getInstance()));

    state->onTimeout();
}

TEST_F(WaitForNativeServicesStartTest, onTimeoutFail)
{
    auto state = WaitForNativeServicesStart::getInstance();

    ON_CALL(mMockCheckServicesTimer, isExpired()).WillByDefault(Return(true));
    ON_CALL(mMockNativeRunner, isDone()).WillByDefault(Return(false));

    EXPECT_CALL(mMockCheckServicesTimer, start()).Times(1);
    EXPECT_CALL(mMockNativeRunner, isDone()).Times(1);
    EXPECT_CALL(mMockCheckServicesTimer, isExpired()).Times(1);

    ASSERT_NE(WaitForVpsStart::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitForVpsStart::getInstance())).Times(0);

    state->onTimeout();
}

TEST_F(WaitForNativeServicesStartTest, onTimeoutNotExpired)
{
    auto state = WaitForNativeServicesStart::getInstance();

    EXPECT_CALL(mMockCheckServicesTimer, isExpired()).WillOnce(Return(false));
    EXPECT_CALL(mMockNativeRunner, isDone()).Times(0);

    state->onTimeout();
}

TEST_F(WaitForNativeServicesStartTest, onExit)
{
    InSequence seq;

    auto state = WaitForNativeServicesStart::getInstance();
    EXPECT_CALL(mMockVcpuActions, sendStartComplete()).Times(1);
    EXPECT_CALL(mMockTimerManager, changeTimerHandler(ETimerType::WDTRTimer)).Times(1);
    EXPECT_CALL(mMockNotifyHelper, notifyAllNative(EVpdState::APP_START_COLDBOOT)).Times(1);

    state->onExit();
}

TEST_F(WaitForNativeServicesStartTest, onEnter)
{
    auto state = WaitForNativeServicesStart::getInstance();
    EXPECT_CALL(mMockNativeRunner, run()).Times(1);
    EXPECT_CALL(mMockCheckServicesTimer, start()).Times(1);

    state->onEnter();
}

/**
 * Check State Name
 */
TEST_F(WaitForNativeServicesStartTest, stateName)
{
    auto state = WaitForNativeServicesStart::getInstance();

    ASSERT_STREQ(state->stateName().c_str(), "WaitForNativeServicesStart");
}

TEST_F(WaitForNativeServicesStartTest, getInstance)
{
    auto state = WaitForNativeServicesStart::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForNativeServicesStartTest, uninitInstance)
{
    WaitForNativeServicesStart::uninitInstance();
    auto state = WaitForNativeServicesStart::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
