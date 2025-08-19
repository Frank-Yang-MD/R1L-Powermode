/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WakeUpManager.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

WakeUpManager::WakeUpManager(IAndroidPowerManager& androidPowerManager)
    : mAndroidPowerManager{androidPowerManager}
{
}

void WakeUpManager::wakeUp()
{
    MLOGI(LogFuncId::VPD, LogId::WakeUpManager_wakeUp);
    mAndroidPowerManager.wakeUp();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
