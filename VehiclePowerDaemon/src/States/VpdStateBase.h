/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPDSTATEBASE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPDSTATEBASE_H_

#include "IVpdState.h"
#include "IVpdStateMachine.h"
#include <string>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Base interface to all states
 */
class VpdStateBase : public IVpdState {
public:
    VpdStateBase(IVpdStateMachine* sm)
        : mStateMachine{sm}
    {
    }

    // LCOV_EXCL_START
    virtual ~VpdStateBase() = default;

    /**
     * Notify enter to the state
     */
    virtual void onEnter() {}

    /**
     * Notify leave from the state
     */
    virtual void onExit() {}

    /**
     * Subscription notification from native clients
     */
    virtual void onSubscribeNative(int) override {}

    /**
     * Unsubscription notification from native clients
     */
    virtual void onUnsubscribeNative(int) {}

    /**
     * Receive 'Stop complete' notification from native clients
     */
    virtual void onStopCompleteNative(int) override {}

    /**
     * Receive 'Start complete' notification from CpuComDaemon
     */
    virtual void onStartCompleteCpuCom() override {}

    /**
     * Receive notification about power state change from VCPU
     */
    virtual void onPowerStateChange(PowerState) override {}

    /**
     * Subscription notification from VehiclePowerService
     */
    virtual void onSubscribeVps(int) override {}

    /**
     * Unsubscription notification from VehiclePowerService
     */
    virtual void onUnsubscribeVps(int) {}

    /**
     * Receive 'Start complete' notification from VehiclePowerService
     */
    virtual void onStartCompleteVps() override { mStateMachine->context().setVpsStartCompleted(true); }

    /**
     * Receive 'Start failed' notification from VehiclePowerService
     */
    virtual void onStartFailedVps() override {}

    /**
     * Receive 'AppStop complete' notification from VehiclePowerService
     */
    virtual void onAppStopCompleteVps() override {}

    /**
     * Receive 'FwStop complete' notification from VehiclePowerService
     */
    virtual void onFwStopCompleteVps() override {}

    /**
     * Receive 'Stop failed' notification from VehiclePowerService
     */
    virtual void onStopFailedVps() override {}

    /**
     * Receive 'Stop comlete' notification from LogDaemon
     */
    virtual void onStopCompleteLog() override {}

    /**
     * Notify timeout
     */
    virtual void onTimeout() override {}

    /**
     * Notify that fw resume complete
     */
    virtual void onFwResumeComplete() override {}

    /**
     * Notify that fw restart complete
     */
    virtual void onFwRestartComplete() override {}

    /**
     * Notify that app resume complete
     */
    virtual void onAppResumeComplete() override {}

    /**
     * Notify that app restart complete
     */
    virtual void onAppRestartComplete() override {}

    /**
     * Notify that unmount complete
     */
    virtual void onVpsUnmountComplete() override {}

    /**
     * Receive 'Shutdown Processing Start Sent' notification from CpuCom
     */
    virtual void onShutdownProcessingStartSent() override {}

    /**
     * Receive 'Resume Processing Start Complete Sent Success' notification from CpuCom
     */
    virtual void onResumeProcessingStartCompleteSentSuccess() override {}

    /**
     * Receive 'Resume Processing Start Complete Sent Failure' notification from CpuCom
     */
    virtual void onResumeProcessingStartCompleteSentFailure() override {}

    /**
     * Receive 'Wake Up' notification from kernel
     */
    virtual void onWakeUp() override {}
    // LCOV_EXCL_STOP

    /**
     * Receive notification about VehiclePowerService disconnection
     */
    virtual void onDisconnectVps() override { mStateMachine->context().setVpsDisconnected(true); }

    void transitionTo(VpdStateBase* state) { mStateMachine->transitionTo(state); }

    virtual inline std::string stateName() const { return std::string("UNDEFINED"); }

protected:
    IVpdStateMachine* mStateMachine;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPDSTATEBASE_H_
