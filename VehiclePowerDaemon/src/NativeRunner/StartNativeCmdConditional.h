/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STARTNATIVECMDCONDITIONAL_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STARTNATIVECMDCONDITIONAL_H_

#include "ICommandResult.h"
#include "ICondition.h"
#include "IConfigScriptCommand.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class StartNativeCmdConditional : public IConfigScriptCommand {
public:
    StartNativeCmdConditional(std::unique_ptr<IConfigScriptCommand>, std::unique_ptr<ICondition>);

    std::unique_ptr<ICommandResult> execute() override;

private:
    std::unique_ptr<IConfigScriptCommand> mConfigScriptCommand;
    std::unique_ptr<ICondition> mCondition;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STARTNATIVECMDCONDITIONAL_H_
