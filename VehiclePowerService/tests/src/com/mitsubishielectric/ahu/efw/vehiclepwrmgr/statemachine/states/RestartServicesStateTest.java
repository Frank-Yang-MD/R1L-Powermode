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
public class RestartServicesStateTest {
    @Mock private IVpsStateMachine mStateMachineMock;

    @Mock private ISubscriberNotification mFwServicesNotificationMock;

    @Mock private IVpdNativeAdapter mVpdNativeAdapterMock;

    @Mock private IResetCounter mResetCounter;

    @Mock ICheckCompletion mCheckCompletion;

    private RestartServicesState mRestartServicesState;

    @Before
    public void setUp() {
        RestartServicesState.initInstance(
                mStateMachineMock,
                mFwServicesNotificationMock,
                mResetCounter,
                mCheckCompletion,
                mVpdNativeAdapterMock);
        mRestartServicesState = RestartServicesState.getInstance();
    }

    @Test
    public void testOnEnterComplete() {
        when(mCheckCompletion.isComplete()).thenReturn(true);
        mRestartServicesState.onEnter();
        verify(mResetCounter, times(1)).reset();
        verify(mVpdNativeAdapterMock, times(1)).vpdFwRestartComplete();
        verify(mStateMachineMock, times(1)).transitionTo(RestartAppServicesState.getInstance());
        verifyZeroInteractions(mFwServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void testOnEnterNotComplete() {
        when(mCheckCompletion.isComplete()).thenReturn(false);
        mRestartServicesState.onEnter();
        verify(mResetCounter, times(1)).reset();
        verify(mFwServicesNotificationMock, times(1)).notifyAppRestart();
        verifyNoMoreInteractions(mFwServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testOnExit() {
        mRestartServicesState.onExit();
        verifyZeroInteractions(mFwServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testOnEfwServicesRestartComplete() {
        mRestartServicesState.onEfwServicesRestartComplete();
        verify(mVpdNativeAdapterMock, times(1)).vpdFwRestartComplete();
        verifyNoMoreInteractions(mVpdNativeAdapterMock);
        verifyZeroInteractions(mFwServicesNotificationMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void testOnAppRestart() {
        mRestartServicesState.onAppRestart();
        verify(mStateMachineMock, times(1)).transitionTo(RestartAppServicesState.getInstance());
        verifyNoMoreInteractions(mStateMachineMock);
        verifyZeroInteractions(mFwServicesNotificationMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }

    @Test
    public void testOnCompletionTimeout() {
        mRestartServicesState.onCompletionTimeout();
        verify(mCheckCompletion).logNotCompleted();
    }
}
