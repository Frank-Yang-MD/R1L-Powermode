/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock-actions.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "EventHandlingMocks.h"
#include "MockActions.h"
#include "MockAndroidInit.h"
#include "MockNotifyHelper.h"
#include "MockPowerStateHolder.h"
#include "MockSM.h"
#include "MockStateHolder.h"
#include "VpdStateMachine.h"
#include "WaitForVpsStart.h"
#include "WaitForVpsStartComplete.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using namespace testing;
using common::EVpdState;

class WaitForVpsStartTest : public Test {
protected:
    void SetUp() override
    {
        WaitForVpsStart::initInstance(&mMockSM, mMockSubscriptionManager, mMockStateHolder);
        WaitForVpsStartComplete::initInstance(&mMockSM, mMockNotifyHelper, mMockPowerStateHolder,
                                              mMockAndroidInit, mMockVcpuActions);
    }

    MockSubscriptionManager mMockSubscriptionManager;
    MockSM mMockSM;
    MockNotifyHelper mMockNotifyHelper;
    MockAndroidInit mMockAndroidInit;
    MockStateHolder mMockStateHolder;
    MockPowerStateHolder mMockPowerStateHolder;
    MockVcpuActions mMockVcpuActions;
};

/**
 * Check transition from 'WaitForVpsStart' state to 'WaitForVpsStart'
 * at the moment SM is entering 'WaitForVpsStart' state
 */
TEST_F(WaitForVpsStartTest, onEnter)
{
    auto state = WaitForVpsStart::getInstance();

    ON_CALL(mMockSubscriptionManager, getVpsID()).WillByDefault(Return(1));

    EXPECT_CALL(mMockSubscriptionManager, getVpsID()).Times(1);
    ASSERT_NE(WaitForVpsStartComplete::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitForVpsStartComplete::getInstance()));
    EXPECT_CALL(mMockStateHolder, setVpdState(EVpdState::APP_START_COLDBOOT)).Times(1);

    state->onEnter();
}

TEST_F(WaitForVpsStartTest, onEnterFail)
{
    auto state = WaitForVpsStart::getInstance();

    ON_CALL(mMockSubscriptionManager, getVpsID()).WillByDefault(Return(-1));

    EXPECT_CALL(mMockSubscriptionManager, getVpsID()).Times(1);
    EXPECT_CALL(mMockStateHolder, setVpdState(EVpdState::APP_START_COLDBOOT)).Times(1);

    ASSERT_NE(WaitForVpsStartComplete::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitForVpsStartComplete::getInstance())).Times(0);
    state->onEnter();
}

/**
 * Check transition from 'WaitForVpsStart' state to 'WaitForVpsStart'
 * when 'onSubscribeVps' method is called
 */
TEST_F(WaitForVpsStartTest, onSubscribeVps)
{
    auto state = WaitForVpsStart::getInstance();

    ASSERT_NE(WaitForVpsStartComplete::getInstance(), nullptr);
    EXPECT_CALL(mMockSM, transitionTo(WaitForVpsStartComplete::getInstance()));
    state->onSubscribeVps(100);
}

/**
 * Test stateName() method
 */
TEST_F(WaitForVpsStartTest, stateName)
{
    auto state = WaitForVpsStart::getInstance();

    ASSERT_STREQ(state->stateName().c_str(), "WaitForVpsStart");
}

TEST_F(WaitForVpsStartTest, getInstance)
{
    auto state = WaitForVpsStart::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForVpsStartTest, uninitInstance)
{
    WaitForVpsStart::uninitInstance();
    auto state = WaitForVpsStart::getInstance();
    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
