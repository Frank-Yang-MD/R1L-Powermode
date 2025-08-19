/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORAPPSTOPVPS_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORAPPSTOPVPS_H_

#include "IAndroidInit.h"
#include "INotifyHelper.h"
#include "IVcpuActions.h"
#include "Timer.h"
#include "VpdStateBase.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IVcpuActions;

/**
 * Wait for 'AppStop Complete' notification forom Vehicle Power Service
 */
class WaitForAppStopVps : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine*,
                             INotifyHelper&,
                             ITimer&,
                             ITimer&,
                             IVcpuActions&,
                             IAndroidInit&);

    static void uninitInstance() { sInstance.reset(nullptr); }

    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;

    /**
     * Transition to WaitForFwStopVps on "AppStopComplete" event from VPS.
     */
    virtual void onAppStopCompleteVps() override;

    virtual void onTimeout() override;

    virtual inline std::string stateName() const override
    {
        return std::string("WaitForAppStopVps");
    }

private:
    INotifyHelper& mNotifyHelper;

    WaitForAppStopVps(IVpdStateMachine*,
                      INotifyHelper&,
                      ITimer&,
                      ITimer&,
                      IVcpuActions&,
                      IAndroidInit&);
    void checkForColdboot() const;

    static std::unique_ptr<VpdStateBase> sInstance;

    ITimer& mAppStopVpsTimer;
    ITimer& mFwStopVpsTimer;

    IVcpuActions& mVcpuActions;
    IAndroidInit& mAndroidInit;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORAPPSTOPVPS_H_
