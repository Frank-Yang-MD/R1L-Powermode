package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyZeroInteractions;
import static org.mockito.Mockito.when;
import static org.powermock.api.mockito.PowerMockito.mockStatic;
import static org.powermock.api.mockito.PowerMockito.whenNew;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IJavaRunner;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ISubscriberNotification;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VehiclePowerService;
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
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor({
    "com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VehiclePowerService",
    "com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog"
})
@PrepareForTest({
    VehiclePowerService.class,
    BroadcastMelcoBootCompleteSender.class,
    CompleteNotificationCounter.class,
    StartNormalEfwServicesState.class,
    StartLateEfwServicesState.class,
    StartNormalAppServicesState.class,
    StartLateAppServicesState.class,
    NormalOperationState.class,
    StopAppServiceState.class,
    StopEfwServicesState.class,
    StoppedState.class,
    RestartServicesState.class,
    ResumeServicesState.class,
    RestartAppServicesState.class,
    ResumeAppServicesState.class,
    WaitVpdOnStartState.class
})
public class StateInitializerTest {
    @Mock private VehiclePowerService mVehiclePowerServiceMock;

    @Mock private BroadcastMelcoBootCompleteSender mBroadcastMelcoBootCompleteSenderMock;

    @Mock private IVpsStateMachine mStateMachineMock;

    @Mock private IJavaRunner mLateSrvRunnerMock;

    @Mock private IJavaRunner mNormalSrvRunnerMock;

    @Mock private IJavaRunner mLateAppRunnerMock;

    @Mock private IJavaRunner mNormalAppRunnerMock;

    @Mock private IVpdNativeAdapter mNativeAdapterMock;

    @Mock private ISubscriberNotification mSrvSubscriberNotificationMock;

    @Mock private ISubscriberNotification mAppSubscriberNotificationMock;

    @Mock private CompleteNotificationCounter mStopCompleteAppCounterMock;

    @Mock private CompleteNotificationCounter mStopCompleteFwCounterMock;

    @Mock private CompleteNotificationCounter mFwResumeCompleteCounterMock;

    @Mock private CompleteNotificationCounter mFwRestartCompleteCounterMock;

    @Mock private CompleteNotificationCounter mAppResumeCompleteCounterMock;

    @Mock private CompleteNotificationCounter mAppRestartCompleteCounterMock;

    @Mock private IStartupInfo mStartupInfoMock;

    @Mock private IServicesRunningChecker mServicesRunningCheckerMock;

    @Mock private IVpdNativeAdapter mVpdNativeAdapterMock;

    @Mock private WaitVpdOnStartState mWaitOnVpdStartState;

    @InjectMocks private StateInitializer mStateInitializer;

    @Before
    public void setUp() throws Exception {
        mockStatic(StartNormalEfwServicesState.class);
        mockStatic(StartLateEfwServicesState.class);
        mockStatic(StartNormalAppServicesState.class);
        mockStatic(StartLateAppServicesState.class);
        mockStatic(NormalOperationState.class);
        mockStatic(StopAppServiceState.class);
        mockStatic(StopEfwServicesState.class);
        mockStatic(StoppedState.class);
        mockStatic(RestartServicesState.class);
        mockStatic(ResumeServicesState.class);
        mockStatic(RestartAppServicesState.class);
        mockStatic(ResumeAppServicesState.class);
        mockStatic(MLog.class);

        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));

        whenNew(WaitVpdOnStartState.class)
                .withArguments(mStateMachineMock)
                .thenReturn(mWaitOnVpdStartState);
        when(mWaitOnVpdStartState.stateName()).thenReturn("TestState");
    }

    @Test
    public void testUseService() {
        assertEquals(mStateInitializer, mStateInitializer.use(mVehiclePowerServiceMock));

        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mLateSrvRunnerMock);
        verifyZeroInteractions(mNormalSrvRunnerMock);
        verifyZeroInteractions(mLateAppRunnerMock);
        verifyZeroInteractions(mNormalAppRunnerMock);
        verifyZeroInteractions(mNativeAdapterMock);
        verifyZeroInteractions(mSrvSubscriberNotificationMock);
        verifyZeroInteractions(mAppSubscriberNotificationMock);
        verifyZeroInteractions(mStopCompleteAppCounterMock);
        verifyZeroInteractions(mStopCompleteFwCounterMock);
        verifyZeroInteractions(mFwResumeCompleteCounterMock);
        verifyZeroInteractions(mFwRestartCompleteCounterMock);
        verifyZeroInteractions(mAppResumeCompleteCounterMock);
        verifyZeroInteractions(mAppRestartCompleteCounterMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mServicesRunningCheckerMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testUseIVpsStateMachine() {
        assertEquals(mStateInitializer, mStateInitializer.use(mStateMachineMock));

        verifyZeroInteractions(mBroadcastMelcoBootCompleteSenderMock);
        verifyZeroInteractions(mLateSrvRunnerMock);
        verifyZeroInteractions(mNormalSrvRunnerMock);
        verifyZeroInteractions(mLateAppRunnerMock);
        verifyZeroInteractions(mNormalAppRunnerMock);
        verifyZeroInteractions(mNativeAdapterMock);
        verifyZeroInteractions(mSrvSubscriberNotificationMock);
        verifyZeroInteractions(mAppSubscriberNotificationMock);
        verifyZeroInteractions(mStopCompleteAppCounterMock);
        verifyZeroInteractions(mStopCompleteFwCounterMock);
        verifyZeroInteractions(mFwResumeCompleteCounterMock);
        verifyZeroInteractions(mFwRestartCompleteCounterMock);
        verifyZeroInteractions(mAppResumeCompleteCounterMock);
        verifyZeroInteractions(mAppRestartCompleteCounterMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mServicesRunningCheckerMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testUseIJavaRunners() {
        assertEquals(
                mStateInitializer,
                mStateInitializer.use(
                        mLateSrvRunnerMock,
                        mNormalSrvRunnerMock,
                        mLateAppRunnerMock,
                        mNormalAppRunnerMock));

        verifyZeroInteractions(mBroadcastMelcoBootCompleteSenderMock);
        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mNativeAdapterMock);
        verifyZeroInteractions(mSrvSubscriberNotificationMock);
        verifyZeroInteractions(mAppSubscriberNotificationMock);
        verifyZeroInteractions(mStopCompleteAppCounterMock);
        verifyZeroInteractions(mStopCompleteFwCounterMock);
        verifyZeroInteractions(mFwResumeCompleteCounterMock);
        verifyZeroInteractions(mFwRestartCompleteCounterMock);
        verifyZeroInteractions(mAppResumeCompleteCounterMock);
        verifyZeroInteractions(mAppRestartCompleteCounterMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mServicesRunningCheckerMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testUseIVpdNativeAdapter() {
        assertEquals(mStateInitializer, mStateInitializer.use(mNativeAdapterMock));

        verifyZeroInteractions(mBroadcastMelcoBootCompleteSenderMock);
        verifyZeroInteractions(mLateSrvRunnerMock);
        verifyZeroInteractions(mNormalSrvRunnerMock);
        verifyZeroInteractions(mLateAppRunnerMock);
        verifyZeroInteractions(mNormalAppRunnerMock);
        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mSrvSubscriberNotificationMock);
        verifyZeroInteractions(mAppSubscriberNotificationMock);
        verifyZeroInteractions(mStopCompleteAppCounterMock);
        verifyZeroInteractions(mStopCompleteFwCounterMock);
        verifyZeroInteractions(mFwResumeCompleteCounterMock);
        verifyZeroInteractions(mFwRestartCompleteCounterMock);
        verifyZeroInteractions(mAppResumeCompleteCounterMock);
        verifyZeroInteractions(mAppRestartCompleteCounterMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mServicesRunningCheckerMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testUseIBootDaemonNativeAdapter() {
        assertEquals(mStateInitializer, mStateInitializer.use(mVpdNativeAdapterMock));

        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mLateSrvRunnerMock);
        verifyZeroInteractions(mNormalSrvRunnerMock);
        verifyZeroInteractions(mLateAppRunnerMock);
        verifyZeroInteractions(mNormalAppRunnerMock);
        verifyZeroInteractions(mNativeAdapterMock);
        verifyZeroInteractions(mSrvSubscriberNotificationMock);
        verifyZeroInteractions(mAppSubscriberNotificationMock);
        verifyZeroInteractions(mStopCompleteAppCounterMock);
        verifyZeroInteractions(mStopCompleteFwCounterMock);
        verifyZeroInteractions(mFwResumeCompleteCounterMock);
        verifyZeroInteractions(mFwRestartCompleteCounterMock);
        verifyZeroInteractions(mAppResumeCompleteCounterMock);
        verifyZeroInteractions(mAppRestartCompleteCounterMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mServicesRunningCheckerMock);
        verifyZeroInteractions(mBroadcastMelcoBootCompleteSenderMock);
    }

    @Test
    public void testUseISubscriberNotifications() {
        assertEquals(
                mStateInitializer,
                mStateInitializer.use(
                        mSrvSubscriberNotificationMock, mAppSubscriberNotificationMock));

        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mLateSrvRunnerMock);
        verifyZeroInteractions(mNormalSrvRunnerMock);
        verifyZeroInteractions(mLateAppRunnerMock);
        verifyZeroInteractions(mNormalAppRunnerMock);
        verifyZeroInteractions(mNativeAdapterMock);
        verifyZeroInteractions(mBroadcastMelcoBootCompleteSenderMock);
        verifyZeroInteractions(mStopCompleteAppCounterMock);
        verifyZeroInteractions(mStopCompleteFwCounterMock);
        verifyZeroInteractions(mFwResumeCompleteCounterMock);
        verifyZeroInteractions(mFwRestartCompleteCounterMock);
        verifyZeroInteractions(mAppResumeCompleteCounterMock);
        verifyZeroInteractions(mAppRestartCompleteCounterMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mServicesRunningCheckerMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testUseCompleteNotificationCounters() {
        assertEquals(
                mStateInitializer,
                mStateInitializer.use(
                        mStopCompleteAppCounterMock,
                        mStopCompleteFwCounterMock,
                        mFwResumeCompleteCounterMock,
                        mFwRestartCompleteCounterMock,
                        mAppResumeCompleteCounterMock,
                        mAppRestartCompleteCounterMock));

        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mLateSrvRunnerMock);
        verifyZeroInteractions(mNormalSrvRunnerMock);
        verifyZeroInteractions(mLateAppRunnerMock);
        verifyZeroInteractions(mNormalAppRunnerMock);
        verifyZeroInteractions(mNativeAdapterMock);
        verifyZeroInteractions(mSrvSubscriberNotificationMock);
        verifyZeroInteractions(mAppSubscriberNotificationMock);
        verifyZeroInteractions(mBroadcastMelcoBootCompleteSenderMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mServicesRunningCheckerMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testUseIStartupInfo() {
        assertEquals(mStateInitializer, mStateInitializer.use(mStartupInfoMock));

        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mLateSrvRunnerMock);
        verifyZeroInteractions(mNormalSrvRunnerMock);
        verifyZeroInteractions(mLateAppRunnerMock);
        verifyZeroInteractions(mNormalAppRunnerMock);
        verifyZeroInteractions(mNativeAdapterMock);
        verifyZeroInteractions(mSrvSubscriberNotificationMock);
        verifyZeroInteractions(mAppSubscriberNotificationMock);
        verifyZeroInteractions(mStopCompleteAppCounterMock);
        verifyZeroInteractions(mStopCompleteFwCounterMock);
        verifyZeroInteractions(mFwResumeCompleteCounterMock);
        verifyZeroInteractions(mFwRestartCompleteCounterMock);
        verifyZeroInteractions(mAppResumeCompleteCounterMock);
        verifyZeroInteractions(mAppRestartCompleteCounterMock);
        verifyZeroInteractions(mBroadcastMelcoBootCompleteSenderMock);
        verifyZeroInteractions(mServicesRunningCheckerMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testUseIServicesRunningChecker() {
        assertEquals(mStateInitializer, mStateInitializer.use(mServicesRunningCheckerMock));

        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mLateSrvRunnerMock);
        verifyZeroInteractions(mNormalSrvRunnerMock);
        verifyZeroInteractions(mLateAppRunnerMock);
        verifyZeroInteractions(mNormalAppRunnerMock);
        verifyZeroInteractions(mNativeAdapterMock);
        verifyZeroInteractions(mSrvSubscriberNotificationMock);
        verifyZeroInteractions(mAppSubscriberNotificationMock);
        verifyZeroInteractions(mStopCompleteAppCounterMock);
        verifyZeroInteractions(mStopCompleteFwCounterMock);
        verifyZeroInteractions(mFwResumeCompleteCounterMock);
        verifyZeroInteractions(mFwRestartCompleteCounterMock);
        verifyZeroInteractions(mAppResumeCompleteCounterMock);
        verifyZeroInteractions(mAppRestartCompleteCounterMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mBroadcastMelcoBootCompleteSenderMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void initStates() {
        doNothing().when(mStateMachineMock).transitionTo(mWaitOnVpdStartState);
        mStateInitializer.initStates();
        verify(mStateMachineMock).transitionTo(mWaitOnVpdStartState);
    }
}
