/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORAPPRESTARTCOMPLETE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORAPPRESTARTCOMPLETE_H_

#include "VpdStateBase.h"
#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class INotifyHelper;
class ITimer;
class IVcpuActions;
class IPowerStateHolder;

/**
 * @brief The WaitForAppRestartComplete class
 */
class WaitForAppRestartComplete : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine* sm,
                             INotifyHelper& nh,
                             ITimer& rt,
                             IVcpuActions& va,
                             IPowerStateHolder& psh);
    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;
    virtual void onAppRestartComplete() override;
    virtual void onTimeout() override;
    virtual void onExit() override;
    virtual void onDisconnectVps() override;

    virtual inline std::string stateName() const override
    {
        return std::string{"WaitForAppRestartComplete"};
    }

private:
    WaitForAppRestartComplete(IVpdStateMachine* sm,
                              INotifyHelper& nh,
                              ITimer& rt,
                              IVcpuActions& va,
                              IPowerStateHolder& psh);

    void toNextState();

    static std::unique_ptr<VpdStateBase> sInstance;

    INotifyHelper& mNotifyHelper;
    ITimer& mAppRestartAppTimer;
    IVcpuActions& mCpuActions;
    IPowerStateHolder& mPowerStateHolder;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORAPPRESTARTCOMPLETE_H_
