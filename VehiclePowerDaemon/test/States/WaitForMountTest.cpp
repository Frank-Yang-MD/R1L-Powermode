/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <RestartNative.h>
#include <WaitForMount.h>

#include "MockNotifyHelper.h"
#include "MockSM.h"
#include <MockStateHolder.h>

using namespace testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class WaitForMountTest : public Test {
public:
    void SetUp() { WaitForMount::initInstance(&mockSM, mockNotifyHelper); }

    MockSM mockSM;
    MockNotifyHelper mockNotifyHelper;
    MockStateHolder mockStateHolder;
};

TEST_F(WaitForMountTest, onEnter)
{
    auto state = WaitForMount::getInstance();

    EXPECT_CALL(mockNotifyHelper, notifyVps(common::EVpdNotifications::RequestMount)).Times(1);
    EXPECT_CALL(mockSM, transitionTo(RestartNative::getInstance())).Times(1);

    state->onEnter();
}

TEST_F(WaitForMountTest, stateName)
{
    auto state = WaitForMount::getInstance();
    ASSERT_STREQ(state->stateName().c_str(), "WaitForMount");
}

TEST_F(WaitForMountTest, getInstance)
{
    auto state = WaitForMount::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForMountTest, uninitInstance)
{
    WaitForMount::uninitInstance();
    auto state = WaitForMount::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
