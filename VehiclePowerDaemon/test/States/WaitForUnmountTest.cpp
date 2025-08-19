/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <WaitForModulesLoadBeforeUnload.h>
#include <WaitForUnmount.h>

#include "MockNotifyHelper.h"
#include "MockSM.h"
#include <MockKernelModulesControl.h>
#include <MockPowerStateHolder.h>
#include <MockStateHolder.h>
#include <MockTimer.h>
#include <MockVcpuInteraction.h>

using namespace testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class WaitForUnmountTest : public Test {
public:
    void SetUp()
    {
        WaitForUnmount::initInstance(&mockSM, mockNotifyHelper, mockTimer, mockVcpuInteration);
        WaitForModulesLoadBeforeUnload::initInstance(&mockSM, mockTimer, mockTimer,
                                                     mockKernelModulesControl);
    }

    MockSM mockSM;
    MockNotifyHelper mockNotifyHelper;
    MockTimer mockTimer;
    MockVcpuInteraction mockVcpuInteration;
    MockKernelModulesControl mockKernelModulesControl;
    MockPowerStateHolder mockPowerStateHolder;
};

TEST_F(WaitForUnmountTest, onEnter)
{
    auto state = WaitForUnmount::getInstance();

    EXPECT_CALL(mockNotifyHelper, notifyVps(common::EVpdNotifications::RequestUnmount)).Times(1);
    EXPECT_CALL(mockTimer, start()).Times(1);

    state->onEnter();
}

TEST_F(WaitForUnmountTest, onTimeoutExpired)
{
    auto state = WaitForUnmount::getInstance();

    EXPECT_CALL(mockTimer, isExpired()).Times(1).WillOnce(Return(true));
    EXPECT_CALL(mockVcpuInteration, sendSelfRebootTrue()).Times(1);
    EXPECT_CALL(mockSM, transitionTo(WaitForModulesLoadBeforeUnload::getInstance())).Times(1);

    state->onTimeout();
}

TEST_F(WaitForUnmountTest, onTimeoutNotExpired)
{
    auto state = WaitForUnmount::getInstance();

    EXPECT_CALL(mockTimer, isExpired()).Times(1).WillOnce(Return(false));
    EXPECT_CALL(mockVcpuInteration, sendSelfRebootTrue()).Times(0);
    EXPECT_CALL(mockSM, transitionTo(WaitForModulesLoadBeforeUnload::getInstance())).Times(0);

    state->onTimeout();
}

TEST_F(WaitForUnmountTest, onUnmounComplete)
{
    auto state = WaitForUnmount::getInstance();

    EXPECT_CALL(mockSM, transitionTo(WaitForModulesLoadBeforeUnload::getInstance())).Times(1);

    state->onVpsUnmountComplete();
}

TEST_F(WaitForUnmountTest, stateName)
{
    auto state = WaitForUnmount::getInstance();
    ASSERT_STREQ(state->stateName().c_str(), "WaitForUnmount");
}

TEST_F(WaitForUnmountTest, getInstance)
{
    auto state = WaitForUnmount::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForUnmountTest, uninitInstance)
{
    WaitForUnmount::uninitInstance();
    auto state = WaitForUnmount::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
