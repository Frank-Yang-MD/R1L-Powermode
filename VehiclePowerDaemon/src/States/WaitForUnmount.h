/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORUNMOUNT_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORUNMOUNT_H

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
 * @brief The WaitForUnmount class
 */
class WaitForUnmount : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine* sm,
                             INotifyHelper& notifyHelper,
                             ITimer& waitForUnmountTimer,
                             IVcpuActions& cpuComActions);
    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;
    virtual void onVpsUnmountComplete() override;
    virtual void onTimeout() override;

    virtual inline std::string stateName() const override { return std::string{"WaitForUnmount"}; }

private:
    WaitForUnmount(IVpdStateMachine* sm,
                   INotifyHelper& notifyHelper,
                   ITimer& waitForUnmountTimer,
                   IVcpuActions& cpuComActions);

    static std::unique_ptr<VpdStateBase> sInstance;

    INotifyHelper& mNotifyHelper;
    ITimer& mWaitForUnmountTimer;
    IVcpuActions& mCpuActions;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORUNMOUNT_H
