/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORFWSTOPVPS_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORFWSTOPVPS_H_

#include "INotifyHelper.h"
#include "IPowerStateHolder.h"
#include "ITimeoutControl.h"
#include "Timer.h"
#include "VpdStateBase.h"

#include <chrono>
#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IVcpuActions;

/**
 * Wait for 'FwStop Complete' notification forom Vehicle Power Service
 */
class WaitForFwStopVps : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine*, INotifyHelper&, ITimer&, IVcpuActions& vcpuActions);

    static void uninitInstance() { sInstance.reset(nullptr); }

    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;

    /**
     * Transition to WaitNativeAppStopProcessingComplete on "FwStopComplete"
     * notification from VPS.
     */
    virtual void onFwStopCompleteVps() override;

    /**
     * Send "selfReboot" command to VPCU.
     */
    virtual void onStopFailedVps() override;

    virtual void onTimeout() override;

    virtual inline std::string stateName() const override
    {
        return std::string("WaitForFwStopVps");
    }

private:
    INotifyHelper& mNotifyHelper;

    WaitForFwStopVps(IVpdStateMachine*, INotifyHelper&, ITimer&, IVcpuActions& vcpuActions);

    static std::unique_ptr<VpdStateBase> sInstance;

    ITimer& mFwStopVpsTimer;
    IVcpuActions& mVcpuActions;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORFWSTOPVPS_H_
