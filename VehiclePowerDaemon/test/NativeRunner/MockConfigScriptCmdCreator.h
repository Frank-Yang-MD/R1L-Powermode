/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKCONFIGSCRIPTCMDCREATOR_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKCONFIGSCRIPTCMDCREATOR_H_

#include "IConfigScriptCmdCreator.h"

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockConfigScriptCmdCreator : public IConfigScriptCmdCreator {
public:
    MOCK_METHOD1(createCommand, std::unique_ptr<IConfigScriptCommand>(const std::string&));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKCONFIGSCRIPTCMDCREATOR_H_
