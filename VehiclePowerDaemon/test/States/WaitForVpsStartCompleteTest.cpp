/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "EVpdState.h"
#include "MockActions.h"
#include "MockAndroidInit.h"
#include "MockNotifyHelper.h"
#include "MockPowerStateHolder.h"
#include "MockSM.h"
#include "MockStateHolder.h"
#include "MockTimer.h"
#include "MockVpdRestartHolder.h"
#include "NormalOperation.h"
#include "VpdStateMachineContext.h"
#include "WaitForAppStopVps.h"
#include "WaitForVpsStartComplete.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::Eq;
using ::testing::Return;
using ::testing::ReturnRef;

/**
 * Test Fixture Object
 */
class WaitForVpsStartCompleteTest : public ::testing::Test {
public:
    WaitForVpsStartCompleteTest()
    {
        ON_CALL(mMockSM, context()).WillByDefault(ReturnRef(mContext));
        WaitForVpsStartComplete::initInstance(&mMockSM, mMockNotifyHelper, mMockPowerStateHolder,
                                              mMockAndroidInit, mMockVcpuActions);
        NormalOperation::initInstance(&mMockSM, mMockPowerStateHolder, mMockVpdRestartHolder,
                                      mMockStateHolder);

        WaitForAppStopVps::initInstance(&mMockSM, mMockNotifyHelper, mMockAppStopVpsTimer,
                                        mMockEfwStopVpsTimer, mMockVcpuActions, mMockAndroidInit);
    }

    const std::string MELCO_SERVICES_START_COMPLETE_PROP_NAME{"vendor.melco.boot_completed"};
    const std::string MELCO_SERVICES_START_COMPLETE_PROP_VALUE{"1"};

    VpdStateMachineContext mContext;

    MockSM mMockSM;
    MockNotifyHelper mMockNotifyHelper;
    MockAndroidInit mMockAndroidInit;
    MockPowerStateHolder mMockPowerStateHolder;
    MockVcpuActions mMockVcpuActions;
    MockVpdRestartHolder mMockVpdRestartHolder;
    MockStateHolder mMockStateHolder;

    testing::NiceMock<MockTimer> mMockAppStopVpsTimer;
    testing::NiceMock<MockTimer> mMockEfwStopVpsTimer;
};

/**
 * Check if a notification about system state change is sent to Vehicle Power Service
 * at the moment SM is entering 'WaitForVpsStartComplete' state
 */
TEST_F(WaitForVpsStartCompleteTest, onEnter_NOT_start_сompleted)
{
    auto state = WaitForVpsStartComplete::getInstance();
    mContext.setVpsStartCompleted(false);
    EXPECT_CALL(mMockNotifyHelper, notifyVps(common::EVpdState::APP_START_COLDBOOT)).Times(1);

    state->onEnter();
}

TEST_F(WaitForVpsStartCompleteTest, onEnter_start_Completed_shudownState)
{
    auto state = WaitForVpsStartComplete::getInstance();
    mContext.setVpsStartCompleted(true);
    ON_CALL(mMockPowerStateHolder, isShutdownState()).WillByDefault(Return(true));

    ASSERT_NE(WaitForAppStopVps::getInstance(), nullptr);
    EXPECT_CALL(mMockAndroidInit, propertySet(MELCO_SERVICES_START_COMPLETE_PROP_NAME,
                                              MELCO_SERVICES_START_COMPLETE_PROP_VALUE))
        .Times(1);
    EXPECT_CALL(mMockSM, transitionTo(WaitForAppStopVps::getInstance()));

    state->onEnter();
}

TEST_F(WaitForVpsStartCompleteTest, onEnter_start_Completed_NOT_shudownState)
{
    auto state = WaitForVpsStartComplete::getInstance();
    mContext.setVpsStartCompleted(true);
    ON_CALL(mMockPowerStateHolder, isShutdownState()).WillByDefault(Return(false));

    ASSERT_NE(NormalOperation::getInstance(), nullptr);
    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).Times(1);
    EXPECT_CALL(mMockVcpuActions, sendMainStartComplete()).Times(1);
    EXPECT_CALL(mMockAndroidInit, propertySet(MELCO_SERVICES_START_COMPLETE_PROP_NAME,
                                              MELCO_SERVICES_START_COMPLETE_PROP_VALUE))
        .Times(1);
    EXPECT_CALL(mMockSM, transitionTo(NormalOperation::getInstance()));

    state->onEnter();
}

TEST_F(WaitForVpsStartCompleteTest, onExit)
{
    auto state = WaitForVpsStartComplete::getInstance();
    mContext.setVpsDisconnected(true);
    mContext.setVpsStartCompleted(true);

    state->onExit();

    ASSERT_FALSE(mContext.isVpsDisconnected());
    ASSERT_FALSE(mContext.isVpsStartCompleted());
}

/**
 * Check transition from 'WaitForVpsStartComplete' to 'NormalOperation'
 * when 'onStartCompleteVps' method is called
 */
TEST_F(WaitForVpsStartCompleteTest, onStartCompleteVps)
{
    auto state = WaitForVpsStartComplete::getInstance();
    ASSERT_NE(NormalOperation::getInstance(), nullptr);
    EXPECT_CALL(mMockVcpuActions, sendMainStartComplete()).Times(1);
    EXPECT_CALL(mMockSM, transitionTo(NormalOperation::getInstance()));

    state->onStartCompleteVps();
}

/**
 * Check transition from 'WaitForVpsStartComplete' to 'WaitForAppStopVps'
 * when 'onStartCompleteVps' method is called and current state is SHUTDOWN
 */
TEST_F(WaitForVpsStartCompleteTest, onStartCompleteVpsInShutdownState)
{
    auto state = WaitForVpsStartComplete::getInstance();
    ASSERT_NE(NormalOperation::getInstance(), nullptr);
    ASSERT_NE(WaitForAppStopVps::getInstance(), nullptr);

    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).WillRepeatedly(Return(true));
    EXPECT_CALL(mMockVcpuActions, sendMainStartComplete()).Times(0);
    EXPECT_CALL(mMockSM, transitionTo(WaitForAppStopVps::getInstance()));

    state->onStartCompleteVps();
}

/**
 * Test stateName() method
 */
TEST_F(WaitForVpsStartCompleteTest, stateName)
{
    auto state = WaitForVpsStartComplete::getInstance();

    ASSERT_STREQ(state->stateName().c_str(), "WaitForVpsStartComplete");
}

TEST_F(WaitForVpsStartCompleteTest, getInstance)
{
    auto state = WaitForVpsStartComplete::getInstance();
    ASSERT_NE(state, nullptr);
}

TEST_F(WaitForVpsStartCompleteTest, uninitInstance)
{
    WaitForVpsStartComplete::uninitInstance();
    auto state = WaitForVpsStartComplete::getInstance();

    ASSERT_EQ(state, nullptr);
}

TEST_F(WaitForVpsStartCompleteTest,
       must_set_boot_completed_propery_on_start_complete_vps_in_shutdown)
{
    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).WillRepeatedly(Return(true));
    EXPECT_CALL(mMockAndroidInit, propertySet(Eq(MELCO_SERVICES_START_COMPLETE_PROP_NAME),
                                              Eq(MELCO_SERVICES_START_COMPLETE_PROP_VALUE)))
        .Times(1);

    auto state = WaitForVpsStartComplete::getInstance();
    state->onStartCompleteVps();
}

TEST_F(WaitForVpsStartCompleteTest,
       must_set_boot_completed_propery_on_start_complete_vps_not_in_shutdown)
{
    EXPECT_CALL(mMockPowerStateHolder, isShutdownState()).WillRepeatedly(Return(false));
    EXPECT_CALL(mMockAndroidInit, propertySet(Eq(MELCO_SERVICES_START_COMPLETE_PROP_NAME),
                                              Eq(MELCO_SERVICES_START_COMPLETE_PROP_VALUE)))
        .Times(1);

    auto state = WaitForVpsStartComplete::getInstance();
    state->onStartCompleteVps();
}

TEST_F(WaitForVpsStartCompleteTest, startVpsFailed)
{
    EXPECT_CALL(mMockVcpuActions, sendSelfRebootTrue()).Times(1);

    auto state = WaitForVpsStartComplete::getInstance();
    state->onStartFailedVps();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
