/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SENDSHUTDOWNPROCESSINGSTART_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SENDSHUTDOWNPROCESSINGSTART_H_

#include "ITimerManager.h"
#include "VpdStateBase.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IPowerStateHolder;
class IVcpuActions;
/**
 * Notification from CpuCom
 */
class SendShutdownProcessingStart final : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine*, IVcpuActions&, IPowerStateHolder&, ITimerManager&);
    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;

    virtual void onExit() override;

    virtual void onShutdownProcessingStartSent() override;

    virtual inline std::string stateName() const override { return "SendShutdownProcessingStart"; }

private:
    SendShutdownProcessingStart(IVpdStateMachine*,
                                IVcpuActions&,
                                IPowerStateHolder&,
                                ITimerManager&);

    IVcpuActions& mVcpuActions;

    static std::unique_ptr<VpdStateBase> sInstance;

    IPowerStateHolder& mPowerStateHolder;
    ITimerManager& mTimerManager;
    PowerState mPreviousState;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SENDSHUTDOWNPROCESSINGSTART_H_
