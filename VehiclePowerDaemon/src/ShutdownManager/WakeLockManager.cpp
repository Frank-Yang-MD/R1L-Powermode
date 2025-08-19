/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WakeLockManager.h"
#include "VehiclePowerDaemonLog.h"

#include <errno.h>
#include <hardware_legacy/power.h>
#include <string.h>

#include <fstream>

const char* WAKE_LOCK_ID = "VehiclePowerDaemon";

const std::string WAKE_LOCK_MODE_CONTROL_NODE = "/sys/power/wake_ignore";

const std::string DISABLE_WAKE_LOCK_VALUE = "1";
const std::string ENABLE_WAKE_LOCK_VALUE = "0";

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

void WakeLockManager::acquireWakeLock()
{
    MLOGD(LogFuncId::VPD, LogId::WakeLockManager_acquireWakeLock);
    int res = acquire_wake_lock(PARTIAL_WAKE_LOCK, WAKE_LOCK_ID);

    if (res == 0) {
        MLOGD(LogFuncId::VPD, LogId::WakeLockManager_acquireWakeLock_Wake_lock_acquired);
    }
    else {
        MLOGF(LogFuncId::VPD,
              LogId::WakeLockManager_acquireWakeLock_Cant_aquire_wakelock_error_code, res);
        MLOGF(LogFuncId::VPD, LogId::WakeLockManager_acquireWakeLock_Cant_aquire_wakelock_errno,
              errno, strerror(errno));
    }
}

void WakeLockManager::releaseWakeLock()
{
    MLOGD(LogFuncId::VPD, LogId::WakeLockManager_releaseWakeLock);

    int res = release_wake_lock(WAKE_LOCK_ID);

    if (res == 0) {
        MLOGD(LogFuncId::VPD, LogId::WakeLockManager_releaseWakeLock_Wake_lock_released);
    }
    else {
        MLOGW(LogFuncId::VPD,
              LogId::WakeLockManager_releaseWakeLock_Cant_release_wakelock_error_code, res);
        MLOGW(LogFuncId::VPD, LogId::WakeLockManager_releaseWakeLock_Cant_release_wakelock_errno,
              errno, strerror(errno));
    }
}

void WakeLockManager::enableWakeLocks()
{
    MLOGI(LogFuncId::VPD, LogId::WakeLockManager_enableWakeLocks);

    std::ofstream{WAKE_LOCK_MODE_CONTROL_NODE} << ENABLE_WAKE_LOCK_VALUE;

    if (errno) {
        MLOGW(LogFuncId::VPD, LogId::WakeLockManager_enableWakeLocks_Cant_enable_wakelocks, errno,
              strerror(errno));
    }
}

void WakeLockManager::disableWakeLocks()
{
    MLOGI(LogFuncId::VPD, LogId::WakeLockManager_disableWakeLocks);

    std::ofstream{WAKE_LOCK_MODE_CONTROL_NODE} << DISABLE_WAKE_LOCK_VALUE;

    if (errno) {
        MLOGW(LogFuncId::VPD, LogId::WakeLockManager_disableWakeLocks_Cant_disable_wakelocks, errno,
              strerror(errno));
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
