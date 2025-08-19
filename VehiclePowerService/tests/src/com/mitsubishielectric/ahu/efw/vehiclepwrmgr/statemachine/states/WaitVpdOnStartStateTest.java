/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class WaitVpdOnStartStateTest {
    @Mock private IVpsStateMachine mStateMachineMock;

    private WaitVpdOnStartState mWaitVpdOnStartState;

    @Before
    public void setUp() {
        WaitVpdOnStartState.initInstance(mStateMachineMock);
        mWaitVpdOnStartState = WaitVpdOnStartState.getInstance();
    }

    @Test
    public void onAppStartColdboot() {
        mWaitVpdOnStartState.onAppStartColdboot();
        verify(mStateMachineMock, times(1)).transitionTo(StartNormalEfwServicesState.getInstance());
        verifyNoMoreInteractions(mStateMachineMock);
    }
}
