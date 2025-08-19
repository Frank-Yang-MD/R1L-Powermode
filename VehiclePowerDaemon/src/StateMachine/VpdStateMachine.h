/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPDSTATEMACHINE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPDSTATEMACHINE_H_

#include <chrono>
#include <map>

#include "IVpdState.h"
#include "IVpdStateMachine.h"
#include "VpdStateBase.h"
#include "VpdStateMachineContext.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class VpdStateMachine
    : public IVpdStateMachine
    , public IVpdState {
public:
    VpdStateMachine();
    virtual ~VpdStateMachine() override;

    virtual void transitionTo(VpdStateBase*) override;

    /**
     * Subscription notification from native clients
     */
    virtual void onSubscribeNative(int) override;

    /**
     * Receive 'Stop complete' notification from native clients
     */
    virtual void onStopCompleteNative(int) override;

    /**
     * Receive 'Start complete' notification from CpuComDaemon
     */
    virtual void onStartCompleteCpuCom() override;

    /**
     * Receive notification about changing of current power state
     */
    virtual void onPowerStateChange(PowerState) override;

    /**
     * Subscription notification from VehiclePowerService
     */
    virtual void onSubscribeVps(int) override;

    /**
     * Receive 'Start complete' notification from VehiclePowerService
     */
    virtual void onStartCompleteVps() override;

    /**
     * Receive 'Start complete' notification from VehiclePowerService
     */
    virtual void onStartFailedVps() override;

    /**
     * Receive 'Stop comlete' notification from LogDaemon
     */
    virtual void onStopCompleteLog() override;

    /**
     *  Timeout notification
     */
    virtual void onTimeout() override;

    /**
     *  Receive 'AppStop complete' notification from VPS
     */
    virtual void onAppStopCompleteVps() override;

    /**
     *  Receive 'FwStop complete' notification from VPS
     */
    virtual void onFwStopCompleteVps() override;

    /**
     *  Receive 'Stop failed' notification from VPS
     */
    virtual void onStopFailedVps() override;

    /**
     * Receive 'Fw Resume Complete' notification from VPS
     */
    virtual void onFwResumeComplete() override;

    /**
     * Receive 'Fw Restart Complete' notification from VPS
     */
    virtual void onFwRestartComplete() override;

    /**
     * Receive 'App Resume Complete' notification from VPS
     */
    virtual void onAppResumeComplete() override;

    /**
     * Receive 'App Restart Complete' notification from VPS
     */
    virtual void onAppRestartComplete() override;

    /**
     * Receive 'Unmount Complete' notification from VPS
     */
    virtual void onVpsUnmountComplete() override;

    /**
     * Receive 'Shutdown Processing Start' notification from CpuCom
     */
    virtual void onShutdownProcessingStartSent() override;

    /**
     * Receive 'Resume Processing success' notification from CpuCom
     */
    virtual void onResumeProcessingStartCompleteSentSuccess() override;

    /**
     * Receive 'Resume Processing failure' notification from CpuCom
     */
    virtual void onResumeProcessingStartCompleteSentFailure() override;

    /**
     * Receive 'WakeUp' notification kernel
     */
    virtual void onWakeUp() override;

    /**
     * Receive notification about VehiclePowerService disconnection
     */
    virtual void onDisconnectVps() override;

    /**
     * @return mVpdStateMachineContext instance
     */
    virtual IVpdStateMachineContext& context() override;

private:
    VpdStateBase* mState;
    VpdStateMachineContext mContext;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPDSTATEMACHINE_H_
