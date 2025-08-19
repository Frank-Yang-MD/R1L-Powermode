/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.VpsAbstractState;

public class WaitVpdOnStartState extends VpsAbstractState {
    private static WaitVpdOnStartState sInstance;

    WaitVpdOnStartState(IVpsStateMachine stateMachine) {
        super(stateMachine);
    }

    public static void initInstance(IVpsStateMachine stateMachine) {
        sInstance = new WaitVpdOnStartState(stateMachine);
    }

    public static WaitVpdOnStartState getInstance() {
        return sInstance;
    }

    @Override
    public void onAppStartColdboot() {
        transitionTo(StartNormalEfwServicesState.getInstance());
    }
}
