/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gtest/gtest.h>

#include "InitialState.h"
#include "NormalOperation.h"
#include "WaitForStartCpuCom.h"

#include "MockActions.h"
#include "MockAndroidInit.h"
#include "MockCpuCommControl.h"
#include "MockPowerStateHolder.h"
#include "MockSM.h"
#include "MockStateHolder.h"
#include "MockTimerManager.h"
#include "MockVpdRestartHolder.h"
#include "MockWakeUpEventDetector.h"

using namespace testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class InitialStateTest : public Test {
protected:
    void SetUp() override
    {
        InitialState::initInstance(&mMockSM, mMockWakeLockManager, mMockVpdRestartHolder,
                                   mMockCpuCommControl, mMockWakeUpEventDetector,
                                   mMockTimerManager);
        WaitForStartCpuCom::initInstance(&mMockSM, mMockAndroidInit, mMockCpuCommControl);
        NormalOperation::initInstance(&mMockSM, mMockPowerStateHolder, mMockVpdRestartHolder,
                                      mMockStateHolder);
    }

    void TearDown() override
    {
        InitialState::uninitInstance();
        WaitForStartCpuCom::uninitInstance();
        NormalOperation::uninitInstance();
    }

    StrictMock<MockSM> mMockSM;
    StrictMock<MockWakeLockManager> mMockWakeLockManager;

    MockAndroidInit mMockAndroidInit;
    MockVpdRestartHolder mMockVpdRestartHolder;
    MockCpuCommControl mMockCpuCommControl;
    MockPowerStateHolder mMockPowerStateHolder;
    MockWakeUpEventDetector mMockWakeUpEventDetector;
    MockTimerManager mMockTimerManager;
    MockStateHolder mMockStateHolder;
};

/**
 * Check transition from 'InitialState' to 'WaitForStartCpuCom'
 */
TEST_F(InitialStateTest, onTimeoutFirstStart)
{
    auto state = InitialState::getInstance();

    EXPECT_CALL(mMockVpdRestartHolder, isVpdRestarted()).WillOnce(Return(false));

    EXPECT_CALL(mMockWakeLockManager, acquireWakeLock()).Times(1);
    EXPECT_CALL(mMockWakeUpEventDetector, start()).Times(1);

    EXPECT_CALL(mMockSM, transitionTo(WaitForStartCpuCom::getInstance())).Times(1);

    state->onTimeout();
}

/**
 * Check transition from 'InitialState' to 'NormalOperation' in case of restart
 */
TEST_F(InitialStateTest, onTimeoutRestart)
{
    auto state = InitialState::getInstance();

    EXPECT_CALL(mMockVpdRestartHolder, isVpdRestarted()).WillOnce(Return(true));

    EXPECT_CALL(mMockCpuCommControl, start()).Times(1);
    EXPECT_CALL(mMockTimerManager, changeTimerHandler(ETimerType::WDTRTimer)).Times(1);
    EXPECT_CALL(mMockWakeUpEventDetector, start()).Times(1);

    EXPECT_CALL(mMockSM, transitionTo(NormalOperation::getInstance())).Times(1);

    state->onTimeout();
}

/**
 * Test stateName() method
 */
TEST_F(InitialStateTest, stateName)
{
    auto state = InitialState::getInstance();

    ASSERT_STREQ(state->stateName().c_str(), "InitialState");
}

/**
 * Check if getInstance() return initilized instance
 */
TEST_F(InitialStateTest, getInstance)
{
    auto state = InitialState::getInstance();

    ASSERT_NE(state, nullptr);
}

/**
 * After uninitInstance method getInstance() should return nullptr
 */
TEST_F(InitialStateTest, uninitInstance)
{
    InitialState::uninitInstance();
    auto state = InitialState::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
