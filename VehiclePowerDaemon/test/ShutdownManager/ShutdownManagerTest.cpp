/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MockAndroidPowerManager.h"
#include "MockPowerStateHolder.h"

#include <IShutdownAction.h>
#include <ShutdownManager.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using namespace ::testing;

class ShutdownManagerTest : public ::testing::Test {
public:
    ShutdownManagerTest()
        : mShutdownManager{mAndroidPowerManager, mMockPowerStateHolder}
        , mShutdownAction{mShutdownManager}
    {
    }

    MockAndroidPowerManager mAndroidPowerManager;
    ShutdownManager mShutdownManager;
    IShutdownAction& mShutdownAction;
    MockPowerStateHolder mMockPowerStateHolder;
};

TEST_F(ShutdownManagerTest, shutdownAction)
{
    EXPECT_CALL(mMockPowerStateHolder, getPowerState())
        .Times(1)
        .WillOnce(Return(PowerState::SHUTDOWN));

    EXPECT_CALL(mAndroidPowerManager, shutdown()).Times(1);

    mShutdownAction.performShutdownAction();
}

TEST_F(ShutdownManagerTest, suspendAction)
{
    EXPECT_CALL(mMockPowerStateHolder, getPowerState())
        .Times(1)
        .WillOnce(Return(PowerState::SUSPEND));

    EXPECT_CALL(mAndroidPowerManager, suspend()).Times(1);

    mShutdownAction.performShutdownAction();
}

TEST_F(ShutdownManagerTest, rebootAction)
{
    EXPECT_CALL(mMockPowerStateHolder, getPowerState())
        .Times(1)
        .WillOnce(Return(PowerState::REBOOT));

    EXPECT_CALL(mAndroidPowerManager, reboot()).Times(1);

    mShutdownAction.performShutdownAction();
}

TEST_F(ShutdownManagerTest, rebootRecoveryAction)
{
    EXPECT_CALL(mMockPowerStateHolder, getPowerState())
        .Times(1)
        .WillOnce(Return(PowerState::REBOOT_RECOVERY));

    EXPECT_CALL(mAndroidPowerManager, rebootRecovery()).Times(1);

    mShutdownAction.performShutdownAction();
}

TEST_F(ShutdownManagerTest, normalState)
{
    EXPECT_CALL(mMockPowerStateHolder, getPowerState())
        .Times(1)
        .WillOnce(Return(PowerState::NORMAL));

    EXPECT_CALL(mAndroidPowerManager, shutdown()).Times(0);
    EXPECT_CALL(mAndroidPowerManager, suspend()).Times(0);
    EXPECT_CALL(mAndroidPowerManager, reboot()).Times(0);
    EXPECT_CALL(mAndroidPowerManager, rebootRecovery()).Times(0);

    mShutdownAction.performShutdownAction();
}

TEST_F(ShutdownManagerTest, suspendCancelState)
{
    EXPECT_CALL(mMockPowerStateHolder, getPowerState())
        .Times(1)
        .WillOnce(Return(PowerState::SUSPEND_CANCEL));

    EXPECT_CALL(mAndroidPowerManager, shutdown()).Times(0);
    EXPECT_CALL(mAndroidPowerManager, suspend()).Times(0);
    EXPECT_CALL(mAndroidPowerManager, reboot()).Times(0);
    EXPECT_CALL(mAndroidPowerManager, rebootRecovery()).Times(0);

    mShutdownAction.performShutdownAction();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
