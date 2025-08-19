/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MockActions.h"
#include "MockAndroidInit.h"
#include "MockCpuCommControl.h"
#include "MockNativeRunner.h"
#include "MockNotifyHelper.h"
#include "MockSM.h"
#include "MockTimerHandler.h"
#include "MockTimerManager.h"
#include "VpdStateMachine.h"
#include "WaitForNativeServicesStart.h"
#include "WaitForStartCpuCom.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

const std::string START_PROP_NAME("vendor.cpucomdaemon");
const std::string START_PROP_VALUE("start");

/**
 * Contains all data needed for WaitForStartCpuCom tests
 */
class WaitForStartCpuComTestData {
public:
    WaitForStartCpuComTestData()
    {
        WaitForStartCpuCom::initInstance(&mMockSM, mMockAndroidInit, mMockCpuCommControl);
        WaitForNativeServicesStart::initInstance(&mMockSM, mMockVcpuActions, mMockNativeRunner,
                                                 mMockCheckServicesTimer, mMockTimerManager,
                                                 mMockNotifyHelper);
    }
    ~WaitForStartCpuComTestData() { WaitForStartCpuCom::uninitInstance(); }

    MockAndroidInit& mockAndroidInit() { return mMockAndroidInit; }
    MockCpuCommControl& mockCpuCommControl() { return mMockCpuCommControl; }
    MockSM& mockSM() { return mMockSM; }

private:
    MockAndroidInit mMockAndroidInit;
    MockCpuCommControl mMockCpuCommControl;
    MockSM mMockSM;
    MockVcpuActions mMockVcpuActions;
    MockTimer mMockCheckServicesTimer;
    MockNativeRunner mMockNativeRunner;
    MockTimerManager mMockTimerManager;
    MockNotifyHelper mMockNotifyHelper;
};

/**
 * Test Fixture Object
 */
class WaitForStartCpuComTest : public ::testing::Test {
public:
    WaitForStartCpuComTestData* data() { return mData.get(); }

protected:
    void SetUp() { mData.reset(new WaitForStartCpuComTestData()); }
    void TearDown() { mData.reset(nullptr); }

private:
    std::unique_ptr<WaitForStartCpuComTestData> mData;
};

/**
 * Check if a trigger enabling native services start is set
 * at the moment SM is entering 'WaitForStartCpuCom' state
 */
TEST_F(WaitForStartCpuComTest, onEnter)
{
    auto state = WaitForStartCpuCom::getInstance();
    EXPECT_CALL(data()->mockAndroidInit(), propertySet(START_PROP_NAME, START_PROP_VALUE)).Times(1);
    EXPECT_CALL(data()->mockCpuCommControl(), stop()).Times(1);

    state->onEnter();
}

/**
 * Check transition from 'WaitForStartCpuCom' state to 'WaitNativeAppStart'
 * when 'onStartCompleteCpuCom' method is called
 */
TEST_F(WaitForStartCpuComTest, onStartCompleteCpuCom)
{
    auto state = WaitForStartCpuCom::getInstance();
    EXPECT_CALL(data()->mockCpuCommControl(), start()).Times(1);
    ASSERT_NE(WaitForNativeServicesStart::getInstance(), nullptr);
    EXPECT_CALL(data()->mockSM(), transitionTo(WaitForNativeServicesStart::getInstance()));
    EXPECT_CALL(data()->mockCpuCommControl(), stop()).Times(1);

    state->onStartCompleteCpuCom();
}

/**
 * Test stateName() method
 */
TEST_F(WaitForStartCpuComTest, stateName)
{
    auto state = WaitForStartCpuCom::getInstance();
    ASSERT_STREQ(state->stateName().c_str(), "WaitForStartCpuCom");
    EXPECT_CALL(data()->mockCpuCommControl(), stop()).Times(1);
}

TEST_F(WaitForStartCpuComTest, getInstance)
{
    auto state = WaitForStartCpuCom::getInstance();
    ASSERT_NE(state, nullptr);
    EXPECT_CALL(data()->mockCpuCommControl(), stop()).Times(1);
}

TEST_F(WaitForStartCpuComTest, uninitInstance)
{
    EXPECT_CALL(data()->mockCpuCommControl(), stop()).Times(1);
    WaitForStartCpuCom::uninitInstance();
    auto state = WaitForStartCpuCom::getInstance();

    ASSERT_EQ(state, nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
