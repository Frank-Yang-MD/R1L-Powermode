/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICONFIGSCRIPTCOMMAND_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICONFIGSCRIPTCOMMAND_H_

#include "ICommandResult.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IConfigScriptCommand {
public:
    // LCOV_EXCL_START
    virtual ~IConfigScriptCommand() = default;
    // LCOV_EXCL_STOP

    virtual std::unique_ptr<ICommandResult> execute() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICONFIGSCRIPTCOMMAND_H_
