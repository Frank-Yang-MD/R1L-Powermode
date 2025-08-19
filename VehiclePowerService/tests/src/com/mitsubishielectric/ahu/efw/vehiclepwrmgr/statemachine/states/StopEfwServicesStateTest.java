/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import static org.mockito.Mockito.*;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ISubscriberNotification;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.ICheckCompletion;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.IResetCounter;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runningchecker.IServicesRunningChecker;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

public class StopEfwServicesStateTest {
    private StopEfwServicesState mStopEfwServicesState;
    private IVpsStateMachine mStateMachineMock;
    private IResetCounter mResetCounterMock;
    private ISubscriberNotification mSubscriberNotificationMock;
    private ICheckCompletion mCheckCompletionMock;
    private IVpdNativeAdapter mVpdNativeAdapterMock;
    private IServicesRunningChecker mServicesRunningCheckerMock;

    @Before
    public void beforeTest() {
        mStateMachineMock = mock(IVpsStateMachine.class);
        mResetCounterMock = mock(IResetCounter.class);
        mSubscriberNotificationMock = mock(ISubscriberNotification.class);
        mCheckCompletionMock = mock(ICheckCompletion.class);
        mVpdNativeAdapterMock = mock(IVpdNativeAdapter.class);
        mServicesRunningCheckerMock = mock(IServicesRunningChecker.class);

        StopEfwServicesState.initInstance(
                mStateMachineMock,
                mResetCounterMock,
                mCheckCompletionMock,
                mSubscriberNotificationMock,
                mVpdNativeAdapterMock,
                mServicesRunningCheckerMock);

        mStopEfwServicesState = StopEfwServicesState.getInstance();
    }

    @Test
    public void testOnEnter() {
        mStopEfwServicesState.onEnter();

        verify(mResetCounterMock, times(1)).reset();
        verify(mSubscriberNotificationMock, times(1)).notifyAppStop();
        verifyNoMoreInteractions(mResetCounterMock);
        verifyNoMoreInteractions(mSubscriberNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mCheckCompletionMock);
        verifyZeroInteractions(mServicesRunningCheckerMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testOnEfwServicesStopComplete() {
        when(mServicesRunningCheckerMock.isAllServicesRunning()).thenReturn(true);
        when(mCheckCompletionMock.isComplete()).thenReturn(true);

        mStopEfwServicesState.onEfwServicesStopComplete();

        verify(mVpdNativeAdapterMock, times(1)).vpdFwStopComplete();
        verify(mStateMachineMock, times(1)).transitionTo(StoppedState.getInstance());
        verify(mServicesRunningCheckerMock, times(1)).isAllServicesRunning();
        verify(mCheckCompletionMock, times(1)).isComplete();
        verifyNoMoreInteractions(mVpdNativeAdapterMock);
        verifyNoMoreInteractions(mStateMachineMock);
        verifyNoMoreInteractions(mServicesRunningCheckerMock);
        verifyNoMoreInteractions(mCheckCompletionMock);
        verifyZeroInteractions(mSubscriberNotificationMock);
    }

    @Test
    public void testOnEfwServicesStopCompleteVpdStopFailed() {
        when(mServicesRunningCheckerMock.isAllServicesRunning()).thenReturn(false);
        when(mCheckCompletionMock.isComplete()).thenReturn(false);

        mStopEfwServicesState.onEfwServicesStopComplete();

        verify(mVpdNativeAdapterMock, times(0)).vpdFwStopComplete();
        verify(mVpdNativeAdapterMock, times(1)).vpdStopFailed();
        verify(mStateMachineMock, times(1)).transitionTo(StoppedState.getInstance());
        verify(mServicesRunningCheckerMock, times(1)).isAllServicesRunning();
        verify(mCheckCompletionMock, times(0)).isComplete();
        verifyNoMoreInteractions(mVpdNativeAdapterMock);
        verifyNoMoreInteractions(mStateMachineMock);
        verifyNoMoreInteractions(mServicesRunningCheckerMock);
        verifyZeroInteractions(mCheckCompletionMock);
        verifyZeroInteractions(mSubscriberNotificationMock);
    }

    @Test
    public void testOnEfwServicesStopCompleteServicesRunningCounterIncomplete() {
        when(mServicesRunningCheckerMock.isAllServicesRunning()).thenReturn(true);
        when(mCheckCompletionMock.isComplete()).thenReturn(false);

        mStopEfwServicesState.onEfwServicesStopComplete();

        verify(mVpdNativeAdapterMock, times(0)).vpdFwStopComplete();
        verify(mVpdNativeAdapterMock, times(1)).vpdStopFailed();
        verify(mStateMachineMock, times(1)).transitionTo(StoppedState.getInstance());
        verify(mServicesRunningCheckerMock, times(1)).isAllServicesRunning();
    }

    @Test
    public void testOnFwResume() {
        mStopEfwServicesState.onFwResume();

        verify(mStateMachineMock, times(1)).transitionTo(ResumeServicesState.getInstance());
        verifyNoMoreInteractions(mStateMachineMock);
        verifyZeroInteractions(mSubscriberNotificationMock);
        verifyZeroInteractions(mResetCounterMock);
        verifyZeroInteractions(mCheckCompletionMock);
        verifyZeroInteractions(mServicesRunningCheckerMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testOnFwRestart() {
        mStopEfwServicesState.onFwRestart();

        verify(mStateMachineMock, times(1)).transitionTo(RestartServicesState.getInstance());
        verifyNoMoreInteractions(mStateMachineMock);
        verifyZeroInteractions(mSubscriberNotificationMock);
        verifyZeroInteractions(mResetCounterMock);
        verifyZeroInteractions(mCheckCompletionMock);
        verifyZeroInteractions(mServicesRunningCheckerMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testOnCompletionTimeout() {
        mStopEfwServicesState.onCompletionTimeout();

        verify(mCheckCompletionMock, times(1)).logNotCompleted();
        verifyNoMoreInteractions(mCheckCompletionMock);
        verifyZeroInteractions(mSubscriberNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mResetCounterMock);
        verifyZeroInteractions(mServicesRunningCheckerMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void stateName() {
        Assert.assertEquals("StopEfwServicesState", mStopEfwServicesState.stateName());
    }
}
