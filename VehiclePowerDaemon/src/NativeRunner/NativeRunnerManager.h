/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NATIVERUNNERMANAGER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NATIVERUNNERMANAGER_H_

#include "ICommandResult.h"
#include "IConfigScriptCmdCreator.h"
#include "INativeConfigProvider.h"
#include "INativeRunner.h"

#include <memory>
#include <string>
#include <vector>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class NativeRunnerManager : public INativeRunner {
public:
    NativeRunnerManager(INativeConfigProvider&, IConfigScriptCmdCreator&);

    void run() override;
    bool isDone() override;

private:
    INativeConfigProvider& mConfigProvider;
    IConfigScriptCmdCreator& mConfigScriptCmdCreator;
    std::vector<std::unique_ptr<ICommandResult>> mCommandResults;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NATIVERUNNERMANAGER_H_
