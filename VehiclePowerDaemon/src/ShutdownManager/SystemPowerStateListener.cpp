/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "SystemPowerStateListener.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

inline bool isShutdownState(const PowerState ps)
{
    return (ps == PowerState::SHUTDOWN) || (ps == PowerState::SUSPEND) ||
           (ps == PowerState::REBOOT) || (ps == PowerState::REBOOT_RECOVERY);
}

SystemPowerStateListener::SystemPowerStateListener(IVcpuActions& vcpuActions,
                                                   ITimerManager& timerManager)
    : mVcpuActions{vcpuActions}
    , mTimerManager{timerManager}
{
}

void SystemPowerStateListener::onPowerStateChange(PowerState powerState)
{
    if (powerState == PowerState::SUSPEND_CANCEL) {
        MLOGI(LogFuncId::VPD,
              LogId::SystemPowerStateListener_onPowerStateChange_sendShutdownCancelAccept);
        mVcpuActions.sendShutdownCancelAccept();
        mTimerManager.changeTimerHandler(ETimerType::WDTRTimer);
    }
    else if (isShutdownState(powerState)) {
        mTimerManager.changeTimerHandler(ETimerType::ADTRTimer);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com