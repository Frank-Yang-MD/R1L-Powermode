/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpmBridgeDaemonAdapter.h"
#include "VehiclePowerDaemonLog.h"

#include <cutils/properties.h>
#include <hardware_legacy/power.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

bool VpmBridgeDaemonAdapter::acquireVpmBridgeDaemon()
{
    bool result = true;

    if (mVpmBridgeDaemon == nullptr) {
        mVpmBridgeDaemon = IVpmBridgeDaemon::getService();
        if (mVpmBridgeDaemon == nullptr) {
            result = false;
        }
    }

    return result;
}

void VpmBridgeDaemonAdapter::shutdown()
{
    if (!acquireVpmBridgeDaemon()) {
        MLOGW(LogFuncId::VPD, LogId::VpmBridgeDaemonAdapter_Cannot_acquire_VpmBridgeDaemon_instance,
              __func__);
        return;
    }
    MLOGD(LogFuncId::VPD, LogId::VpmBridgeDaemonAdapter_Request_to_VpmBridgeDaemon, __func__);
    mVpmBridgeDaemon->shutdown();
}

void VpmBridgeDaemonAdapter::suspend()
{
    if (!acquireVpmBridgeDaemon()) {
        MLOGW(LogFuncId::VPD, LogId::VpmBridgeDaemonAdapter_Cannot_acquire_VpmBridgeDaemon_instance,
              __func__);
        return;
    }

    MLOGD(LogFuncId::VPD, LogId::VpmBridgeDaemonAdapter_Request_to_VpmBridgeDaemon, __func__);
    mVpmBridgeDaemon->suspend();
}

void VpmBridgeDaemonAdapter::reboot()
{
    if (!acquireVpmBridgeDaemon()) {
        MLOGW(LogFuncId::VPD, LogId::VpmBridgeDaemonAdapter_Cannot_acquire_VpmBridgeDaemon_instance,
              __func__);
        return;
    }
    MLOGD(LogFuncId::VPD, LogId::VpmBridgeDaemonAdapter_Request_to_VpmBridgeDaemon, __func__);
    mVpmBridgeDaemon->reboot();
}

void VpmBridgeDaemonAdapter::rebootRecovery()
{
    if (!acquireVpmBridgeDaemon()) {
        MLOGW(LogFuncId::VPD, LogId::VpmBridgeDaemonAdapter_Cannot_acquire_VpmBridgeDaemon_instance,
              __func__);
        return;
    }
    MLOGD(LogFuncId::VPD, LogId::VpmBridgeDaemonAdapter_Request_to_VpmBridgeDaemon, __func__);
    mVpmBridgeDaemon->rebootRecovery();
}

void VpmBridgeDaemonAdapter::wakeUp()
{
    if (!acquireVpmBridgeDaemon()) {
        MLOGW(LogFuncId::VPD, LogId::VpmBridgeDaemonAdapter_Cannot_acquire_VpmBridgeDaemon_instance,
              __func__);
        return;
    }
    MLOGD(LogFuncId::VPD, LogId::VpmBridgeDaemonAdapter_Request_to_VpmBridgeDaemon, __func__);
    mVpmBridgeDaemon->wakeUp();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
