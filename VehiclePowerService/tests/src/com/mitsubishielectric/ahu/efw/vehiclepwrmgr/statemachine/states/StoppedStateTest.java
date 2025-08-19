/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnit;
import org.mockito.junit.MockitoRule;

public class StoppedStateTest {
    private StoppedState mStoppedState;

    @Rule public MockitoRule rule = MockitoJUnit.rule();

    @Mock private IVpsStateMachine mStateMachineMock;

    @Before
    public void setUp() {
        StoppedState.initInstance(mStateMachineMock);
        mStoppedState = StoppedState.getInstance();
    }

    @Test
    public void testOnFwResume() {
        mStoppedState.onFwResume();
        verify(mStateMachineMock, times(1)).transitionTo(ResumeServicesState.getInstance());
        verifyNoMoreInteractions(mStateMachineMock);
    }

    @Test
    public void testOnFwRestart() {
        mStoppedState.onFwRestart();
        verify(mStateMachineMock, times(1)).transitionTo(RestartServicesState.getInstance());
        verifyNoMoreInteractions(mStateMachineMock);
    }
}
