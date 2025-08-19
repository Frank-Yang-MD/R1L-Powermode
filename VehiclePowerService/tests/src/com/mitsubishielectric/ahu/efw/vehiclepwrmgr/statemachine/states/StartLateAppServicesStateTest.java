/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.verifyZeroInteractions;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IJavaRunner;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class StartLateAppServicesStateTest {
    @Mock private IVpsStateMachine mStateMachineMock;

    @Mock private IJavaRunner mRunnerMock;

    @Mock private IVpdNativeAdapter mVpdNativeAdapterMock;

    @Mock private IStartupInfo mStartupInfoMock;

    private StartLateAppServicesState mStartLateAppServicesState;

    @Before
    public void setUp() {
        StartLateAppServicesState.initInstance(
                mStateMachineMock, mRunnerMock,
                mVpdNativeAdapterMock, mStartupInfoMock);
        mStartLateAppServicesState = StartLateAppServicesState.getInstance();
    }

    @Test
    public void testOnEnter() {
        mStartLateAppServicesState.onEnter();
        verify(mRunnerMock, times(1)).start();
        verifyNoMoreInteractions(mRunnerMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void testOnExit() {
        mStartLateAppServicesState.onExit();
        verify(mVpdNativeAdapterMock, times(1)).vpdStartComplete();
        verify(mStartupInfoMock, times(1)).setAllAppsStarted(true);
        verifyNoMoreInteractions(mVpdNativeAdapterMock);
        verifyNoMoreInteractions(mStartupInfoMock);
        verifyZeroInteractions(mRunnerMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void testOnLateAppServicesStarted() {
        mStartLateAppServicesState.onLateAppServicesStarted();
        verify(mStateMachineMock, times(1)).transitionTo(NormalOperationState.getInstance());
        verifyNoMoreInteractions(mStateMachineMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mRunnerMock);
        verifyZeroInteractions(mVpdNativeAdapterMock);
    }
}
