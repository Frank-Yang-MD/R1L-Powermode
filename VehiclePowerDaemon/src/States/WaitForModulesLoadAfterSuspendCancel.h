/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORMODULESLOADAFTERSUSPENDCANCEL_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORMODULESLOADAFTERSUSPENDCANCEL_H_

#include "VpdStateBase.h"

#include <IKernelModulesControl.h>
#include <ITimer.h>
#include <WatchDogTimerHandler.h>

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class WaitForModulesLoadAfterSuspendCancel final : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine*, IKernelModulesControl&);

    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;

    virtual inline std::string stateName() const override
    {
        return std::string{"WaitForModulesLoadAfterSuspendCancel"};
    }

private:
    WaitForModulesLoadAfterSuspendCancel(IVpdStateMachine*, IKernelModulesControl&);

    IKernelModulesControl& mKernelModulesControl;

    static std::unique_ptr<VpdStateBase> sInstance;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORMODULESLOADAFTERSUSPENDCANCEL_H_
