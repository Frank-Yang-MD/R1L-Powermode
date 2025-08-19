/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForAppResumeComplete.h"
#include "Resume.h"
#include "VpdStateMachineContext.h"
#include "WaitForVpsStart.h"

#include <MockActions.h>
#include <MockNotifyHelper.h>
#include <MockSM.h>
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
using ::testing::Sequence;
using ::testing::Test;

class WaitForAppResumeCompleteTest : public Test {
protected:
    WaitForAppResumeCompleteTest()
    {
        WaitForAppResumeComplete::initInstance(&mStateMachineMock, mNotifyHelperMock, mTimerMock,
                                               mCpuActionsMock);
        ON_CALL(mStateMachineMock, context()).WillByDefault(ReturnRef(mContext));
    }

    ~WaitForAppResumeCompleteTest();

    VpdStateMachineContext mContext;
    NiceMock<MockSM> mStateMachineMock;
    NiceMock<MockNotifyHelper> mNotifyHelperMock;
    NiceMock<MockTimer> mTimerMock;
    NiceMock<MockVcpuActions> mCpuActionsMock;
};

WaitForAppResumeCompleteTest::~WaitForAppResumeCompleteTest() {}

TEST_F(WaitForAppResumeCompleteTest, stateName)
{
    auto state = WaitForAppResumeComplete::getInstance();
    ASSERT_STREQ(state->stateName().c_str(), "WaitForAppResumeComplete");
}

TEST_F(WaitForAppResumeCompleteTest, getInstance)
{
    auto state = WaitForAppResumeComplete::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForAppResumeCompleteTest, uninitInstance)
{
    WaitForAppResumeComplete::uninitInstance();
    auto state = WaitForAppResumeComplete::getInstance();
    ASSERT_EQ(state, nullptr);
}

TEST_F(WaitForAppResumeCompleteTest, onEnter)
{
    auto state = WaitForAppResumeComplete::getInstance();

    {
        testing::InSequence seq;
        EXPECT_CALL(mTimerMock, start()).Times(1);
        EXPECT_CALL(mNotifyHelperMock, notifyVps(EVpdNotifications::AppResume)).Times(1);
        EXPECT_CALL(mCpuActionsMock, sendMainStartComplete()).Times(1);
    }

    state->onEnter();
}

TEST_F(WaitForAppResumeCompleteTest, onExit)
{
    auto state = WaitForAppResumeComplete::getInstance();

    EXPECT_CALL(mStateMachineMock, transitionTo(Resume::getInstance())).Times(0);

    state->onExit();
}

TEST_F(WaitForAppResumeCompleteTest, onAppResumeComplete)
{
    auto state = WaitForAppResumeComplete::getInstance();

    EXPECT_CALL(mStateMachineMock, transitionTo(Resume::getInstance())).Times(1);

    state->onAppResumeComplete();
}

TEST_F(WaitForAppResumeCompleteTest, onTimeout_whenTimerIsExpired)
{
    auto state = WaitForAppResumeComplete::getInstance();
    EXPECT_CALL(mTimerMock, isExpired()).WillOnce(Return(true));

    EXPECT_CALL(mStateMachineMock, transitionTo(Resume::getInstance())).Times(1);

    state->onTimeout();
}

TEST_F(WaitForAppResumeCompleteTest, onTimeout_whenTimerIsNotExpired)
{
    auto state = WaitForAppResumeComplete::getInstance();
    EXPECT_CALL(mTimerMock, isExpired()).WillOnce(Return(false));

    EXPECT_CALL(mStateMachineMock, transitionTo(Resume::getInstance())).Times(0);

    state->onTimeout();
}

TEST_F(WaitForAppResumeCompleteTest, onDisconnectVps)
{
    auto state = WaitForAppResumeComplete::getInstance();
    mContext.setVpsDisconnected(false);

    EXPECT_CALL(mStateMachineMock, transitionTo(WaitForVpsStart::getInstance())).Times(1);

    state->onDisconnectVps();

    ASSERT_TRUE(mContext.isVpsDisconnected());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
