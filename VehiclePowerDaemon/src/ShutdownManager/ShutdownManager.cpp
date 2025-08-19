/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ShutdownManager.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

ShutdownManager::ShutdownManager(IAndroidPowerManager& androidPowerManager,
                                 IPowerStateHolder& powerStateHolder)
    : mAndroidPowerManager{androidPowerManager}
    , mPowerStateHolder{powerStateHolder}
{
}

void ShutdownManager::performShutdownAction()
{
    MLOGI(LogFuncId::VPD, LogId::ShutdownManager_performShutdownAction);

    const auto powerState = mPowerStateHolder.getPowerState();

    switch (powerState) {
    case PowerState::SUSPEND:
        mAndroidPowerManager.suspend();
        break;
    case PowerState::SHUTDOWN:
        mAndroidPowerManager.shutdown();
        break;
    case PowerState::REBOOT:
        mAndroidPowerManager.reboot();
        break;
    case PowerState::REBOOT_RECOVERY:
        mAndroidPowerManager.rebootRecovery();
        break;
    default:

        MLOGW(LogFuncId::VPD,
              LogId::ShutdownManager_performShutdownAction_Undefined_shutdown_action);
        break;
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
