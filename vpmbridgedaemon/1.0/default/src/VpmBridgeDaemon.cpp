/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpmBridgeDaemon.h"
#include "VpmBridgeDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace vpmbridgedaemon {
namespace V1_0 {
namespace implementation {

VpmBridgeDaemon::VpmBridgeDaemon(AndroidPmAdapter& androidPmAdapter)
    : mAndroidPmAdapter{androidPmAdapter}
{
}

// Methods from IVpmBridgeDaemon follow.
Return<void> VpmBridgeDaemon::shutdown()
{
    MLOGI(LogFuncId::VPBD, LogId::VpmBridgeDaemon_API_call, __func__);
    mAndroidPmAdapter.shutdown();
    return Void();
}

Return<void> VpmBridgeDaemon::suspend()
{
    MLOGI(LogFuncId::VPBD, LogId::VpmBridgeDaemon_API_call, __func__);
    mAndroidPmAdapter.suspend();
    return Void();
}

Return<void> VpmBridgeDaemon::reboot()
{
    MLOGI(LogFuncId::VPBD, LogId::VpmBridgeDaemon_API_call, __func__);
    mAndroidPmAdapter.reboot();
    return Void();
}

Return<void> VpmBridgeDaemon::rebootRecovery()
{
    MLOGI(LogFuncId::VPBD, LogId::VpmBridgeDaemon_API_call, __func__);
    mAndroidPmAdapter.rebootRecovery();
    return Void();
}

Return<void> VpmBridgeDaemon::wakeUp()
{
    MLOGI(LogFuncId::VPBD, LogId::VpmBridgeDaemon_API_call, __func__);
    mAndroidPmAdapter.wakeUp();
    return Void();
}

}  // namespace implementation
}  // namespace V1_0
}  // namespace vpmbridgedaemon
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
