/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORFWRESTARTCOMPLETE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORFWRESTARTCOMPLETE_H_

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

/**
 * @brief The WaitForFwRestartComplete class
 */
class WaitForFwRestartComplete : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine* sm, INotifyHelper& nh, ITimer& rt, IVcpuActions& va);
    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;
    virtual void onFwRestartComplete() override;
    virtual void onTimeout() override;

    virtual void onDisconnectVps() override;

    virtual inline std::string stateName() const override
    {
        return std::string{"WaitForFwRestartComplete"};
    }

private:
    WaitForFwRestartComplete(IVpdStateMachine* sm, INotifyHelper& nh, ITimer& rt, IVcpuActions& va);

    static std::unique_ptr<VpdStateBase> sInstance;

    INotifyHelper& mNotifyHelper;
    ITimer& mAppRestartEfwTimer;
    IVcpuActions& mCpuActions;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORFWRESTARTCOMPLETE_H_
