/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ResumeNative.h"

#include "MockNotifyHelper.h"
#include "MockSM.h"
#include "MockStateHolder.h"
#include "VpdStateMachineContext.h"
#include "WaitForFwResumeComplete.h"
#include "WaitForVpsStart.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdState;

using ::testing::NiceMock;
using ::testing::ReturnRef;
using ::testing::Sequence;
using ::testing::Test;

class ResumeNativeTest : public Test {
protected:
    ResumeNativeTest()
    {
        ON_CALL(mStateMachineMock, context()).WillByDefault(ReturnRef(mContext));
        ResumeNative::initInstance(&mStateMachineMock, mNotifyHelperMock, mStateHolderMock);
    }
    ~ResumeNativeTest();

    VpdStateMachineContext mContext;

    NiceMock<MockNotifyHelper> mNotifyHelperMock;
    NiceMock<MockSM> mStateMachineMock;
    NiceMock<MockStateHolder> mStateHolderMock;
};

ResumeNativeTest::~ResumeNativeTest() {}

TEST_F(ResumeNativeTest, stateName)
{
    auto state = ResumeNative::getInstance();
    ASSERT_STREQ(state->stateName().c_str(), "ResumeNative");
}

TEST_F(ResumeNativeTest, getInstance)
{
    auto state = ResumeNative::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(ResumeNativeTest, uninitInstance)
{
    ResumeNative::uninitInstance();
    auto state = ResumeNative::getInstance();
    ASSERT_EQ(state, nullptr);
}

TEST_F(ResumeNativeTest, onEnter_vps_not_disconnected)
{
    auto state = ResumeNative::getInstance();
    Sequence seq;
    mContext.setVpsDisconnected(false);

    EXPECT_CALL(mStateHolderMock, setVpdState(EVpdState::APP_RESUME)).Times(1).InSequence(seq);
    EXPECT_CALL(mNotifyHelperMock, notifyLog(EVpdState::APP_RESUME)).Times(1).InSequence(seq);
    EXPECT_CALL(mNotifyHelperMock, notifyAllNative(EVpdState::APP_RESUME)).Times(1).InSequence(seq);

    EXPECT_CALL(mStateMachineMock, transitionTo(WaitForFwResumeComplete::getInstance()))
        .Times(1)
        .InSequence(seq);

    state->onEnter();
}

TEST_F(ResumeNativeTest, onEnter_vps_disconnected)
{
    auto state = ResumeNative::getInstance();
    Sequence seq;
    mContext.setVpsDisconnected(true);

    EXPECT_CALL(mStateHolderMock, setVpdState(EVpdState::APP_RESUME)).Times(1).InSequence(seq);
    EXPECT_CALL(mNotifyHelperMock, notifyLog(EVpdState::APP_RESUME)).Times(1).InSequence(seq);
    EXPECT_CALL(mNotifyHelperMock, notifyAllNative(EVpdState::APP_RESUME)).Times(1).InSequence(seq);

    EXPECT_CALL(mStateMachineMock, transitionTo(WaitForVpsStart::getInstance()))
        .Times(1)
        .InSequence(seq);

    state->onEnter();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
