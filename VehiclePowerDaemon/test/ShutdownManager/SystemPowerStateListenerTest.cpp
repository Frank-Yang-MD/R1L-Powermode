/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "MockActions.h"
#include "MockTimerManager.h"

#include "SystemPowerStateListener.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::InSequence;
using ::testing::StrictMock;
using ::testing::Test;

/**
 * Unit test for SystemPowerStateListener class
 */
class SystemPowerStateListenerTest : public Test {
public:
    SystemPowerStateListenerTest()
        : mSystemPowerStateListener{mMockVcpuActions, mMockTimerManager}
    {
    }

    ~SystemPowerStateListenerTest();

protected:
    /**
     * StrictMock - will fail on any "uninteresting" call.
     *
     * Tests: onPowerStateChange_Normal, onPowerStateChange_WaitForCpuComDelivery
     */
    StrictMock<MockVcpuActions> mMockVcpuActions;
    StrictMock<MockTimerManager> mMockTimerManager;

    SystemPowerStateListener mSystemPowerStateListener;
};

// Define empty desctructor to supress out-of-line definition warning
SystemPowerStateListenerTest::~SystemPowerStateListenerTest() {}

TEST_F(SystemPowerStateListenerTest, onPowerStateChange_SuspendCancel)
{
    InSequence seq;

    EXPECT_CALL(mMockVcpuActions, sendShutdownCancelAccept()).Times(1);
    EXPECT_CALL(mMockTimerManager, changeTimerHandler(ETimerType::WDTRTimer)).Times(1);

    mSystemPowerStateListener.onPowerStateChange(PowerState::SUSPEND_CANCEL);
}

TEST_F(SystemPowerStateListenerTest, onPowerStateChange_Shutdown)
{
    EXPECT_CALL(mMockTimerManager, changeTimerHandler(ETimerType::ADTRTimer)).Times(1);

    mSystemPowerStateListener.onPowerStateChange(PowerState::SHUTDOWN);
}

TEST_F(SystemPowerStateListenerTest, onPowerStateChange_Suspend)
{
    EXPECT_CALL(mMockTimerManager, changeTimerHandler(ETimerType::ADTRTimer)).Times(1);

    mSystemPowerStateListener.onPowerStateChange(PowerState::SUSPEND);
}

TEST_F(SystemPowerStateListenerTest, onPowerStateChange_Reboot)
{
    EXPECT_CALL(mMockTimerManager, changeTimerHandler(ETimerType::ADTRTimer)).Times(1);

    mSystemPowerStateListener.onPowerStateChange(PowerState::REBOOT);
}

TEST_F(SystemPowerStateListenerTest, onPowerStateChange_RebootRecovery)
{
    EXPECT_CALL(mMockTimerManager, changeTimerHandler(ETimerType::ADTRTimer)).Times(1);

    mSystemPowerStateListener.onPowerStateChange(PowerState::REBOOT_RECOVERY);
}

TEST_F(SystemPowerStateListenerTest, onPowerStateChange_Normal)
{
    // StrictMock will check that no mocks are called
    mSystemPowerStateListener.onPowerStateChange(PowerState::NORMAL);
}

TEST_F(SystemPowerStateListenerTest, onPowerStateChange_WaitForCpuComDelivery)
{
    // StrictMock will check that no mocks are called
    mSystemPowerStateListener.onPowerStateChange(PowerState::WAIT_FOR_CPUCOM_DELIVERY);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
