/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForFwRestartComplete.h"
#include "VpdStateMachineContext.h"
#include "WaitForAppRestartComplete.h"
#include "WaitForVpsStart.h"

#include "MockNotifyHelper.h"
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

class WaitForFwRestartCompleteTest : public Test {
protected:
    WaitForFwRestartCompleteTest()
    {
        WaitForFwRestartComplete::initInstance(&mSMMock, mNotifyHelperMock, mTimerMock,
                                               mCpuActionsMock);
        ON_CALL(mSMMock, context()).WillByDefault(ReturnRef(mContext));
    }
    ~WaitForFwRestartCompleteTest();

    VpdStateMachineContext mContext;
    NiceMock<MockSM> mSMMock;
    NiceMock<MockNotifyHelper> mNotifyHelperMock;
    NiceMock<MockTimer> mTimerMock;
    NiceMock<MockVcpuActions> mCpuActionsMock;
};

WaitForFwRestartCompleteTest::~WaitForFwRestartCompleteTest() {}

TEST_F(WaitForFwRestartCompleteTest, stateName)
{
    auto state = WaitForFwRestartComplete::getInstance();
    ASSERT_STREQ(state->stateName().c_str(), "WaitForFwRestartComplete");
}

TEST_F(WaitForFwRestartCompleteTest, getInstance)
{
    auto state = WaitForFwRestartComplete::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForFwRestartCompleteTest, uninitInstance)
{
    WaitForFwRestartComplete::uninitInstance();
    auto state = WaitForFwRestartComplete::getInstance();
    ASSERT_EQ(state, nullptr);
}

TEST_F(WaitForFwRestartCompleteTest, onEnter)
{
    auto state = WaitForFwRestartComplete::getInstance();

    EXPECT_CALL(mNotifyHelperMock, notifyVps(EVpdNotifications::FwRestart)).Times(1);

    state->onEnter();
}

TEST_F(WaitForFwRestartCompleteTest, onFwRestartComplete)
{
    auto state = WaitForFwRestartComplete::getInstance();

    EXPECT_CALL(mSMMock, transitionTo(WaitForAppRestartComplete::getInstance())).Times(1);

    state->onFwRestartComplete();
}

TEST_F(WaitForFwRestartCompleteTest, onTimeout)
{
    auto state = WaitForFwRestartComplete::getInstance();

    ON_CALL(mTimerMock, isExpired()).WillByDefault(Return(true));
    EXPECT_CALL(mTimerMock, isExpired()).Times(1);
    ASSERT_NE(WaitForAppRestartComplete::getInstance(), nullptr);
    EXPECT_CALL(mSMMock, transitionTo(WaitForAppRestartComplete::getInstance())).Times(1);

    state->onTimeout();
}

TEST_F(WaitForFwRestartCompleteTest, onTimeoutNotExpired)
{
    auto state = WaitForFwRestartComplete::getInstance();

    EXPECT_CALL(mTimerMock, isExpired()).WillOnce(Return(false));
    ASSERT_NE(WaitForAppRestartComplete::getInstance(), nullptr);
    EXPECT_CALL(mSMMock, transitionTo(WaitForAppRestartComplete::getInstance())).Times(0);

    state->onTimeout();
}

TEST_F(WaitForFwRestartCompleteTest, onDisconnectVps)
{
    auto state = WaitForFwRestartComplete::getInstance();
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
