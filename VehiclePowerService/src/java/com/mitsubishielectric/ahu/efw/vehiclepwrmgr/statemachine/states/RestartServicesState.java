/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ISubscriberNotification;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.ICheckCompletion;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.IResetCounter;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.VpsAbstractState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;

/**
 * RestartServicesState
 *
 * <p>Used for restarting the services
 */
public final class RestartServicesState extends VpsAbstractState {
    private static RestartServicesState sInstance;
    private final ISubscriberNotification mFwServicesNotification;
    private final IResetCounter mResetCounter;
    private final ICheckCompletion mCheckCompletion;
    private final IVpdNativeAdapter mVpdNativeAdapter;

    /**
     * Has to be run first of all methods.
     *
     * @param stateMachine state machine.
     * @param fwServicesNotification notifying fw to restart
     * @param resetCounter interface for completion counter reset
     * @param checkCompletion interface for checking completion counter
     * @param vpdNativeAdapter adapter for native layer of VehiclePowerDaemon
     */
    public static void initInstance(
            IVpsStateMachine stateMachine,
            ISubscriberNotification fwServicesNotification,
            IResetCounter resetCounter,
            ICheckCompletion checkCompletion,
            IVpdNativeAdapter vpdNativeAdapter) {
        sInstance =
                new RestartServicesState(
                        stateMachine,
                        fwServicesNotification,
                        resetCounter,
                        checkCompletion,
                        vpdNativeAdapter);
    }

    public static RestartServicesState getInstance() {
        return sInstance;
    }

    @Override
    public void onAppRestart() {
        super.transitionTo(RestartAppServicesState.getInstance());
    }

    @Override
    public void onEfwServicesRestartComplete() {
        mVpdNativeAdapter.vpdFwRestartComplete();
    }

    @Override
    public void onCompletionTimeout() {
        mCheckCompletion.logNotCompleted();
    }

    @Override
    public void onEnter() {
        mResetCounter.reset();
        if (mCheckCompletion.isComplete()) {
            mVpdNativeAdapter.vpdFwRestartComplete();
            onAppRestart();
        } else {
            mFwServicesNotification.notifyAppRestart();
        }
    }

    private RestartServicesState(
            IVpsStateMachine stateMachine,
            ISubscriberNotification fwServicesNotification,
            IResetCounter resetCounter,
            ICheckCompletion checkCompletion,
            IVpdNativeAdapter vpdNativeAdapter) {
        super(stateMachine);
        mFwServicesNotification = fwServicesNotification;
        mResetCounter = resetCounter;
        mCheckCompletion = checkCompletion;
        mVpdNativeAdapter = vpdNativeAdapter;
    }
}
