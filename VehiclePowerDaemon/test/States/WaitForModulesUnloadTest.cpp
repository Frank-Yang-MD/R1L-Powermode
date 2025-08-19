/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <SendShutdownProcessingStart.h>
#include <WaitForModulesLoadAfterSuspendCancel.h>
#include <WaitForModulesUnload.h>

#include "MockActions.h"
#include "MockKernelModulesControl.h"
#include "MockPowerStateHolder.h"
#include "MockSM.h"
#include "MockTimer.h"
#include "MockTimerHandler.h"
#include "MockTimerManager.h"
#include "MockWakeUpManager.h"

using namespace testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class WaitForModulesUnloadTest : public Test {
protected:
    void SetUp()
    {
        WaitForModulesUnload::initInstance(&mMockSM, mCheckTimer, mTimeout,
                                           mMockKernelModulesControl, mMockPowerStateHolder);

        SendShutdownProcessingStart::initInstance(&mMockSM, mMockVcpuActions, mMockPowerStateHolder,
                                                  mMockTimerManager);
        WaitForModulesLoadAfterSuspendCancel::initInstance(&mMockSM, mMockKernelModulesControl);
    }

    MockSM mMockSM;
    MockVcpuActions mMockVcpuActions;
    MockTimer mCheckTimer;
    MockTimer mTimeout;
    MockKernelModulesControl mMockKernelModulesControl;
    MockTimerHandling mMockTimerHandler;

    MockPowerStateHolder mMockPowerStateHolder;
    MockTimerManager mMockTimerManager;
};

TEST_F(WaitForModulesUnloadTest, onEnter)
{
    auto state = WaitForModulesUnload::getInstance();

    EXPECT_CALL(mCheckTimer, start()).Times(1);
    EXPECT_CALL(mTimeout, start()).Times(1);
    EXPECT_CALL(mMockKernelModulesControl, unloadModules()).Times(1);

    state->onEnter();
}

TEST_F(WaitForModulesUnloadTest, onTimeoutExpiredShutdownState)
{
    auto state = WaitForModulesUnload::getInstance();

    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(mTimeout, isExpired()).WillRepeatedly(Return(true));
    EXPECT_CALL(mMockSM, transitionTo(SendShutdownProcessingStart::getInstance())).Times(1);

    state->onTimeout();
}

TEST_F(WaitForModulesUnloadTest, onTimeoutExpiredNotShutdownState)
{
    auto state = WaitForModulesUnload::getInstance();

    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).Times(1).WillOnce(Return(false));
    EXPECT_CALL(mTimeout, isExpired()).WillRepeatedly(Return(true));
    EXPECT_CALL(mMockSM, transitionTo(WaitForModulesLoadAfterSuspendCancel::getInstance()))
        .Times(1);

    state->onTimeout();
}

TEST_F(WaitForModulesUnloadTest, onModulesUnloadedShutdownState)
{
    auto state = WaitForModulesUnload::getInstance();

    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(mCheckTimer, isExpired()).WillRepeatedly(Return(true));
    EXPECT_CALL(mMockKernelModulesControl, isModulesUnloaded()).WillRepeatedly(Return(true));
    EXPECT_CALL(mMockSM, transitionTo(SendShutdownProcessingStart::getInstance())).Times(1);

    state->onTimeout();
}

TEST_F(WaitForModulesUnloadTest, onModulesUnloadedNotShutdownState)
{
    auto state = WaitForModulesUnload::getInstance();

    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).Times(1).WillOnce(Return(false));
    EXPECT_CALL(mCheckTimer, isExpired()).WillRepeatedly(Return(true));
    EXPECT_CALL(mMockKernelModulesControl, isModulesUnloaded()).WillRepeatedly(Return(true));
    EXPECT_CALL(mMockSM, transitionTo(WaitForModulesLoadAfterSuspendCancel::getInstance()))
        .Times(1);

    state->onTimeout();
}

TEST_F(WaitForModulesUnloadTest, onModulesUnloading)
{
    auto state = WaitForModulesUnload::getInstance();

    EXPECT_CALL(mCheckTimer, isExpired()).WillRepeatedly(Return(true));
    EXPECT_CALL(mMockKernelModulesControl, isModulesUnloaded()).WillRepeatedly(Return(false));
    EXPECT_CALL(mMockSM, transitionTo(SendShutdownProcessingStart::getInstance())).Times(0);

    state->onTimeout();
}

TEST_F(WaitForModulesUnloadTest, onTimeoutCheckTimerNotExpired)
{
    auto state = WaitForModulesUnload::getInstance();

    EXPECT_CALL(mCheckTimer, isExpired()).WillRepeatedly(Return(false));
    EXPECT_CALL(mMockKernelModulesControl, isModulesUnloaded()).Times(0);
    EXPECT_CALL(mMockSM, transitionTo(SendShutdownProcessingStart::getInstance())).Times(0);

    state->onTimeout();
}

TEST_F(WaitForModulesUnloadTest, stateName)
{
    auto state = WaitForModulesUnload::getInstance();
    ASSERT_STREQ(state->stateName().c_str(), "WaitForModulesUnload");
}

TEST_F(WaitForModulesUnloadTest, getInstance)
{
    auto state = WaitForModulesUnload::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForModulesUnloadTest, uninitInstance)
{
    WaitForModulesUnload::uninitInstance();
    auto state = WaitForModulesUnload::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
