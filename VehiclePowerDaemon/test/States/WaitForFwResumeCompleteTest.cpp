/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitForFwResumeComplete.h"
#include "WaitForAppResumeComplete.h"

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
using ::testing::Sequence;
using ::testing::Test;

class WaitForFwResumeCompleteTest : public Test {
protected:
    WaitForFwResumeCompleteTest()
    {
        WaitForFwResumeComplete::initInstance(&mStateMachineMock, mNotifyHelperMock, mTimerMock,
                                              mCpuActionsMock);
    }

    ~WaitForFwResumeCompleteTest();

    NiceMock<MockSM> mStateMachineMock;
    NiceMock<MockNotifyHelper> mNotifyHelperMock;
    NiceMock<MockTimer> mTimerMock;
    NiceMock<MockVcpuActions> mCpuActionsMock;
};

WaitForFwResumeCompleteTest::~WaitForFwResumeCompleteTest() {}

TEST_F(WaitForFwResumeCompleteTest, stateName)
{
    auto state = WaitForFwResumeComplete::getInstance();
    ASSERT_STREQ(state->stateName().c_str(), "WaitForFwResumeComplete");
}

TEST_F(WaitForFwResumeCompleteTest, getInstance)
{
    auto state = WaitForFwResumeComplete::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForFwResumeCompleteTest, uninitInstance)
{
    WaitForFwResumeComplete::uninitInstance();
    auto state = WaitForFwResumeComplete::getInstance();
    ASSERT_EQ(state, nullptr);
}

TEST_F(WaitForFwResumeCompleteTest, onEnter)
{
    auto state = WaitForFwResumeComplete::getInstance();

    // EXPECT_CALL(mCpuActionsMock, sendStartComplete()).Times(1);
    EXPECT_CALL(mTimerMock, start()).Times(1);
    EXPECT_CALL(mNotifyHelperMock, notifyVps(EVpdNotifications::FwResume)).Times(1);

    state->onEnter();
}

TEST_F(WaitForFwResumeCompleteTest, onFwResumeComplete)
{
    auto state = WaitForFwResumeComplete::getInstance();

    EXPECT_CALL(mStateMachineMock, transitionTo(WaitForAppResumeComplete::getInstance())).Times(1);

    state->onFwResumeComplete();
}

TEST_F(WaitForFwResumeCompleteTest, onTimeout_whenTimerIsExpired)
{
    auto state = WaitForFwResumeComplete::getInstance();
    EXPECT_CALL(mTimerMock, isExpired()).WillOnce(Return(true));

    EXPECT_CALL(mStateMachineMock, transitionTo(WaitForAppResumeComplete::getInstance())).Times(1);

    state->onTimeout();
}

TEST_F(WaitForFwResumeCompleteTest, onTimeout_whenTimerIsNotExpired)
{
    auto state = WaitForFwResumeComplete::getInstance();
    EXPECT_CALL(mTimerMock, isExpired()).WillOnce(Return(false));

    EXPECT_CALL(mStateMachineMock, transitionTo(WaitForAppResumeComplete::getInstance())).Times(0);

    state->onTimeout();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
