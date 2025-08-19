/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gtest/gtest.h>

#include "PowerStateHolder.h"

using namespace ::testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

const static std::vector<PowerState> shutdownStates = {
    PowerState::SUSPEND,
    PowerState::SHUTDOWN,
    PowerState::REBOOT,
    PowerState::REBOOT_RECOVERY,
};

const static std::vector<PowerState> notShutdownStates = {
    PowerState::NORMAL,
    PowerState::SUSPEND_CANCEL,
};

std::string powerStateToString(PowerState powerState);

class PowerStateHolderTest : public ::testing::Test {
public:
    ~PowerStateHolderTest();

protected:
    PowerStateHolder mPowerStateHolder;
};

// Define empty desctructor to supress out-of-line definition warning
PowerStateHolderTest::~PowerStateHolderTest() {}

TEST_F(PowerStateHolderTest, initialState)
{
    EXPECT_EQ(mPowerStateHolder.getPowerState(), PowerState::NORMAL);
}

TEST_F(PowerStateHolderTest, correctlyDetermineNotShutdownState)
{
    for (const auto& state : notShutdownStates) {
        mPowerStateHolder.onPowerStateChange(state);
        EXPECT_EQ(mPowerStateHolder.isShutdownState(), false);
    }
}

TEST_F(PowerStateHolderTest, correctlyDetermineShutdownState)
{
    for (const auto& state : shutdownStates) {
        mPowerStateHolder.onPowerStateChange(state);
        EXPECT_EQ(mPowerStateHolder.isShutdownState(), true);
    }
}

TEST_F(PowerStateHolderTest, powerStateToString_returns_UNKNOWN_if_wrong_state)
{
    std::string val;
    val = powerStateToString(static_cast<PowerState>(255));
    EXPECT_STREQ("UNKNOWN", val.c_str());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
