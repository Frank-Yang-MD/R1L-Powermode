/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpmBridgeDaemonLog.h"
#include "Log.h"
#include <string>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace vpmbridgedaemon {

void InitializeLogMessages()
{
    using com::mitsubishielectric::ahu::common::LogMessageFormats;
    using common::DisplayTypeString;
    const LogMessageFormats VpmBridgeDaemonLogMessages = {
        // clang-format off
        {LogId::VpmBridgeDaemon_API_call,
        "VpmBridgeDaemon.VpmBridgeDaemon: %s API call", {DisplayTypeString(1024, "Parameter")}},
        {LogId::AndroidPmAdapter_Android_PM_binder_is_null,
        "VpmBridgeDaemon.AndroidPmAdapter: Android PM binder is null"},
        {LogId::AndroidPmAdapter_Cannot_cast_to_Android_PM_instance,
        "VpmBridgeDaemon.AndroidPmAdapter: Cannot cast binder to Android PM instance"},
        {LogId::AndroidPmAdapter_Android_PM_instance_obtained,
        "VpmBridgeDaemon.AndroidPmAdapter: Android PM instance obtained"},
        {LogId::AndroidPmAdapter_Cannot_obtain_Android_PM_instance,
        "VpmBridgeDaemon.AndroidPmAdapter: Cannot obtain Android PM instance"},
        {LogId::AndroidPmAdapter_Request_to_Android_PM,
        "VpmBridgeDaemon.AndroidPmAdapter: %s: Request to Android PM", {DisplayTypeString(1024, "Parameter")}},
        {LogId::AndroidPmAdapter_Request_to_Android_PM_failed,
        "VpmBridgeDaemon.AndroidPmAdapter: Request to Android PM FAILED!"},
        {LogId::Main_Started_successfully,
        "VpmBridgeDaemon.Main: Started successfully"},
        {LogId::Main_Cannot_register_service,
        "VpmBridgeDaemon.Main: Cannot register service"},
        // clang-format on
    };
    using com::mitsubishielectric::ahu::common::InitializeFunctionID;
    InitializeFunctionID(LogFuncId::VPBD, VpmBridgeDaemonLogMessages);
}

void TerminateLogMessages()
{
    using com::mitsubishielectric::ahu::common::TerminateFunctionID;
    TerminateFunctionID(LogFuncId::VPBD);
}

}  // namespace vpmbridgedaemon
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
