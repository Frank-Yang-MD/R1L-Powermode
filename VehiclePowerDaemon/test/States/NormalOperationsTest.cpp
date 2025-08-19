/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <chrono>
#include <gtest/gtest.h>

#include "NormalOperation.h"
#include "WaitForAppStopVps.h"
#include "WaitForVpsStart.h"

#include "EventHandlingMocks.h"
#include "MockAndroidInit.h"
#include "MockISubscriptionManager.h"
#include "MockNotifyHelper.h"
#include "MockPowerStateHolder.h"
#include "MockSM.h"
#include "MockStateHolder.h"
#include "MockTimeOutControl.h"
#include "MockTimerHandler.h"
#include "MockVcpuInteraction.h"
#include "MockVpdRestartHolder.h"
#include "VpdStateMachineContext.h"

using namespace testing;

using ::testing::NiceMock;
using ::testing::ReturnRef;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class NormalOperationTest : public Test {
protected:
    void SetUp() override
    {
        WaitForAppStopVps::initInstance(&mMockSM, mMockNotifyHelper, mStopVpsTimer, mStopVpsTimer,
                                        mMockVcpuInteraction, mMockAndroidInit);
        NormalOperation::initInstance(&mMockSM, mMockPowerStateHolder, mMockVpdRestartHolder,
                                      mMockStateHolder);
        WaitForVpsStart::initInstance(&mMockSM, mMockISubscriptionManager, mMockStateHolder);
        ON_CALL(mMockSM, context()).WillByDefault(ReturnRef(mContext));
    }

    VpdStateMachineContext mContext;
    MockAndroidInit mMockAndroidInit;
    MockSM mMockSM;
    MockNotifyHelper mMockNotifyHelper;
    MockTimer mStopVpsTimer;
    MockVcpuInteraction mMockVcpuInteraction;
    NiceMock<MockPowerStateHolder> mMockPowerStateHolder;
    MockVpdRestartHolder mMockVpdRestartHolder;
    MockStateHolder mMockStateHolder;
    NiceMock<MockISubscriptionManager> mMockISubscriptionManager;
};

/**
 * Check transition from 'NormalOperation' to 'WaitForStopVps'
 * after receiving 'Suspend request' from VCPU
 */
TEST_F(NormalOperationTest, onPowerStateChangeShutdownState)
{
    auto state = NormalOperation::getInstance();

    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(mMockSM, transitionTo(WaitForAppStopVps::getInstance())).Times(1);

    state->onPowerStateChange(PowerState::SHUTDOWN);
}

/**
 * Check transition from 'NormalOperation' to 'WaitForStopVps'
 * after receiving 'Suspend request' from VCPU
 */
TEST_F(NormalOperationTest, onPowerStateChangeNotShutdownState)
{
    auto state = NormalOperation::getInstance();

    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).Times(1).WillOnce(Return(false));
    EXPECT_CALL(mMockSM, transitionTo(WaitForAppStopVps::getInstance())).Times(0);

    state->onPowerStateChange(PowerState::SUSPEND_CANCEL);
}

TEST_F(NormalOperationTest, getInstance)
{
    auto state = NormalOperation::getInstance();

    ASSERT_NE(state, nullptr);
}

TEST_F(NormalOperationTest, uninitInstance)
{
    NormalOperation::uninitInstance();
    auto state = NormalOperation::getInstance();

    ASSERT_EQ(state, nullptr);
}

/**
 * Test stateName() method
 */
TEST_F(NormalOperationTest, stateName)
{
    auto state = NormalOperation::getInstance();

    ASSERT_STREQ(state->stateName().c_str(), "NormalOperation");
}

TEST_F(NormalOperationTest, onEnter)
{
    auto state = NormalOperation::getInstance();

    EXPECT_CALL(mMockStateHolder, setVpdState(common::EVpdState::APP_START_COLDBOOT)).Times(1);
    EXPECT_CALL(mMockPowerStateHolder, onPowerStateChange(PowerState::NORMAL)).Times(1);
    EXPECT_CALL(mMockVpdRestartHolder, vpdStarted()).Times(1);

    state->onEnter();
}

TEST_F(NormalOperationTest, onDisconnectVps)
{
    auto state = NormalOperation::getInstance();
    mContext.setVpsDisconnected(false);

    EXPECT_CALL(mMockSM, transitionTo(WaitForVpsStart::getInstance())).Times(1);

    state->onDisconnectVps();

    ASSERT_TRUE(mContext.isVpsDisconnected());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
