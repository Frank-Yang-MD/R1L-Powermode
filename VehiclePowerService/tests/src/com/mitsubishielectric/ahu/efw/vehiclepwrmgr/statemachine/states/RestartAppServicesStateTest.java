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
public class RestartAppServicesStateTest {
    @Mock private IVpsStateMachine mStateMachineMock;

    @Mock private ISubscriberNotification mAppServicesNotificationMock;

    @Mock private IVpdNativeAdapter mVpdNativeAdapterMock;

    @Mock private IResetCounter mResetCounter;

    @Mock ICheckCompletion mCheckCompletion;

    private RestartAppServicesState mRestartServicesState;

    @Before
    public void setUp() {
        RestartAppServicesState.initInstance(
                mStateMachineMock,
                mAppServicesNotificationMock,
                mResetCounter,
                mCheckCompletion,
                mVpdNativeAdapterMock);
        mRestartServicesState = RestartAppServicesState.getInstance();
    }

    @Test
    public void testOnEnterComplete() {
        when(mCheckCompletion.isComplete()).thenReturn(true);
        mRestartServicesState.onEnter();
        verify(mResetCounter, times(1)).reset();
        verify(mVpdNativeAdapterMock, times(1)).vpdAppRestartComplete();
        verify(mStateMachineMock, times(1)).transitionTo(NormalOperationState.getInstance());
        verifyZeroInteractions(mAppServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void testOnEnterNotComplete() {
        when(mCheckCompletion.isComplete()).thenReturn(false);
        mRestartServicesState.onEnter();
        verify(mResetCounter, times(1)).reset();
        verify(mAppServicesNotificationMock, times(1)).notifyAppRestart();
        verifyNoMoreInteractions(mAppServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testOnExit() {
        mRestartServicesState.onExit();
        verifyZeroInteractions(mAppServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testOnAppServicesRestartComplete() {
        mRestartServicesState.onAppServicesRestartComplete();
        verify(mVpdNativeAdapterMock, times(1)).vpdAppRestartComplete();
        verify(mStateMachineMock, times(1)).transitionTo(NormalOperationState.getInstance());
        verifyNoMoreInteractions(mVpdNativeAdapterMock);
        verifyZeroInteractions(mAppServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void testOnCompletionTimeout() {
        mRestartServicesState.onCompletionTimeout();
        verify(mStateMachineMock, times(1)).transitionTo(NormalOperationState.getInstance());
        verify(mCheckCompletion, times(1)).logNotCompleted();
        verifyNoMoreInteractions(mStateMachineMock);
        verifyZeroInteractions(mAppServicesNotificationMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }
}
