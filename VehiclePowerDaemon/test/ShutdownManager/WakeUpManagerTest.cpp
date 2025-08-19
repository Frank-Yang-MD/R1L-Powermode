/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MockAndroidPowerManager.h"

#include <WakeUpManager.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class WakeUpManagerTest : public ::testing::Test {
public:
    WakeUpManagerTest()
        : mWakeUpManager{mAndroidPowerManager}
    {
    }

    MockAndroidPowerManager mAndroidPowerManager;
    WakeUpManager mWakeUpManager;
};

TEST_F(WakeUpManagerTest, acquireWakeLock)
{
    EXPECT_CALL(mAndroidPowerManager, wakeUp()).Times(1);

    mWakeUpManager.wakeUp();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
