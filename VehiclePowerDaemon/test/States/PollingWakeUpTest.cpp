/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <chrono>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <thread>

#include "ITimeoutControl.h"
#include "PollingWakeUp.h"
#include "ResumeProcessingStartComplete.h"
#include "VpdStateMachine.h"

#include "MockActions.h"
#include "MockPowerStateHolder.h"
#include "MockSM.h"
#include "MockTimerManager.h"
#include "MockWakeUpEventDetector.h"

using namespace testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class PollingWakeUpTest : public Test {
protected:
    void SetUp()
    {
        PollingWakeUp::initInstance(&mMockSM, mMockWakeUpEventDetector, mMockWakeLockManager,
                                    mMockShutdownAction);
        ResumeProcessingStartComplete::initInstance(&mMockSM, mMockVcpuActions,
                                                    mMockPowerStateHolder, mMockTimerManager);
    }

    StrictMock<MockWakeUpEventDetector> mMockWakeUpEventDetector;
    StrictMock<MockSM> mMockSM;
    StrictMock<MockWakeLockManager> mMockWakeLockManager;
    StrictMock<MockShutdownAction> mMockShutdownAction;
    StrictMock<MockVcpuActions> mMockVcpuActions;
    StrictMock<MockPowerStateHolder> mMockPowerStateHolder;
    StrictMock<MockTimerManager> mMockTimerManager;
};

TEST_F(PollingWakeUpTest, onEnter_prepare_success)
{
    auto state = PollingWakeUp::getInstance();

    {
        testing::InSequence s;

        EXPECT_CALL(mMockWakeLockManager, releaseWakeLock()).Times(1);
        EXPECT_CALL(mMockWakeLockManager, disableWakeLocks()).Times(1);
        EXPECT_CALL(mMockShutdownAction, performShutdownAction()).Times(1);
    }

    state->onEnter();
}

TEST_F(PollingWakeUpTest, onWakeUp)
{
    auto state = PollingWakeUp::getInstance();

    EXPECT_CALL(mMockSM, transitionTo(ResumeProcessingStartComplete::getInstance())).Times(1);

    state->onWakeUp();
}

TEST_F(PollingWakeUpTest, onExit)
{
    auto state = PollingWakeUp::getInstance();

    EXPECT_CALL(mMockWakeLockManager, enableWakeLocks()).Times(1);
    EXPECT_CALL(mMockWakeLockManager, acquireWakeLock()).Times(1);

    state->onExit();
}

/**
 * Check State Name
 */
TEST_F(PollingWakeUpTest, stateName)
{
    auto state = PollingWakeUp::getInstance();
    ASSERT_STREQ(state->stateName().c_str(), "PollingWakeUp");
}

TEST_F(PollingWakeUpTest, getInstance)
{
    auto state = PollingWakeUp::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(PollingWakeUpTest, uninitInstance)
{
    PollingWakeUp::uninitInstance();
    auto state = PollingWakeUp::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
