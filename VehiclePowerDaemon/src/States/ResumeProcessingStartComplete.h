/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_RESUMEPROCESSINGSTARTCOMPLETE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_RESUMEPROCESSINGSTARTCOMPLETE_H_

#include "VpdStateBase.h"

#include "IPowerStateHolder.h"
#include "ITimerManager.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IVcpuActions;

/**
 * Notification from CpuCom
 */
class ResumeProcessingStartComplete final : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine*, IVcpuActions&, IPowerStateHolder&, ITimerManager&);
    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;
    virtual void onExit() override;

    virtual void onResumeProcessingStartCompleteSentSuccess() override;

    virtual void onResumeProcessingStartCompleteSentFailure() override;

    virtual inline std::string stateName() const override
    {
        return "ResumeProcessingStartComplete";
    }

private:
    ResumeProcessingStartComplete(IVpdStateMachine*,
                                  IVcpuActions&,
                                  IPowerStateHolder&,
                                  ITimerManager&);

    IVcpuActions& mVcpuActions;
    IPowerStateHolder& mPowerStateHolder;
    ITimerManager& mTimerManager;

    static std::unique_ptr<VpdStateBase> sInstance;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_RESUMEPROCESSINGSTARTCOMPLETE_H_
