/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.verifyZeroInteractions;
import static org.mockito.Mockito.when;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ISubscriberNotification;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.ICheckCompletion;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.IResetCounter;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class ResumeAppServicesStateTest {
    @Mock private IVpsStateMachine mStateMachineMock;

    @Mock private ISubscriberNotification mAppServicesNotificationMock;

    @Mock private IVpdNativeAdapter mVpdNativeAdapterMock;

    @Mock private IResetCounter mResetCounter;

    @Mock ICheckCompletion mCheckCompletion;

    private ResumeAppServicesState mResumeServicesState;

    @Before
    public void setUp() {
        ResumeAppServicesState.initInstance(
                mStateMachineMock,
                mAppServicesNotificationMock,
                mResetCounter,
                mCheckCompletion,
                mVpdNativeAdapterMock);
        mResumeServicesState = ResumeAppServicesState.getInstance();
    }

    @Test
    public void testOnEnterComplete() {
        when(mCheckCompletion.isComplete()).thenReturn(true);
        mResumeServicesState.onEnter();
        verify(mResetCounter, times(1)).reset();
        verify(mVpdNativeAdapterMock, times(1)).vpdAppResumeComplete();
        verify(mStateMachineMock, times(1)).transitionTo(NormalOperationState.getInstance());
        verifyZeroInteractions(mAppServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void testOnEnterNotComplete() {
        when(mCheckCompletion.isComplete()).thenReturn(false);
        mResumeServicesState.onEnter();
        verify(mResetCounter, times(1)).reset();
        verify(mAppServicesNotificationMock, times(1)).notifyAppResume();
        verifyNoMoreInteractions(mAppServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testOnExit() {
        mResumeServicesState.onExit();
        verifyZeroInteractions(mAppServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void onAppServicesResumeComplete() {
        mResumeServicesState.onAppServicesResumeComplete();
        verify(mVpdNativeAdapterMock, times(1)).vpdAppResumeComplete();
        verify(mStateMachineMock, times(1)).transitionTo(NormalOperationState.getInstance());
        verifyNoMoreInteractions(mVpdNativeAdapterMock);
        verifyZeroInteractions(mAppServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void onCompletionTimeout() {
        mResumeServicesState.onCompletionTimeout();
        verify(mStateMachineMock, times(1)).transitionTo(NormalOperationState.getInstance());
        verify(mCheckCompletion, times(1)).logNotCompleted();
        verifyNoMoreInteractions(mStateMachineMock);
        verifyZeroInteractions(mAppServicesNotificationMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }
}
