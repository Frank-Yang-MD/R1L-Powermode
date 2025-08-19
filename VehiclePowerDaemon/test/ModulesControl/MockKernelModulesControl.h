/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKKERNELMODULECONTROL_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKKERNELMODULECONTROL_H_

#include <IKernelModulesControl.h>
#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockKernelModulesControl : public IKernelModulesControl {
public:
    MOCK_METHOD0(loadModules, void());
    MOCK_METHOD0(isModulesLoaded, bool());
    MOCK_METHOD0(unloadModules, void());
    MOCK_METHOD0(isModulesUnloaded, bool());
    MOCK_METHOD0(skipModulesUnloadedCheck, bool());
};
}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKKERNELMODULECONTROL_H_
