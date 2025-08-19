/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <ResumeNative.h>
#include <WakeUp.h>

#include "MockActions.h"
#include "MockKernelModulesControl.h"
#include "MockSM.h"

#include "MockNotifyHelper.h"
#include "MockStateHolder.h"
#include "MockWakeUpManager.h"

using namespace testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class WakeUpTest : public Test {
protected:
    void SetUp()
    {
        WakeUp::initInstance(&mMockSM, mMockKernelModulesControl, mMockWakeUpManager);

        ResumeNative::initInstance(&mMockSM, mMockNotifyHelper, mMockStateHolder);
    }

    MockSM mMockSM;
    MockKernelModulesControl mMockKernelModulesControl;

    MockNotifyHelper mMockNotifyHelper;
    MockStateHolder mMockStateHolder;
    MockWakeUpManager mMockWakeUpManager;
};

TEST_F(WakeUpTest, onEnter)
{
    auto state = WakeUp::getInstance();

    EXPECT_CALL(mMockKernelModulesControl, loadModules()).Times(1);
    EXPECT_CALL(mMockWakeUpManager, wakeUp()).Times(1);
    EXPECT_CALL(mMockSM, transitionTo(ResumeNative::getInstance())).Times(1);

    state->onEnter();
}

TEST_F(WakeUpTest, stateName)
{
    auto state = WakeUp::getInstance();
    EXPECT_EQ(state->stateName(), "WakeUp");
}

TEST_F(WakeUpTest, getInstance)
{
    auto state = WakeUp::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WakeUpTest, uninitInstance)
{
    WakeUp::uninitInstance();
    auto state = WakeUp::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
