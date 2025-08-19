/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_INITIAL_STATE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SM_INIT_STATES;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_EARLY_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;

import android.app.Service;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IJavaRunner;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ISubscriberNotification;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.broadcastsender.BroadcastMelcoBootCompleteSender;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.CompleteNotificationCounter;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runningchecker.IServicesRunningChecker;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states.NormalOperationState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states.RestartAppServicesState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states.RestartServicesState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states.ResumeAppServicesState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states.ResumeServicesState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states.StartLateAppServicesState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states.StartLateEfwServicesState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states.StartNormalAppServicesState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states.StartNormalEfwServicesState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states.StopAppServiceState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states.StopEfwServicesState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states.StoppedState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states.WaitVpdOnStartState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;

/** Init all states and set initial state to state machine */
public final class StateInitializer {
    private BroadcastMelcoBootCompleteSender mBroadcastMelcoBootCompleteSender;
    private IVpsStateMachine mStateMachine;
    private IJavaRunner mLateSrvRunner;
    private IJavaRunner mNormalSrvRunner;
    private IJavaRunner mLateAppRunner;
    private IJavaRunner mNormalAppRunner;
    private IVpdNativeAdapter mNativeAdapter;
    private ISubscriberNotification mSrvSubscriberNotification;
    private ISubscriberNotification mAppSubscriberNotification;
    private CompleteNotificationCounter mStopCompleteAppCounter;
    private CompleteNotificationCounter mStopCompleteFwCounter;
    private CompleteNotificationCounter mFwResumeCompleteCounter;
    private CompleteNotificationCounter mFwRestartCompleteCounter;
    private CompleteNotificationCounter mAppResumeCompleteCounter;
    private CompleteNotificationCounter mAppRestartCompleteCounter;
    private IStartupInfo mStartupInfo;
    private IServicesRunningChecker mServicesRunningChecker;

    public StateInitializer use(Service service) {
        mBroadcastMelcoBootCompleteSender = new BroadcastMelcoBootCompleteSender(service);
        return this;
    }

    public StateInitializer use(IVpsStateMachine sm) {
        mStateMachine = sm;
        return this;
    }

    public StateInitializer use(
            IJavaRunner lateSrvRunner,
            IJavaRunner normalSrvRunner,
            IJavaRunner lateAppRunner,
            IJavaRunner normalAppRunner) {
        mLateSrvRunner = lateSrvRunner;
        mNormalSrvRunner = normalSrvRunner;
        mLateAppRunner = lateAppRunner;
        mNormalAppRunner = normalAppRunner;
        return this;
    }

    public StateInitializer use(IVpdNativeAdapter nativeAdapter) {
        mNativeAdapter = nativeAdapter;
        return this;
    }

    public StateInitializer use(
            ISubscriberNotification srvSubscriberNotification,
            ISubscriberNotification appSubscriberNotification) {
        mSrvSubscriberNotification = srvSubscriberNotification;
        mAppSubscriberNotification = appSubscriberNotification;
        return this;
    }

    public StateInitializer use(
            CompleteNotificationCounter stopCompleteAppCounter,
            CompleteNotificationCounter stopCompleteFwCounter,
            CompleteNotificationCounter resumeCompleteFwCounter,
            CompleteNotificationCounter restartCompleteFwCounter,
            CompleteNotificationCounter resumeCompleteAppCounter,
            CompleteNotificationCounter restartCompleteAppCounter) {
        mStopCompleteAppCounter = stopCompleteAppCounter;
        mStopCompleteFwCounter = stopCompleteFwCounter;
        mFwResumeCompleteCounter = resumeCompleteFwCounter;
        mFwRestartCompleteCounter = restartCompleteFwCounter;
        mAppResumeCompleteCounter = resumeCompleteAppCounter;
        mAppRestartCompleteCounter = restartCompleteAppCounter;
        return this;
    }

    public StateInitializer use(IStartupInfo startupInfo) {
        mStartupInfo = startupInfo;
        return this;
    }

    public StateInitializer use(IServicesRunningChecker servicesRunningChecker) {
        mServicesRunningChecker = servicesRunningChecker;
        return this;
    }

    public void initStates() {
        MLog.i(VPS_EARLY_FUNCTION_ID, SM_INIT_STATES);
        MLog.i(VPS_FUNCTION_ID, SM_INIT_STATES);

        StartNormalEfwServicesState.initInstance(mStateMachine, mNormalSrvRunner);
        StartLateEfwServicesState.initInstance(mStateMachine, mLateSrvRunner, mStartupInfo);
        StartNormalAppServicesState.initInstance(
                mStateMachine,
                mNormalAppRunner,
                mStartupInfo,
                mNativeAdapter,
                mBroadcastMelcoBootCompleteSender);
        StartLateAppServicesState.initInstance(
                mStateMachine, mLateAppRunner, mNativeAdapter, mStartupInfo);
        NormalOperationState.initInstance(mStateMachine);
        StopAppServiceState.initInstance(
                mStateMachine,
                mStopCompleteAppCounter,
                mStopCompleteAppCounter,
                mAppSubscriberNotification,
                mNativeAdapter);
        StopEfwServicesState.initInstance(
                mStateMachine, mStopCompleteFwCounter,
                mStopCompleteFwCounter, mSrvSubscriberNotification,
                mNativeAdapter, mServicesRunningChecker);
        StoppedState.initInstance(mStateMachine);

        RestartServicesState.initInstance(
                mStateMachine,
                mSrvSubscriberNotification,
                mFwRestartCompleteCounter,
                mFwRestartCompleteCounter,
                mNativeAdapter);
        ResumeServicesState.initInstance(
                mStateMachine,
                mSrvSubscriberNotification,
                mFwResumeCompleteCounter,
                mFwResumeCompleteCounter,
                mNativeAdapter);

        RestartAppServicesState.initInstance(
                mStateMachine,
                mAppSubscriberNotification,
                mAppRestartCompleteCounter,
                mAppRestartCompleteCounter,
                mNativeAdapter);

        ResumeAppServicesState.initInstance(
                mStateMachine,
                mAppSubscriberNotification,
                mAppResumeCompleteCounter,
                mAppResumeCompleteCounter,
                mNativeAdapter);

        WaitVpdOnStartState.initInstance(mStateMachine);

        IVpsState initialState = WaitVpdOnStartState.getInstance();
        MLog.i(VPS_EARLY_FUNCTION_ID, SM_INITIAL_STATE, initialState.stateName());
        MLog.i(VPS_FUNCTION_ID, SM_INITIAL_STATE, initialState.stateName());
        mStateMachine.transitionTo(initialState);
    }
}
