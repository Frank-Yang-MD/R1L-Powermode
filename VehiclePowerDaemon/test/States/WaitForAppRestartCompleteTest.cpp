/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForAppRestartComplete.h"
#include "NormalOperation.h"
#include "VpdStateMachineContext.h"
#include "WaitForAppStopVps.h"
#include "WaitForVpsStart.h"

#include "MockNotifyHelper.h"
#include "MockPowerStateHolder.h"
#include "MockSM.h"
#include <MockActions.h>
#include <MockTimer.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdNotifications;

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::Test;

class WaitForAppRestartCompleteTest : public Test {
protected:
    WaitForAppRestartCompleteTest()
    {
        WaitForAppRestartComplete::initInstance(&mSMMock, mNotifyHelperMock, mTimerMock,
                                                mCpuActionsMock, mMockPowerStateHolder);
        ON_CALL(mSMMock, context()).WillByDefault(ReturnRef(mContext));
    }
    ~WaitForAppRestartCompleteTest();

    VpdStateMachineContext mContext;
    NiceMock<MockSM> mSMMock;
    NiceMock<MockNotifyHelper> mNotifyHelperMock;
    NiceMock<MockTimer> mTimerMock;
    NiceMock<MockVcpuActions> mCpuActionsMock;
    NiceMock<MockPowerStateHolder> mMockPowerStateHolder;
};

WaitForAppRestartCompleteTest::~WaitForAppRestartCompleteTest() {}

TEST_F(WaitForAppRestartCompleteTest, stateName)
{
    auto state = WaitForAppRestartComplete::getInstance();
    ASSERT_STREQ(state->stateName().c_str(), "WaitForAppRestartComplete");
}

TEST_F(WaitForAppRestartCompleteTest, getInstance)
{
    auto state = WaitForAppRestartComplete::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForAppRestartCompleteTest, uninitInstance)
{
    WaitForAppRestartComplete::uninitInstance();
    auto state = WaitForAppRestartComplete::getInstance();
    ASSERT_EQ(state, nullptr);
}

TEST_F(WaitForAppRestartCompleteTest, onEnter)
{
    auto state = WaitForAppRestartComplete::getInstance();

    EXPECT_CALL(mTimerMock, start()).Times(1);
    EXPECT_CALL(mNotifyHelperMock, notifyVps(EVpdNotifications::AppRestart)).Times(1);

    state->onEnter();
}

TEST_F(WaitForAppRestartCompleteTest, onExit)
{
    auto state = WaitForAppRestartComplete::getInstance();

    EXPECT_CALL(mCpuActionsMock, sendShutdownCancelComplete()).Times(1);

    state->onExit();
}

TEST_F(WaitForAppRestartCompleteTest, onAppRestartComplete_notShutdownState)
{
    auto state = WaitForAppRestartComplete::getInstance();

    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).WillOnce(Return(false));
    EXPECT_CALL(mSMMock, transitionTo(NormalOperation::getInstance())).Times(1);

    state->onAppRestartComplete();
}

TEST_F(WaitForAppRestartCompleteTest, onAppRestartComplete_shutdownState)
{
    auto state = WaitForAppRestartComplete::getInstance();

    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).WillOnce(Return(true));
    EXPECT_CALL(mSMMock, transitionTo(WaitForAppStopVps::getInstance())).Times(1);

    state->onAppRestartComplete();
}

TEST_F(WaitForAppRestartCompleteTest, onTimeout_notShutdownState)
{
    auto state = WaitForAppRestartComplete::getInstance();

    ON_CALL(mTimerMock, isExpired()).WillByDefault(Return(true));
    EXPECT_CALL(mTimerMock, isExpired()).Times(1);
    ASSERT_NE(WaitForAppRestartComplete::getInstance(), nullptr);
    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).WillOnce(Return(false));
    EXPECT_CALL(mSMMock, transitionTo(NormalOperation::getInstance())).Times(1);

    state->onTimeout();
}

TEST_F(WaitForAppRestartCompleteTest, onTimeout_shutdownState)
{
    auto state = WaitForAppRestartComplete::getInstance();

    ON_CALL(mTimerMock, isExpired()).WillByDefault(Return(true));
    EXPECT_CALL(mTimerMock, isExpired()).Times(1);
    ASSERT_NE(WaitForAppRestartComplete::getInstance(), nullptr);
    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).WillOnce(Return(true));
    EXPECT_CALL(mSMMock, transitionTo(WaitForAppStopVps::getInstance())).Times(1);

    state->onTimeout();
}

TEST_F(WaitForAppRestartCompleteTest, onTimeoutNotExpired)
{
    auto state = WaitForAppRestartComplete::getInstance();

    EXPECT_CALL(mTimerMock, isExpired()).WillOnce(Return(false));
    ASSERT_NE(WaitForAppRestartComplete::getInstance(), nullptr);
    EXPECT_CALL(mSMMock, transitionTo(NormalOperation::getInstance())).Times(0);

    state->onTimeout();
}

TEST_F(WaitForAppRestartCompleteTest, onDisconnectVps)
{
    auto state = WaitForAppRestartComplete::getInstance();
    mContext.setVpsDisconnected(false);

    EXPECT_CALL(mSMMock, transitionTo(WaitForVpsStart::getInstance())).Times(1);

    state->onDisconnectVps();

    ASSERT_TRUE(mContext.isVpsDisconnected());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
