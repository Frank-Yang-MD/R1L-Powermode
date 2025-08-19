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
public class ResumeServicesStateTest {
    @Mock private IVpsStateMachine mStateMachineMock;

    @Mock private ISubscriberNotification mFwServicesNotificationMock;

    @Mock private IVpdNativeAdapter mVpdNativeAdapterMock;

    @Mock private IResetCounter mResetCounter;

    @Mock ICheckCompletion mCheckCompletion;

    private ResumeServicesState mResumeServicesState;

    @Before
    public void setUp() {
        ResumeServicesState.initInstance(
                mStateMachineMock,
                mFwServicesNotificationMock,
                mResetCounter,
                mCheckCompletion,
                mVpdNativeAdapterMock);
        mResumeServicesState = ResumeServicesState.getInstance();
    }

    @Test
    public void testOnEnterComplete() {
        when(mCheckCompletion.isComplete()).thenReturn(true);
        mResumeServicesState.onEnter();
        verify(mResetCounter, times(1)).reset();
        verify(mVpdNativeAdapterMock, times(1)).vpdFwResumeComplete();
        verify(mStateMachineMock, times(1)).transitionTo(ResumeAppServicesState.getInstance());
        verifyZeroInteractions(mFwServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void testOnEnterNotComplete() {
        when(mCheckCompletion.isComplete()).thenReturn(false);
        mResumeServicesState.onEnter();
        verify(mResetCounter, times(1)).reset();
        verify(mFwServicesNotificationMock, times(1)).notifyAppResume();
        verifyNoMoreInteractions(mFwServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testOnExit() {
        mResumeServicesState.onExit();
        verifyZeroInteractions(mFwServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void onEfwServicesResumeComplete() {
        mResumeServicesState.onEfwServicesResumeComplete();
        verify(mVpdNativeAdapterMock, times(1)).vpdFwResumeComplete();
        verifyNoMoreInteractions(mVpdNativeAdapterMock);
        verifyZeroInteractions(mFwServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void testOnAppResume() {
        mResumeServicesState.onAppResume();
        verify(mStateMachineMock, times(1)).transitionTo(ResumeAppServicesState.getInstance());
        verifyNoMoreInteractions(mStateMachineMock);
        verifyZeroInteractions(mFwServicesNotificationMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testOnCompletionTimeout() {
        mResumeServicesState.onCompletionTimeout();
        verify(mCheckCompletion).logNotCompleted();
    }
}
