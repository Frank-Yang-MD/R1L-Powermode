/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
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
 * StopAppServiceState.
 *
 * <p>Using for stop apps.
 */
public final class StopAppServiceState extends VpsAbstractState {
    private static StopAppServiceState sInstance;

    private final IResetCounter mClearStopCompleteCounter;
    private final ICheckCompletion mStopCompleteCounter;
    private final ISubscriberNotification mAppServicesNotification;
    private final IVpdNativeAdapter mVpdNativeAdapter;

    /**
     * Has to be run first of all methods.
     *
     * @param stateMachine state machine.
     * @param clearStopCompleteCounter counter for stopped apps.
     * @param appServicesNotification notifying app to stop.
     * @param vpdNativeAdapter adapter for native layer of VehiclePowerDaemon
     */
    public static void initInstance(
            IVpsStateMachine stateMachine,
            IResetCounter clearStopCompleteCounter,
            ICheckCompletion stopCompleteCounter,
            ISubscriberNotification appServicesNotification,
            IVpdNativeAdapter vpdNativeAdapter) {
        sInstance =
                new StopAppServiceState(
                        stateMachine,
                        clearStopCompleteCounter,
                        stopCompleteCounter,
                        appServicesNotification,
                        vpdNativeAdapter);
    }

    public static StopAppServiceState getInstance() {
        return sInstance;
    }

    /** Stop apps. */
    @Override
    public void onEnter() {
        mClearStopCompleteCounter.reset();
        mAppServicesNotification.notifyAppStop();
    }

    /** Execute when apps stopped to change state. */
    @Override
    public void onAppServicesStopComplete() {
        mVpdNativeAdapter.vpdAppStopComplete();
    }

    /** Transition to FW services stop state. */
    @Override
    public void onFwStop() {
        super.transitionTo(StopEfwServicesState.getInstance());
    }

    @Override
    public void onCompletionTimeout() {
        mStopCompleteCounter.logNotCompleted();
    }

    private StopAppServiceState(
            IVpsStateMachine stateMachine,
            IResetCounter clearStopCompleteCounter,
            ICheckCompletion stopCompleteCounter,
            ISubscriberNotification subscriberNotification,
            IVpdNativeAdapter vpdNativeAdapter) {
        super(stateMachine);
        mClearStopCompleteCounter = clearStopCompleteCounter;
        mStopCompleteCounter = stopCompleteCounter;
        mAppServicesNotification = subscriberNotification;
        mVpdNativeAdapter = vpdNativeAdapter;
    }
}
