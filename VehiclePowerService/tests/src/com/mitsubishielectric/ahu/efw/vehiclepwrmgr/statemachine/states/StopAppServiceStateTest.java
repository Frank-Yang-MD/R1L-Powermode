/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import static org.mockito.Mockito.*;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ISubscriberNotification;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.ICheckCompletion;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.IResetCounter;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;

public class StopAppServiceStateTest {
    private StopAppServiceState mStopAppServiceState;
    private IVpsStateMachine mStateMachineMock;
    private IResetCounter mResetCounterMock;
    private ISubscriberNotification mSubscriptionNotificationMock;
    private IVpdNativeAdapter mVpdNativeAdapterMock;
    private ICheckCompletion mCheckCompletionMock;

    @Before
    public void beforeTest() {
        mStateMachineMock = mock(IVpsStateMachine.class);
        mResetCounterMock = mock(IResetCounter.class);
        mSubscriptionNotificationMock = mock(ISubscriberNotification.class);
        mVpdNativeAdapterMock = mock(IVpdNativeAdapter.class);
        mCheckCompletionMock = mock(ICheckCompletion.class);

        StopAppServiceState.initInstance(
                mStateMachineMock,
                mResetCounterMock,
                mCheckCompletionMock,
                mSubscriptionNotificationMock,
                mVpdNativeAdapterMock);

        mStopAppServiceState = StopAppServiceState.getInstance();
    }

    @Test
    public void onCreate() {
        Assert.assertNotNull(mStopAppServiceState);
    }

    @Test
    public void stateName() {
        Assert.assertEquals("StopAppServiceState", mStopAppServiceState.stateName());
    }

    @Test
    public void onEnter() {
        mStopAppServiceState.onEnter();

        verify(mResetCounterMock, times(1)).reset();
        verify(mSubscriptionNotificationMock, times(1)).notifyAppStop();
        verifyNoMoreInteractions(mResetCounterMock);
        verifyNoMoreInteractions(mSubscriptionNotificationMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void testAppServicesStopComplete() {
        mStopAppServiceState.onAppServicesStopComplete();

        verify(mVpdNativeAdapterMock, times(1)).vpdAppStopComplete();
        verifyNoMoreInteractions(mVpdNativeAdapterMock);
        verifyZeroInteractions(mResetCounterMock);
        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mSubscriptionNotificationMock);
    }

    @Test
    public void testOnFwStopTest() {
        mStopAppServiceState.onFwStop();

        verify(mStateMachineMock, times(1)).transitionTo(StopEfwServicesState.getInstance());
        verifyNoMoreInteractions(mStateMachineMock);
        verifyZeroInteractions(mResetCounterMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
        verifyZeroInteractions(mSubscriptionNotificationMock);
    }

    @Test
    public void testOnCompletionTimeout() {
        mStopAppServiceState.onCompletionTimeout();

        verify(mCheckCompletionMock, times(1)).logNotCompleted();
        verifyNoMoreInteractions(mCheckCompletionMock);
        verifyZeroInteractions(mResetCounterMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
        verifyZeroInteractions(mStateMachineMock);
        verifyZeroInteractions(mSubscriptionNotificationMock);
    }
}
