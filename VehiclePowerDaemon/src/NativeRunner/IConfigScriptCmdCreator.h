/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICONFIGSCRIPTCMDCREATOR_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICONFIGSCRIPTCMDCREATOR_H_

#include "IConfigScriptCommand.h"

#include <memory>
#include <string>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IConfigScriptCmdCreator {
public:
    // LCOV_EXCL_START
    virtual ~IConfigScriptCmdCreator() = default;
    // LCOV_EXCL_STOP

    virtual std::unique_ptr<IConfigScriptCommand> createCommand(const std::string&) = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICONFIGSCRIPTCMDCREATOR_H_
