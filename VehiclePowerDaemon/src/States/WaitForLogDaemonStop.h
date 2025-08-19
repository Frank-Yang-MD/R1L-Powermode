/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORLOGDAEMONSTOP_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORLOGDAEMONSTOP_H_

#include "ILogDaemonActions.h"
#include "INotifyHelper.h"
#include "IPowerStateHolder.h"
#include "IVcpuActions.h"
#include "Timer.h"
#include "VpdStateBase.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * State S9
 * Wait event StopComplete from LogDaemon
 */
class WaitForLogDaemonStop : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine*,
                             INotifyHelper&,
                             ITimer& logdogStopTimer,
                             IPowerStateHolder&,
                             IVcpuActions& vcpuAction);

    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;
    virtual void onTimeout() override;
    /**
     * Transition to State S11(Stop)
     */
    virtual void onStopCompleteLog() override;

    virtual inline std::string stateName() const override
    {
        return std::string("WaitForLogDaemonStop");
    }

private:
    WaitForLogDaemonStop(IVpdStateMachine*,
                         INotifyHelper&,
                         ITimer& logdogStopTimer,
                         IPowerStateHolder&,
                         IVcpuActions& vcpuAction);

    void transitionToNextState();

    static std::unique_ptr<VpdStateBase> sInstance;

    INotifyHelper& mNotifyHelper;
    ITimer& mLogdogStopTimer;
    IPowerStateHolder& mPowerStateHolder;
    IVcpuActions& mVcpuAction;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORLOGDAEMONSTOP_H_
