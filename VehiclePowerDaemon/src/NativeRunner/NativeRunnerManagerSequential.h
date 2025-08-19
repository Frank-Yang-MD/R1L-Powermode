/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NATIVERUNNERMANAGERSEQUENTIAL_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NATIVERUNNERMANAGERSEQUENTIAL_H_

#include "ICommandResult.h"
#include "IConfigScriptCmdCreator.h"
#include "INativeConfigProvider.h"
#include "INativeRunner.h"

#include <memory>
#include <queue>
#include <string>
#include <utility>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using commands_t = std::pair<std::string, std::unique_ptr<IConfigScriptCommand>>;

/**
 * This class provides the ability to execute script commands sequentially
 */
class NativeRunnerManagerSequential : public INativeRunner {
public:
    NativeRunnerManagerSequential(INativeConfigProvider&, IConfigScriptCmdCreator&);

    void run() override;
    bool isDone() override;

private:
    INativeConfigProvider& mConfigProvider;
    IConfigScriptCmdCreator& mConfigScriptCmdCreator;

    std::queue<commands_t> mCommands;
    std::unique_ptr<ICommandResult> mCurrentCommandResult{nullptr};

    void executeCmd();
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NATIVERUNNERMANAGERSEQUENTIAL_H_
