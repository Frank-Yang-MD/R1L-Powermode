/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VcpuPowerStateChangeHandler.h"

#include <VehiclePowerDaemonLog.h>

#include <string>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

VcpuPowerStateChangeHandler::VcpuPowerStateChangeHandler(
    IPowerStateChangeListener& stateMachine,
    IPowerStateChangeListener& powerStateHolder,
    IPowerStateChangeListener& powerStateChangeListener)
    : mStateMachine{stateMachine}
    , mPowerStateHolder{powerStateHolder}
    , mPowerStateChangeListener{powerStateChangeListener}
{
}

void VcpuPowerStateChangeHandler::Process(int, common::EVpdEvent vpdEvent)
{
    const auto [powerState, isValid] = vpdEventToPowerState(vpdEvent);
    if (isValid) {
        mPowerStateHolder.onPowerStateChange(powerState);
        mStateMachine.onPowerStateChange(powerState);
        mPowerStateChangeListener.onPowerStateChange(powerState);
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::VcpuPowerStateChangeHandler_Invalid_Vpd_state,
              std::to_string(vpdEvent).c_str());
    }
}

std::pair<PowerState, bool> VcpuPowerStateChangeHandler::vpdEventToPowerState(
    common::EVpdEvent vpdEvent) const
{
    PowerState powerState = PowerState::NORMAL;
    bool isValid = true;

    switch (vpdEvent) {
    case common::EVpdEvent::ShutdownCancel:
        powerState = PowerState::SUSPEND_CANCEL;
        break;
    case common::EVpdEvent::Suspend:
        powerState = PowerState::SUSPEND;
        break;
    case common::EVpdEvent::Shutdown:
        powerState = PowerState::SHUTDOWN;
        break;
    case common::EVpdEvent::Reboot:
        powerState = PowerState::REBOOT;
        break;
    case common::EVpdEvent::RebootRecovery:
        powerState = PowerState::REBOOT_RECOVERY;
        break;
    default:
        isValid = false;
        break;
    }

    return std::make_pair(powerState, isValid);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
