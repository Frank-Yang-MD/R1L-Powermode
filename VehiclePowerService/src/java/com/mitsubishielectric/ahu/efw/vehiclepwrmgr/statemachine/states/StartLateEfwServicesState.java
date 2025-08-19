/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IJavaRunner;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.VpsAbstractState;

/**
 * StartLateEfwServicesState.
 *
 * <p>Used for late efw apps start.
 */
public final class StartLateEfwServicesState extends VpsAbstractState {
    private static StartLateEfwServicesState sStartLateEfwServicesState;
    private final IJavaRunner mRunner;
    private final IStartupInfo mStartupInfo;

    /**
     * Has to be run before any other method.
     *
     * @param stateMachine state machine.
     * @param runner apps runner.
     * @param startupInfo startup info object to notify when job is done
     */
    public static void initInstance(
            IVpsStateMachine stateMachine, IJavaRunner runner, IStartupInfo startupInfo) {
        sStartLateEfwServicesState =
                new StartLateEfwServicesState(stateMachine, runner, startupInfo);
    }

    public static StartLateEfwServicesState getInstance() {
        return sStartLateEfwServicesState;
    }

    @Override
    public void onEnter() {
        mRunner.start();
    }

    @Override
    public void onExit() {
        mStartupInfo.setAllServicesStarted(true);
    }

    @Override
    public void onLateEfwServicesStarted() {
        super.transitionTo(StartLateAppServicesState.getInstance());
    }

    private StartLateEfwServicesState(
            IVpsStateMachine stateMachine, IJavaRunner runner, IStartupInfo startupInfo) {
        super(stateMachine);
        mRunner = runner;
        mStartupInfo = startupInfo;
    }
}
