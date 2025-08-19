/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAKEUP_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAKEUP_H_

#include "VpdStateBase.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IWakeUpManager;
class IKernelModulesControl;

class WakeUp final : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine*, IKernelModulesControl&, IWakeUpManager&);

    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;

    virtual inline std::string stateName() const override { return std::string{"WakeUp"}; }

private:
    WakeUp(IVpdStateMachine*, IKernelModulesControl&, IWakeUpManager&);

    IKernelModulesControl& mKernelModulesControl;
    IWakeUpManager& mWakeUpManager;

    static std::unique_ptr<VpdStateBase> sInstance;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAKEUP_H_
