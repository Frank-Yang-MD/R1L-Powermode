/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "NativeRunnerManager.h"
#include "IConfigScriptCommand.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

NativeRunnerManager::NativeRunnerManager(INativeConfigProvider& configProvider,
                                         IConfigScriptCmdCreator& configScriptCmdCreator)
    : mConfigProvider(configProvider)
    , mConfigScriptCmdCreator(configScriptCmdCreator)
{
}

void NativeRunnerManager::run()
{
    std::vector<std::string> config = mConfigProvider.loadConfig();
    for (auto& line : config) {
        std::unique_ptr<IConfigScriptCommand> cmd = mConfigScriptCmdCreator.createCommand(line);
        if (cmd == nullptr) {
            MLOGW(LogFuncId::VPD, LogId::NativeRunnerManager_run_Cant_get_command_from_line, line);
            continue;
        }
        std::unique_ptr<ICommandResult> cmdResult = cmd->execute();
        if (cmdResult == nullptr) {
            MLOGW(LogFuncId::VPD, LogId::NativeRunnerManager_run_Cant_get_command_result_from_line,
                  line);
            continue;
        }
        mCommandResults.push_back(std::move(cmdResult));
    }  // LCOV_EXCL_LINE
}

bool NativeRunnerManager::isDone()
{
    for (auto const& cmdResult : mCommandResults) {
        if (!cmdResult->isDone()) {
            return false;
        }
    }
    return true;
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
