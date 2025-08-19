/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.VpsAbstractState;

/**
 * NormalOperationState. In NormalOperationState the system do nothing. The system will switch from
 * NormalOperationState to StopAppState when state machine receives {@link
 * VpdStateMachine#onAppStop()} event
 */
public final class NormalOperationState extends VpsAbstractState {
    private static NormalOperationState sNormalOperationState;

    private NormalOperationState(IVpsStateMachine stateMachine) {
        super(stateMachine);
    }

    /**
     * Have to be run first of all methods.
     *
     * @param stateMachine state machine.
     */
    public static void initInstance(IVpsStateMachine stateMachine) {
        sNormalOperationState = new NormalOperationState(stateMachine);
    }

    public static NormalOperationState getInstance() {
        return sNormalOperationState;
    }

    /** Receive APP_STOP event from VehiclePowerDaemon */
    @Override
    public void onAppStop() {
        super.transitionTo(StopAppServiceState.getInstance());
    }
}
