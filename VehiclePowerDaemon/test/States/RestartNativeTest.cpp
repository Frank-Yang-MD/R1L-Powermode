/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "RestartNative.h"

#include "MockNotifyHelper.h"
#include "MockSM.h"
#include "MockStateHolder.h"
#include "VpdStateMachineContext.h"
#include "WaitForFwRestartComplete.h"
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

class RestartNativeTest : public Test {
protected:
    RestartNativeTest()
    {
        ON_CALL(mStateMachineMock, context()).WillByDefault(ReturnRef(mContext));
        RestartNative::initInstance(&mStateMachineMock, mNotifyHelperMock, mStateHolderMock);
    }
    ~RestartNativeTest();

    VpdStateMachineContext mContext;

    NiceMock<MockNotifyHelper> mNotifyHelperMock;
    NiceMock<MockSM> mStateMachineMock;
    NiceMock<MockStateHolder> mStateHolderMock;
};

RestartNativeTest::~RestartNativeTest() {}

TEST_F(RestartNativeTest, stateName)
{
    auto state = RestartNative::getInstance();
    ASSERT_STREQ(state->stateName().c_str(), "RestartNative");
}

TEST_F(RestartNativeTest, getInstance)
{
    auto state = RestartNative::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(RestartNativeTest, uninitInstance)
{
    RestartNative::uninitInstance();
    auto state = RestartNative::getInstance();
    ASSERT_EQ(state, nullptr);
}

TEST_F(RestartNativeTest, onEnter_vps_not_disconnected)
{
    auto state = RestartNative::getInstance();
    Sequence seq;
    mContext.setVpsDisconnected(false);

    EXPECT_CALL(mStateHolderMock, setVpdState(EVpdState::APP_RESTART)).Times(1).InSequence(seq);
    EXPECT_CALL(mNotifyHelperMock, notifyLog(EVpdState::APP_RESTART)).Times(1).InSequence(seq);
    EXPECT_CALL(mNotifyHelperMock, notifyAllNative(EVpdState::APP_RESTART))
        .Times(1)
        .InSequence(seq);

    EXPECT_CALL(mStateMachineMock, transitionTo(WaitForFwRestartComplete::getInstance()))
        .Times(1)
        .InSequence(seq);

    state->onEnter();
}

TEST_F(RestartNativeTest, onEnter_vps_disconnected)
{
    auto state = RestartNative::getInstance();
    Sequence seq;
    mContext.setVpsDisconnected(true);

    EXPECT_CALL(mStateHolderMock, setVpdState(EVpdState::APP_RESTART)).Times(1).InSequence(seq);
    EXPECT_CALL(mNotifyHelperMock, notifyLog(EVpdState::APP_RESTART)).Times(1).InSequence(seq);
    EXPECT_CALL(mNotifyHelperMock, notifyAllNative(EVpdState::APP_RESTART))
        .Times(1)
        .InSequence(seq);

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
