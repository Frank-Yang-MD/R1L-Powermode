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
 * ResumeServicesState.
 *
 * <p>Used for resuming the services
 */
public final class ResumeServicesState extends VpsAbstractState {
    private static ResumeServicesState sInstance;
    private final ISubscriberNotification mFwServicesNotification;
    private final IResetCounter mResetCounter;
    private final ICheckCompletion mCheckCompletion;
    private final IVpdNativeAdapter mVpdNativeAdapter;

    /**
     * Has to be run first of all methods.
     *
     * @param stateMachine state machine.
     * @param fwServicesNotification notifying fw to resume
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
                new ResumeServicesState(
                        stateMachine,
                        fwServicesNotification,
                        resetCounter,
                        checkCompletion,
                        vpdNativeAdapter);
    }

    public static ResumeServicesState getInstance() {
        return sInstance;
    }

    @Override
    public void onAppResume() {
        super.transitionTo(ResumeAppServicesState.getInstance());
    }

    @Override
    public void onEfwServicesResumeComplete() {
        mVpdNativeAdapter.vpdFwResumeComplete();
    }

    @Override
    public void onCompletionTimeout() {
        mCheckCompletion.logNotCompleted();
    }

    @Override
    public void onEnter() {
        mResetCounter.reset();
        if (mCheckCompletion.isComplete()) {
            mVpdNativeAdapter.vpdFwResumeComplete();
            onAppResume();
        } else {
            mFwServicesNotification.notifyAppResume();
        }
    }

    private ResumeServicesState(
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
