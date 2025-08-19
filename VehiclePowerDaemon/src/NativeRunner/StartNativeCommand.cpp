/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "StartNativeCommand.h"
#include "StartNativeCommandResult.h"
#include "VehiclePowerDaemonLog.h"

#include <string>
#include <vector>

namespace {
const std::string propValue{"start"};
}

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

StartNativeCommand::StartNativeCommand(const std::string& propPame,
                                       const std::vector<std::string>& services,
                                       const IAndroidInit& androidInit)
    : mPropertyName(propPame)
    , mServices(services)
    , mAndroidInit(androidInit)
{
}

std::unique_ptr<ICommandResult> StartNativeCommand::execute()
{
    if (mAndroidInit.propertySet(mPropertyName, propValue)) {
        MLOGW(LogFuncId::VPD, LogId::StartNativeCommand_execute_Cant_set_property_to_value,
              "property '" + mPropertyName + "' to value '" + propValue + "'");
        return nullptr;
    }

    return std::make_unique<StartNativeCommandResult>(
        StartNativeCommandResult(mServices, mAndroidInit));
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
