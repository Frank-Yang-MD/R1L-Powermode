/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORMODULESLOADBEFOREUNLOAD_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORMODULESLOADBEFOREUNLOAD_H_

#include "VpdStateBase.h"

#include <IKernelModulesControl.h>
#include <ITimer.h>

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * This state is intended to prevent race condition between init.post-wakeup.sh
 * and init.pre-suspend.sh scripts in case of immediate suspend after resume.
 */
class WaitForModulesLoadBeforeUnload final : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine*, ITimer&, ITimer&, IKernelModulesControl&);

    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;
    virtual void onTimeout() override;

    virtual inline std::string stateName() const override
    {
        return std::string{"WaitForModulesLoadBeforeUnload"};
    }

private:
    WaitForModulesLoadBeforeUnload(IVpdStateMachine*, ITimer&, ITimer&, IKernelModulesControl&);

    void toNextState();

    ITimer& mCheckTimer;
    ITimer& mTimeout;

    IKernelModulesControl& mKernelModulesControl;

    static std::unique_ptr<VpdStateBase> sInstance;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORMODULESLOADBEFOREUNLOAD_H_
