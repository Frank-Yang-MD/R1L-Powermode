/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "PowerStateHolder.h"

#include <VehiclePowerDaemonLog.h>

#include <string>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::string powerStateToString(PowerState powerState)
{
    std::string powerStateStr;

    switch (powerState) {
    case PowerState::NORMAL:
        powerStateStr = "NORMAL";
        break;
    case PowerState::SUSPEND_CANCEL:
        powerStateStr = "SUSPEND_CANCEL";
        break;
    case PowerState::SUSPEND:
        powerStateStr = "SUSPEND";
        break;
    case PowerState::SHUTDOWN:
        powerStateStr = "SHUTDOWN";
        break;
    case PowerState::REBOOT:
        powerStateStr = "REBOOT";
        break;
    case PowerState::REBOOT_RECOVERY:
        powerStateStr = "NORMAL";
        break;
    case PowerState::WAIT_FOR_CPUCOM_DELIVERY:
        powerStateStr = "WAIT_FOR_CPUCOM_DELIVERY";
        break;
    default:
        powerStateStr = "UNKNOWN";
        break;
    }

    return powerStateStr;
}  // LCOV_EXCL_LINE

PowerStateHolder::PowerStateHolder()
    : mPowerState{PowerState::NORMAL}
{
}

void PowerStateHolder::onPowerStateChange(PowerState powerState)
{
    MLOGI(LogFuncId::VPD, LogId::PowerStateHolder_onPowerStateChange,
          powerStateToString(powerState));
    mPowerState = powerState;
}

bool PowerStateHolder::isShutdownState() const
{
    return !(mPowerState == PowerState::NORMAL || mPowerState == PowerState::SUSPEND_CANCEL);
}

PowerState PowerStateHolder::getPowerState() const { return mPowerState; }

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
