/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "RestartNative.h"
#include "WaitForMount.h"
#include <WaitForModulesLoadAfterSuspendCancel.h>

#include "MockActions.h"
#include "MockKernelModulesControl.h"
#include "MockSM.h"

#include "MockNotifyHelper.h"
#include "MockStateHolder.h"

using namespace testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class WaitForModulesLoadAfterSuspendCancelTest : public Test {
protected:
    void SetUp()
    {
        WaitForModulesLoadAfterSuspendCancel::initInstance(&mMockSM, mMockKernelModulesControl);

        RestartNative::initInstance(&mMockSM, mMockNotifyHelper, mMockStateHolder);
    }

    MockSM mMockSM;
    MockKernelModulesControl mMockKernelModulesControl;

    MockNotifyHelper mMockNotifyHelper;
    MockStateHolder mMockStateHolder;
};

TEST_F(WaitForModulesLoadAfterSuspendCancelTest, onEnter)
{
    auto state = WaitForModulesLoadAfterSuspendCancel::getInstance();

    EXPECT_CALL(mMockKernelModulesControl, loadModules()).Times(1);
    EXPECT_CALL(mMockSM, transitionTo(WaitForMount::getInstance())).Times(1);

    state->onEnter();
}

TEST_F(WaitForModulesLoadAfterSuspendCancelTest, stateName)
{
    auto state = WaitForModulesLoadAfterSuspendCancel::getInstance();
    ASSERT_STREQ(state->stateName().c_str(), "WaitForModulesLoadAfterSuspendCancel");
}

TEST_F(WaitForModulesLoadAfterSuspendCancelTest, getInstance)
{
    auto state = WaitForModulesLoadAfterSuspendCancel::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForModulesLoadAfterSuspendCancelTest, uninitInstance)
{
    WaitForModulesLoadAfterSuspendCancel::uninitInstance();
    auto state = WaitForModulesLoadAfterSuspendCancel::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
