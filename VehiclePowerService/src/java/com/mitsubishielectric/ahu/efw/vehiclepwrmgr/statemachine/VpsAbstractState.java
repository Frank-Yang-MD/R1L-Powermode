/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine;

public abstract class VpsAbstractState implements IVpsState {
    private final IVpsStateMachine mStateMachine;

    public VpsAbstractState(IVpsStateMachine stateMachine) {
        mStateMachine = stateMachine;
    }

    protected void transitionTo(IVpsState state) {
        mStateMachine.transitionTo(state);
    }

    public void onEnter() {}

    public void onExit() {}

    public String stateName() {
        return this.getClass().getSimpleName();
    }

    public void onAppStartColdboot() {}

    public void onAppRestart() {}

    public void onAppResume() {}

    public void onAppStop() {}

    public void onFwStop() {}

    public void onFwRestart() {}

    public void onFwResume() {}

    public void onNormalAppServicesStarted() {}

    public void onLateAppServicesStarted() {}

    public void onLateEfwServicesStarted() {}

    public void onAppServicesStopComplete() {}

    public void onNormalEfwServicesStarted() {}

    public void onEfwServicesStopComplete() {}

    public void onEfwServicesResumeComplete() {}

    public void onEfwServicesRestartComplete() {}

    public void onAppServicesRestartComplete() {}

    public void onAppServicesResumeComplete() {}

    public void onCompletionTimeout() {}

    public void onBootdStatusReceived() {}
}
