#include "StartNativeCmdConditional.h"
#include "StartNativeCmdConditionalResult.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

StartNativeCmdConditional::StartNativeCmdConditional(
    std::unique_ptr<IConfigScriptCommand> configScriptCommand,
    std::unique_ptr<ICondition> condition)
    : mConfigScriptCommand{std::move(configScriptCommand)}
    , mCondition{std::move(condition)}
{
}

std::unique_ptr<ICommandResult> StartNativeCmdConditional::execute()
{
    std::unique_ptr<ICommandResult> cmdResult;

    if (mCondition->isSatisfied()) {
        cmdResult = mConfigScriptCommand->execute();
    }
    else {
        MLOGW(LogFuncId::VPD,
              LogId::StartNativeCmdConditional_execute_Condition_is_not_satisfied_cmd_not_executed);
        cmdResult =
            std::make_unique<StartNativeCmdConditionalResult>(StartNativeCmdConditionalResult{});
    }

    return cmdResult;
}  // LCOV_EXCL_LINE

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
