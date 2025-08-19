package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ISubscriberNotification;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.ICheckCompletion;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.IResetCounter;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.VpsAbstractState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;

/**
 * ResumeAppServicesState.
 *
 * <p>Used for resuming the apps
 */
public final class ResumeAppServicesState extends VpsAbstractState {
    private static ResumeAppServicesState sInstance;
    private final ISubscriberNotification mAppServicesNotification;
    private final IResetCounter mResetCounter;
    private final ICheckCompletion mCheckCompletion;
    private final IVpdNativeAdapter mVpdNativeAdapter;

    /**
     * Has to be run first of all methods.
     *
     * @param stateMachine state machine.
     * @param appServicesNotification notifying app to resume
     * @param resetCounter interface for completion counter reset
     * @param checkCompletion interface for checking completion counter
     * @param vpdNativeAdapter adapter for native layer of VehiclePowerDaemon
     */
    public static void initInstance(
            IVpsStateMachine stateMachine,
            ISubscriberNotification appServicesNotification,
            IResetCounter resetCounter,
            ICheckCompletion checkCompletion,
            IVpdNativeAdapter vpdNativeAdapter) {
        sInstance =
                new ResumeAppServicesState(
                        stateMachine,
                        appServicesNotification,
                        resetCounter,
                        checkCompletion,
                        vpdNativeAdapter);
    }

    public static ResumeAppServicesState getInstance() {
        return sInstance;
    }

    @Override
    public void onAppServicesResumeComplete() {
        mVpdNativeAdapter.vpdAppResumeComplete();
        super.transitionTo(NormalOperationState.getInstance());
    }

    @Override
    public void onCompletionTimeout() {
        mCheckCompletion.logNotCompleted();
        super.transitionTo(NormalOperationState.getInstance());
    }

    @Override
    public void onEnter() {
        mResetCounter.reset();
        if (mCheckCompletion.isComplete()) {
            mVpdNativeAdapter.vpdAppResumeComplete();
            super.transitionTo(NormalOperationState.getInstance());
        } else {
            mAppServicesNotification.notifyAppResume();
        }
    }

    private ResumeAppServicesState(
            IVpsStateMachine stateMachine,
            ISubscriberNotification appServicesNotification,
            IResetCounter resetCounter,
            ICheckCompletion checkCompletion,
            IVpdNativeAdapter vpdNativeAdapter) {
        super(stateMachine);
        mAppServicesNotification = appServicesNotification;
        mResetCounter = resetCounter;
        mCheckCompletion = checkCompletion;
        mVpdNativeAdapter = vpdNativeAdapter;
    }
}
