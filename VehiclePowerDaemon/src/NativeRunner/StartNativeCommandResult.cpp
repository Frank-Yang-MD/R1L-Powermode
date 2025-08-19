/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "StartNativeCommandResult.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

StartNativeCommandResult::StartNativeCommandResult(const std::vector<std::string>& services,
                                                   const IAndroidInit& androidInit)
    : mServices(services)
    , mAndroidInit(androidInit)
{
}

bool StartNativeCommandResult::isDone()
{
    std::string propertyValue;

    for (auto it = mServices.begin(); it != mServices.end();) {
        int prop_length = mAndroidInit.propertyGet(*it, propertyValue);
        if (prop_length > 0 && propertyValue == "running") {
            MLOGI(LogFuncId::VPD, LogId::StartNativeCommandResult_isDone_service_state,
                  "Service '" + *it + "', state '" + propertyValue + "'");
            it = mServices.erase(it);
        }
        else {
            ++it;
        }
    }

    return mServices.empty();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
