/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <WaitForModulesLoadBeforeUnload.h>
#include <WaitForModulesUnload.h>

#include "MockActions.h"
#include "MockKernelModulesControl.h"
#include "MockPowerStateHolder.h"
#include "MockSM.h"
#include "MockTimer.h"
#include "MockTimerHandler.h"
#include "MockWakeUpManager.h"

using namespace testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class WaitForModulesLoadBeforeUnloadTest : public Test {
public:
    ~WaitForModulesLoadBeforeUnloadTest();

    void SetUp()
    {
        WaitForModulesLoadBeforeUnload::initInstance(&mMockSM, mCheckTimer, mTimeout,
                                                     mMockKernelModulesControl);

        WaitForModulesUnload::initInstance(&mMockSM, mTimer, mTimer, mMockKernelModulesControl,
                                           mMockPowerStateHolder);
    }

    NiceMock<MockSM> mMockSM;
    NiceMock<MockVcpuActions> mMockVcpuActions;
    NiceMock<MockTimer> mCheckTimer;
    NiceMock<MockTimer> mTimeout;
    NiceMock<MockKernelModulesControl> mMockKernelModulesControl;

    NiceMock<MockTimer> mTimer;
    NiceMock<MockPowerStateHolder> mMockPowerStateHolder;
};

// Prevent "-Wweak-vtables" compiler warning by providing at least 1 (destructor in this case)
// out-of-line virtual method definition.
WaitForModulesLoadBeforeUnloadTest::~WaitForModulesLoadBeforeUnloadTest() {}

TEST_F(WaitForModulesLoadBeforeUnloadTest, onEnter_modulesLoadedAndSkipModulesUnloadedCheck)
{
    auto state = WaitForModulesLoadBeforeUnload::getInstance();

    EXPECT_CALL(mMockKernelModulesControl, skipModulesUnloadedCheck())
        .Times(1)
        .WillOnce(Return(false));
    EXPECT_CALL(mMockKernelModulesControl, isModulesLoaded()).Times(1).WillOnce(Return(true));

    EXPECT_CALL(mMockSM, transitionTo(WaitForModulesUnload::getInstance())).Times(1);

    state->onEnter();
}

TEST_F(WaitForModulesLoadBeforeUnloadTest, onEnter_modulesNotLoadedAndSkipModulesUnloadedCheck)
{
    auto state = WaitForModulesLoadBeforeUnload::getInstance();

    EXPECT_CALL(mMockKernelModulesControl, skipModulesUnloadedCheck())
        .Times(1)
        .WillOnce(Return(false));
    EXPECT_CALL(mMockKernelModulesControl, isModulesLoaded()).Times(1).WillOnce(Return(false));

    EXPECT_CALL(mCheckTimer, start()).Times(1);
    EXPECT_CALL(mTimeout, start()).Times(1);

    state->onEnter();
}

TEST_F(WaitForModulesLoadBeforeUnloadTest, onEnter_modulesNotLoadedAndDoNotSkipModulesUnloadedCheck)
{
    auto state = WaitForModulesLoadBeforeUnload::getInstance();

    EXPECT_CALL(mMockKernelModulesControl, skipModulesUnloadedCheck())
        .Times(1)
        .WillOnce(Return(true));

    EXPECT_CALL(mMockSM, transitionTo(WaitForModulesUnload::getInstance())).Times(1);

    state->onEnter();
}

TEST_F(WaitForModulesLoadBeforeUnloadTest, onEnter_modulesLoadedAndDoNotSkipModulesUnloadedCheck)
{
    auto state = WaitForModulesLoadBeforeUnload::getInstance();

    EXPECT_CALL(mMockKernelModulesControl, skipModulesUnloadedCheck())
        .Times(1)
        .WillOnce(Return(true));

    EXPECT_CALL(mMockSM, transitionTo(WaitForModulesUnload::getInstance())).Times(1);

    state->onEnter();
}

TEST_F(WaitForModulesLoadBeforeUnloadTest, onTimeoutExpired)
{
    auto state = WaitForModulesLoadBeforeUnload::getInstance();

    EXPECT_CALL(mTimeout, isExpired()).WillRepeatedly(Return(true));
    EXPECT_CALL(mMockSM, transitionTo(WaitForModulesUnload::getInstance())).Times(1);

    state->onTimeout();
}

TEST_F(WaitForModulesLoadBeforeUnloadTest, onTimeoutNotExpired)
{
    auto state = WaitForModulesLoadBeforeUnload::getInstance();

    EXPECT_CALL(mTimeout, isExpired()).WillRepeatedly(Return(false));
    EXPECT_CALL(mMockSM, transitionTo(WaitForModulesUnload::getInstance())).Times(0);

    state->onTimeout();
}

TEST_F(WaitForModulesLoadBeforeUnloadTest, onModulesLoaded)
{
    auto state = WaitForModulesLoadBeforeUnload::getInstance();

    EXPECT_CALL(mCheckTimer, isExpired()).WillRepeatedly(Return(true));
    EXPECT_CALL(mMockKernelModulesControl, isModulesLoaded()).WillRepeatedly(Return(true));
    EXPECT_CALL(mMockSM, transitionTo(WaitForModulesUnload::getInstance())).Times(1);

    state->onTimeout();
}

TEST_F(WaitForModulesLoadBeforeUnloadTest, onModulesLoading)
{
    auto state = WaitForModulesLoadBeforeUnload::getInstance();

    EXPECT_CALL(mCheckTimer, isExpired()).WillRepeatedly(Return(true));
    EXPECT_CALL(mMockKernelModulesControl, isModulesLoaded()).WillRepeatedly(Return(false));
    EXPECT_CALL(mCheckTimer, start()).Times(1);
    EXPECT_CALL(mMockSM, transitionTo(WaitForModulesUnload::getInstance())).Times(0);

    state->onTimeout();
}

TEST_F(WaitForModulesLoadBeforeUnloadTest, stateName)
{
    auto state = WaitForModulesLoadBeforeUnload::getInstance();
    ASSERT_STREQ(state->stateName().c_str(), "WaitForModulesLoadBeforeUnload");
}

TEST_F(WaitForModulesLoadBeforeUnloadTest, getInstance)
{
    auto state = WaitForModulesLoadBeforeUnload::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForModulesLoadBeforeUnloadTest, uninitInstance)
{
    WaitForModulesLoadBeforeUnload::uninitInstance();
    auto state = WaitForModulesLoadBeforeUnload::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
