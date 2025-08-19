/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORAPPRESUMECOMPLETE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORAPPRESUMECOMPLETE_H_

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
 * @brief The WaitForAppResumeComplete class
 */
class WaitForAppResumeComplete : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine* sm,
                             INotifyHelper& notifyHelper,
                             ITimer& appResumeEfwTimer,
                             IVcpuActions& cpuComActions);
    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;
    virtual void onAppResumeComplete() override;
    virtual void onTimeout() override;
    virtual void onExit() override;
    virtual void onDisconnectVps() override;

    virtual inline std::string stateName() const override
    {
        return std::string{"WaitForAppResumeComplete"};
    }

private:
    WaitForAppResumeComplete(IVpdStateMachine* sm,
                             INotifyHelper& notifyHelper,
                             ITimer& appResumeEfwTimer,
                             IVcpuActions& cpuComActions);

    static std::unique_ptr<VpdStateBase> sInstance;

    INotifyHelper& mNotifyHelper;
    ITimer& mAppResumeEfwTimer;
    IVcpuActions& mCpuActions;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORAPPRESUMECOMPLETE_H_
