/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "NativeRunnerManagerSequential.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

NativeRunnerManagerSequential::NativeRunnerManagerSequential(
    INativeConfigProvider& configProvider,
    IConfigScriptCmdCreator& configScriptCmdCreator)
    : mConfigProvider{configProvider}
    , mConfigScriptCmdCreator{configScriptCmdCreator}
{
}

void NativeRunnerManagerSequential::run()
{
    auto config = mConfigProvider.loadConfig();
    if (!config.size()) {
        MLOGW(LogFuncId::VPD, LogId::NativeRunnerManagerSequential_run_Config_is_empty);
    }

    for (const auto& line : config) {
        auto cmd = mConfigScriptCmdCreator.createCommand(line);
        if (cmd == nullptr) {
            MLOGW(LogFuncId::VPD,
                  LogId::NativeRunnerManagerSequential_run_Cant_get_command_from_line, line);
        }
        else {
            mCommands.push(std::make_pair(line, std::move(cmd)));
        }
    }

    if (mCommands.empty()) {
        MLOGW(LogFuncId::VPD,
              LogId::NativeRunnerManagerSequential_run_List_with_commands_to_execute_is_empty);
    }
    else {
        executeCmd();
    }
}

bool NativeRunnerManagerSequential::isDone()
{
    bool isDoneResult{false};

    if (mCommands.empty()) {
        isDoneResult = mCurrentCommandResult ? mCurrentCommandResult->isDone() : true;
    }
    else if (!mCurrentCommandResult || mCurrentCommandResult->isDone()) {
        executeCmd();
    }

    return isDoneResult;
}

void NativeRunnerManagerSequential::executeCmd()
{
    const auto& cmd = mCommands.front();
    MLOGI(LogFuncId::VPD, LogId::NativeRunnerManagerSequential_executeCmd_Run_command, cmd.first);

    mCurrentCommandResult = cmd.second->execute();
    if (!mCurrentCommandResult) {
        MLOGW(LogFuncId::VPD,
              LogId::NativeRunnerManagerSequential_executeCmd_Cant_get_command_result_from_line,
              cmd.first);
    }
    mCommands.pop();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
