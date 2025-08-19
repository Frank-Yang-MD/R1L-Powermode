/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORMODULESUNLOAD_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORMODULESUNLOAD_H_

#include "VpdStateBase.h"

#include <IKernelModulesControl.h>
#include <IPowerStateHolder.h>
#include <ITimer.h>
#include <IVcpuActions.h>

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class WaitForModulesUnload final : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine*,
                             ITimer&,
                             ITimer&,
                             IKernelModulesControl&,
                             IPowerStateHolder&);

    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;
    virtual void onTimeout() override;

    virtual inline std::string stateName() const override
    {
        return std::string{"WaitForModulesUnload"};
    }

private:
    WaitForModulesUnload(IVpdStateMachine*,
                         ITimer&,
                         ITimer&,
                         IKernelModulesControl&,
                         IPowerStateHolder&);

    void toNextState();

    ITimer& mCheckTimer;
    ITimer& mTimeout;
    IKernelModulesControl& mKernelModulesControl;
    IPowerStateHolder& mPowerStateHolder;

    static std::unique_ptr<VpdStateBase> sInstance;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORMODULESUNLOAD_H_
