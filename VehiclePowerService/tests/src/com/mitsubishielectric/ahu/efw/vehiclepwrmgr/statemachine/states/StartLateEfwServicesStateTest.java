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
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class StartLateEfwServicesStateTest {
    @Mock private IVpsStateMachine mStateMachineMock;

    @Mock private IJavaRunner mRunnerMock;

    @Mock private IStartupInfo mStartupInfoMock;

    private StartLateEfwServicesState mStartLateEfwServicesState;

    @Before
    public void setUp() {
        StartLateEfwServicesState.initInstance(mStateMachineMock, mRunnerMock, mStartupInfoMock);
        mStartLateEfwServicesState = StartLateEfwServicesState.getInstance();
    }

    @Test
    public void testOnEnter() {
        mStartLateEfwServicesState.onEnter();
        verify(mRunnerMock, times(1)).start();
        verifyNoMoreInteractions(mRunnerMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void testOnExit() {
        mStartLateEfwServicesState.onExit();
        verify(mStartupInfoMock, times(1)).setAllServicesStarted(true);
        verifyNoMoreInteractions(mStartupInfoMock);
        verifyZeroInteractions(mRunnerMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void testOnLateEfwServicesStarted() {
        mStartLateEfwServicesState.onLateEfwServicesStarted();
        verify(mStateMachineMock, times(1)).transitionTo(StartLateAppServicesState.getInstance());
        verifyNoMoreInteractions(mStateMachineMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mRunnerMock);
    }
}
