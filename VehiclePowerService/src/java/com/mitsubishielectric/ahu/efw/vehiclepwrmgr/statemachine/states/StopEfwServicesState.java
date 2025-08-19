/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ISubscriberNotification;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.ICheckCompletion;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.IResetCounter;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runningchecker.IServicesRunningChecker;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.VpsAbstractState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;

/**
 * StopEfwServicesState.
 *
 * <p>Using for stop services.
 */
public final class StopEfwServicesState extends VpsAbstractState {
    private static StopEfwServicesState sInstance;

    private final IResetCounter mClearStopCompleteCounter;
    private final ICheckCompletion mStopCompleteCounter;
    private final ISubscriberNotification mFwServicesNotification;
    private final IVpdNativeAdapter mVpdNativeAdapter;
    private final IServicesRunningChecker mServicesRunningChecker;

    /**
     * Has to be run first of all methods.
     *
     * @param stateMachine state machine.
     * @param clearStopCompleteCounter counter for stopped services.
     * @param fwServicesNotification notifying fw services to stop.
     * @param vpdNativeAdapter notify VPD about success or failure
     * @param servicesRunningChecker checks is all services started by VPS currently running
     */
    public static void initInstance(
            IVpsStateMachine stateMachine,
            IResetCounter clearStopCompleteCounter,
            ICheckCompletion stopCompleteCounter,
            ISubscriberNotification fwServicesNotification,
            IVpdNativeAdapter vpdNativeAdapter,
            IServicesRunningChecker servicesRunningChecker) {
        sInstance =
                new StopEfwServicesState(
                        stateMachine, clearStopCompleteCounter,
                        stopCompleteCounter, fwServicesNotification,
                        vpdNativeAdapter, servicesRunningChecker);
    }

    public static StopEfwServicesState getInstance() {
        return sInstance;
    }

    /** Stop services. */
    @Override
    public void onEnter() {
        mClearStopCompleteCounter.reset();
        mFwServicesNotification.notifyAppStop();
    }

    /** Execute when apps stopped to change state. */
    @Override
    public void onEfwServicesStopComplete() {
        if (!(mServicesRunningChecker.isAllServicesRunning()
                && mStopCompleteCounter.isComplete())) {
            mVpdNativeAdapter.vpdStopFailed();
        } else {
            mVpdNativeAdapter.vpdFwStopComplete();
        }

        super.transitionTo(StoppedState.getInstance());
    }

    /** Resume all. */
    @Override
    public void onFwResume() {
        super.transitionTo(ResumeServicesState.getInstance());
    }

    /** Notify all services about FW_RESTART */
    @Override
    public void onFwRestart() {
        super.transitionTo(RestartServicesState.getInstance());
    }

    @Override
    public void onCompletionTimeout() {
        mStopCompleteCounter.logNotCompleted();
    }

    private StopEfwServicesState(
            IVpsStateMachine stateMachine,
            IResetCounter clearStopCompleteCounter,
            ICheckCompletion stopCompleteCounter,
            ISubscriberNotification fwServicesNotification,
            IVpdNativeAdapter vpdNativeAdapter,
            IServicesRunningChecker servicesRunningChecker) {
        super(stateMachine);

        mClearStopCompleteCounter = clearStopCompleteCounter;
        mStopCompleteCounter = stopCompleteCounter;
        mFwServicesNotification = fwServicesNotification;
        mVpdNativeAdapter = vpdNativeAdapter;
        mServicesRunningChecker = servicesRunningChecker;
    }
}
