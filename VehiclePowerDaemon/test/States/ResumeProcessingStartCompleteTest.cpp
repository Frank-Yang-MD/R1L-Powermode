/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ResumeProcessingStartComplete.h"
#include "WakeUp.h"

#include "MockActions.h"
#include "MockKernelModulesControl.h"
#include "MockPowerStateHolder.h"
#include "MockSM.h"
#include "MockTimerManager.h"
#include "MockWakeUpManager.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::_;

class ResumeProcessingStartCompleteTest : public ::testing::Test {
public:
    ResumeProcessingStartCompleteTest()
    {
        WakeUp::initInstance(&mMockSM, mMockKernelModulesControl, mMockWakeUpManager);
        ResumeProcessingStartComplete::initInstance(&mMockSM, mMockVcpuActions,
                                                    mMockPowerStateHolder, mMockTimerManager);
    }

    MockSM mMockSM;
    MockVcpuActions mMockVcpuActions;
    MockPowerStateHolder mMockPowerStateHolder;
    MockKernelModulesControl mMockKernelModulesControl;
    MockWakeUpManager mMockWakeUpManager;
    MockTimerManager mMockTimerManager;
};

/**
 * Check if 'Start complete' notification is sent to V-CPU on 'ResumeProcessingStartComplete' state
 */
TEST_F(ResumeProcessingStartCompleteTest, onEnter)
{
    auto state = ResumeProcessingStartComplete::getInstance();

    EXPECT_CALL(mMockVcpuActions, sendStartComplete()).Times(1);

    state->onEnter();
}

TEST_F(ResumeProcessingStartCompleteTest, onExit)
{
    auto state = ResumeProcessingStartComplete::getInstance();

    EXPECT_CALL(mMockTimerManager, changeTimerHandler(ETimerType::WDTRTimer)).Times(1);

    state->onExit();
}

/**
 * Check transition to 'WakeUp' state.
 */
TEST_F(ResumeProcessingStartCompleteTest, onResumeProcessingStartCompleteSentSuccess)
{
    auto state = ResumeProcessingStartComplete::getInstance();

    EXPECT_CALL(mMockPowerStateHolder, onPowerStateChange(PowerState::NORMAL)).Times(1);
    EXPECT_CALL(mMockSM, transitionTo(WakeUp::getInstance())).Times(1);

    state->onResumeProcessingStartCompleteSentSuccess();
}

/**
 * Check if 'Start complete' notification is sent to V-CPU
 */
TEST_F(ResumeProcessingStartCompleteTest, onResumeProcessingStartCompleteSentFailure)
{
    auto state = ResumeProcessingStartComplete::getInstance();

    EXPECT_CALL(mMockVcpuActions, sendStartComplete()).Times(1);

    state->onResumeProcessingStartCompleteSentFailure();
}

/**
 * Check State Name
 */
TEST_F(ResumeProcessingStartCompleteTest, stateName)
{
    auto state = ResumeProcessingStartComplete::getInstance();

    ASSERT_STREQ(state->stateName().c_str(), "ResumeProcessingStartComplete");
}

TEST_F(ResumeProcessingStartCompleteTest, getInstance)
{
    auto state = ResumeProcessingStartComplete::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(ResumeProcessingStartCompleteTest, uninitInstance)
{
    ResumeProcessingStartComplete::uninitInstance();
    auto state = ResumeProcessingStartComplete::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
