/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "SendShutdownProcessingStart.h"
#include "MockActions.h"
#include "MockAndroidInit.h"
#include "MockPowerStateHolder.h"
#include "MockSM.h"
#include "MockTimerManager.h"
#include "MockWakeUpEventDetector.h"
#include "PollingWakeUp.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::_;
using ::testing::InSequence;

class SendShutdownProcessingStartTest : public ::testing::Test {
public:
    SendShutdownProcessingStartTest()
    {
        PollingWakeUp::initInstance(&mMockSM, mMockWakeUpEventDetector, mMockWakeLockManager,
                                    mMockShutdownAction);
        SendShutdownProcessingStart::initInstance(&mMockSM, mMockVcpuActions, mPowerStateHolder,
                                                  mMockTimerManager);
    }

    MockAndroidInit mMockAndroidInit;
    MockShutdownAction mMockShutdownAction;
    MockVcpuActions mMockVcpuActions;
    MockSM mMockSM;
    MockWakeLockManager mMockWakeLockManager;
    MockPowerStateHolder mPowerStateHolder;
    MockWakeUpEventDetector mMockWakeUpEventDetector;
    MockTimerManager mMockTimerManager;
};

/**
 * Check transition to 'PollingWakeUp' state.
 */
TEST_F(SendShutdownProcessingStartTest, onShutdownProcessingStartSent)
{
    auto state = SendShutdownProcessingStart::getInstance();

    EXPECT_CALL(mMockSM, transitionTo(PollingWakeUp::getInstance())).Times(1);

    state->onShutdownProcessingStartSent();
}

/**
 * Check if PowerState was changes to WAIT_FOR_CPUCOM_DELIVERY
 * Check if 'Shutdown processing start' notification is sent to V-CPU on
 * 'SendShutdownProcessingStart' state enter.
 */
TEST_F(SendShutdownProcessingStartTest, onEnter)
{
    InSequence seq;

    auto state = SendShutdownProcessingStart::getInstance();

    EXPECT_CALL(mPowerStateHolder, onPowerStateChange(PowerState::WAIT_FOR_CPUCOM_DELIVERY))
        .Times(1);
    EXPECT_CALL(mMockTimerManager, changeTimerHandler(ETimerType::None)).Times(1);
    EXPECT_CALL(mMockVcpuActions, sendShutdownProcessingStart()).Times(1);

    state->onEnter();
}

/**
 * Check if PowerState was changed to the previous
 */
TEST_F(SendShutdownProcessingStartTest, onExit)
{
    auto state = SendShutdownProcessingStart::getInstance();

    EXPECT_CALL(mPowerStateHolder, onPowerStateChange(_)).Times(1);

    state->onExit();
}

/**
 * Check State Name
 */
TEST_F(SendShutdownProcessingStartTest, stateName)
{
    auto state = SendShutdownProcessingStart::getInstance();

    ASSERT_STREQ(state->stateName().c_str(), "SendShutdownProcessingStart");
}

TEST_F(SendShutdownProcessingStartTest, getInstance)
{
    auto state = SendShutdownProcessingStart::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(SendShutdownProcessingStartTest, uninitInstance)
{
    SendShutdownProcessingStart::uninitInstance();
    auto state = SendShutdownProcessingStart::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
