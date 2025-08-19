/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STARTNATIVECOMMAND_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STARTNATIVECOMMAND_H_

#include "IAndroidInit.h"
#include "ICommandResult.h"
#include "IConfigScriptCommand.h"

#include <memory>
#include <string>
#include <vector>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class StartNativeCommand : public IConfigScriptCommand {
public:
    StartNativeCommand(const std::string&, const std::vector<std::string>&, const IAndroidInit&);

    std::unique_ptr<ICommandResult> execute() override;

private:
    const std::string mPropertyName;
    const std::vector<std::string> mServices;

    const IAndroidInit& mAndroidInit;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STARTNATIVECOMMAND_H_
