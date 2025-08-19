/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.VpsAbstractState;

/**
 * StoppedState.
 *
 * <p>State what all stopped in it.
 */
public final class StoppedState extends VpsAbstractState {
    private static StoppedState sStoppedState;

    /**
     * Has to be run first of all methods.
     *
     * @param stateMachine state machine.
     */
    public static void initInstance(IVpsStateMachine stateMachine) {
        sStoppedState = new StoppedState(stateMachine);
    }

    public static StoppedState getInstance() {
        return sStoppedState;
    }

    /** Resume all. */
    @Override
    public void onFwResume() {
        super.transitionTo(ResumeServicesState.getInstance());
    }

    /** Restart all. */
    @Override
    public void onFwRestart() {
        super.transitionTo(RestartServicesState.getInstance());
    }

    private StoppedState(IVpsStateMachine stateMachine) {
        super(stateMachine);
    }
}
