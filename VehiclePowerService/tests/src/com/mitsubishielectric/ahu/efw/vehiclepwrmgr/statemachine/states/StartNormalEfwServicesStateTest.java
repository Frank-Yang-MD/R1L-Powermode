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
public class StartNormalEfwServicesStateTest {
    @Mock IVpsStateMachine mStateMachineMock;
    @Mock IJavaRunner mRunnerMock;
    @Mock IStartupInfo mStartupInfoMock;

    private StartNormalEfwServicesState uut;

    @Before
    public void init() {
        StartNormalEfwServicesState.initInstance(mStateMachineMock, mRunnerMock);
        uut = StartNormalEfwServicesState.getInstance();
    }

    @Test
    public void testOnEnter() {
        uut.onEnter();
        verify(mRunnerMock, times(1)).start();
        verifyNoMoreInteractions(mRunnerMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void testOnNormalEfwServicesStarted() {
        uut.onNormalEfwServicesStarted();
        verify(mStateMachineMock, times(1)).transitionTo(StartNormalAppServicesState.getInstance());
        verifyNoMoreInteractions(mStateMachineMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mRunnerMock);
    }
}
