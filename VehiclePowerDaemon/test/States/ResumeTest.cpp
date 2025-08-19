/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "NormalOperation.h"
#include "Resume.h"
#include "WaitForAppStopVps.h"

#include "MockActions.h"
#include "MockAndroidInit.h"
#include "MockNotifyHelper.h"
#include "MockPowerStateHolder.h"
#include "MockSM.h"
#include "MockStateHolder.h"
#include "MockTimeOutControl.h"
#include "MockTimerHandler.h"
#include "MockVcpuInteraction.h"
#include "MockVpdRestartHolder.h"

using namespace testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class ResumeTest : public Test {
protected:
    void SetUp() override
    {
        Resume::initInstance(&mMockSM, mMockPowerStateHolder);
        NormalOperation::initInstance(&mMockSM, mMockPowerStateHolder, mMockVpdRestartHolder,
                                      mMockStateHolder);
        WaitForAppStopVps::initInstance(&mMockSM, mMockNotifyHelper, mStopVpsTimer, mStopVpsTimer,
                                        mMockVcpuInteraction, mMockAndroidInit);
    }
    MockVcpuActions mMockVcpuActions;
    MockNotifyHelper mMockNotifyHelper;
    MockSM mMockSM;
    MockTimer mStopVpsTimer;
    MockStateHolder mMockStateHolder;
    MockVcpuInteraction mMockVcpuInteraction;
    MockPowerStateHolder mMockPowerStateHolder;
    MockVpdRestartHolder mMockVpdRestartHolder;
    MockAndroidInit mMockAndroidInit;
};

/**
 * Check if 'Start complete' notification is sent to V-CPU on 'Resume' state enter.
 * Check if a notification about System state (Resume) change is sent to Vehicle Power Service
 * and Native services at the moment SM is entering 'Resume' state.
 * After that, check transition to 'NormalOperation' state.
 * Check if a notification about start completion is sent to EarlyHMI during transition
 */
TEST_F(ResumeTest, onPowerStateChangeShutdownState)
{
    auto state = Resume::getInstance();

    ASSERT_NE(WaitForAppStopVps::getInstance(), nullptr);
    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(mMockSM, transitionTo(WaitForAppStopVps::getInstance())).Times(1);
    state->onEnter();
}

TEST_F(ResumeTest, onPowerStateChangeNotShutdownState)
{
    auto state = Resume::getInstance();

    ASSERT_NE(NormalOperation::getInstance(), nullptr);
    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).Times(1).WillOnce(Return(false));
    EXPECT_CALL(mMockSM, transitionTo(NormalOperation::getInstance())).Times(1);
    state->onEnter();
}

TEST_F(ResumeTest, stateName)
{
    auto state = Resume::getInstance();
    ASSERT_STREQ(state->stateName().c_str(), "Resume");
}

TEST_F(ResumeTest, getInstance)
{
    auto state = Resume::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(ResumeTest, uninitInstance)
{
    Resume::uninitInstance();
    auto state = Resume::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
