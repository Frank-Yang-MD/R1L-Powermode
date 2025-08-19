/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IJavaRunner;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.VpsAbstractState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;

/**
 * StartLateAppServicesState.
 *
 * <p>Used for late apps start.
 */
public final class StartLateAppServicesState extends VpsAbstractState {
    private static StartLateAppServicesState sStartLateAppServicesState;
    private final IJavaRunner mRunner;
    private final IVpdNativeAdapter mVpd;
    private final IStartupInfo mStartupInfo;

    /**
     * Has to be run before any other method.
     *
     * @param stateMachine state machine.
     * @param runner apps runner.
     * @param startupInfo startup info object to notify when job is done
     */
    public static void initInstance(
            IVpsStateMachine stateMachine,
            IJavaRunner runner,
            IVpdNativeAdapter vpd,
            IStartupInfo startupInfo) {
        sStartLateAppServicesState =
                new StartLateAppServicesState(stateMachine, runner, vpd, startupInfo);
    }

    public static StartLateAppServicesState getInstance() {
        return sStartLateAppServicesState;
    }

    @Override
    public void onEnter() {
        mRunner.start();
    }

    @Override
    public void onExit() {
        mVpd.vpdStartComplete();
        mStartupInfo.setAllAppsStarted(true);
    }

    /** Execute when apps have started to change state. */
    @Override
    public void onLateAppServicesStarted() {
        super.transitionTo(NormalOperationState.getInstance());
    }

    private StartLateAppServicesState(
            IVpsStateMachine stateMachine,
            IJavaRunner runner,
            IVpdNativeAdapter vpd,
            IStartupInfo startupInfo) {
        super(stateMachine);
        mRunner = runner;
        mVpd = vpd;
        mStartupInfo = startupInfo;
    }
}
