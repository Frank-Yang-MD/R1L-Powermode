/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_START_NORMAL_APP_SERVICES_STATE_ONEXIT_NOTIFY_BOOTD;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IJavaRunner;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.broadcastsender.IBroadcastSender;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.VpsAbstractState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;

/**
 * StartNormalAppServicesState.
 *
 * <p>Used for apps start.
 */
public final class StartNormalAppServicesState extends VpsAbstractState {
    private final IBroadcastSender mIBroadcastSender;
    private static StartNormalAppServicesState sStartNormalAppServicesState;
    private final IJavaRunner mRunner;
    private final IVpdNativeAdapter mVpdNativeAdapter;
    private final IStartupInfo mStartupInfo;

    /**
     * Has to be run first of all methods.
     *
     * @param stateMachine state machine.
     * @param runner apps runner.
     * @param startupInfo startup info object to notify when job is done
     */
    public static void initInstance(
            IVpsStateMachine stateMachine,
            IJavaRunner runner,
            IStartupInfo startupInfo,
            IVpdNativeAdapter vpdNativeAdapter,
            IBroadcastSender broadcastSender) {
        sStartNormalAppServicesState =
                new StartNormalAppServicesState(
                        stateMachine, runner, startupInfo, vpdNativeAdapter, broadcastSender);
    }

    public static StartNormalAppServicesState getInstance() {
        return sStartNormalAppServicesState;
    }

    /** Starts all apps except for early. */
    @Override
    public void onEnter() {
        mRunner.start();
    }

    @Override
    public void onExit() {
        MLog.i(VPS_FUNCTION_ID, VPS_START_NORMAL_APP_SERVICES_STATE_ONEXIT_NOTIFY_BOOTD);
        mVpdNativeAdapter.hmiIsReady();
        mStartupInfo.setEarlyAppsStarted(true);
        mIBroadcastSender.sendBroadcast();
    }

    /** Execute when apps started to change state. */
    @Override
    public void onNormalAppServicesStarted() {
        super.transitionTo(StartLateEfwServicesState.getInstance());
    }

    private StartNormalAppServicesState(
            IVpsStateMachine stateMachine,
            IJavaRunner runner,
            IStartupInfo startupInfo,
            IVpdNativeAdapter vpdNativeAdapter,
            IBroadcastSender broadcastSender) {
        super(stateMachine);
        mIBroadcastSender = broadcastSender;
        mRunner = runner;
        mVpdNativeAdapter = vpdNativeAdapter;
        mStartupInfo = startupInfo;
    }
}
