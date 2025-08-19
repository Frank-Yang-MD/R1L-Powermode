/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import static junit.framework.Assert.assertNotNull;
import static junit.framework.Assert.assertNull;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class NormalOperationStateTest {
    @Mock private IVpsStateMachine mMockIVpsStateMachine;

    /** Test of normal state initialization */
    @Test
    public void getInstance() {
        NormalOperationState.initInstance(mMockIVpsStateMachine);
        assertNotNull(NormalOperationState.getInstance());
    }

    /** getInstance should return null if initInstance is not called before */
    @Test
    public void getInstanceBeforeInitInstance() {
        assertNull(NormalOperationState.getInstance());
    }

    /**
     * Checks that state transition to StopAppServiceState will occur if onAppState event happens
     */
    @Test
    public void onAppStop() {
        NormalOperationState.initInstance(mMockIVpsStateMachine);

        NormalOperationState.getInstance().onAppStop();

        verify(mMockIVpsStateMachine, times(1)).transitionTo(StopAppServiceState.getInstance());
    }
}
