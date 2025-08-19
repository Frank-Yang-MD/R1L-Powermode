/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine;

import static org.mockito.Mockito.never;
import static org.mockito.Mockito.spy;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor("com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog")
public class VpsStateMachineTest {
    private final VpsStateMachine mStateMachine = new VpsStateMachine();

    private TestState mInitialState;

    @Mock private IVpsState mFinalState;

    @Before
    public void setUp() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
        mInitialState = spy(new TestState(mStateMachine));
        mStateMachine.transitionTo(mInitialState);
    }

    @Test
    public void onAppStartColdboot() {
        mStateMachine.onAppStartColdboot();
        verify(mInitialState).onAppStartColdboot();
    }

    @Test
    public void onAppRestart() {
        mStateMachine.onAppRestart();
        verify(mInitialState).onAppRestart();
    }

    @Test
    public void onAppResume() {
        mStateMachine.onAppResume();
        verify(mInitialState).onAppResume();
    }

    @Test
    public void onAppStop() {
        mStateMachine.onAppStop();
        verify(mInitialState).onAppStop();
    }

    @Test
    public void onFwRestart() {
        mStateMachine.onFwRestart();
        verify(mInitialState).onFwRestart();
    }

    @Test
    public void onFwResume() {
        mStateMachine.onFwResume();
        verify(mInitialState).onFwResume();
    }

    @Test
    public void onFwStop() {
        mStateMachine.onFwStop();
        verify(mInitialState).onFwStop();
    }

    @Test
    public void onNormalAppsStarted() {
        mStateMachine.onNormalAppServicesStarted();
        verify(mInitialState).onNormalAppServicesStarted();
    }

    @Test
    public void onLateAppsStarted() {
        mStateMachine.onLateAppServicesStarted();
        verify(mInitialState).onLateAppServicesStarted();
    }

    @Test
    public void onNormalEfwServicesStarted() {
        mStateMachine.onNormalEfwServicesStarted();
        verify(mInitialState).onNormalEfwServicesStarted();
    }

    @Test
    public void onLateEfwServicesStarted() {
        mStateMachine.onLateEfwServicesStarted();
        verify(mInitialState).onLateEfwServicesStarted();
    }

    @Test
    public void onAppsStopped() {
        mStateMachine.onAppsStopped();
        verify(mInitialState).onAppServicesStopComplete();
    }

    @Test
    public void onServicesStopped() {
        mStateMachine.onServicesStopped();
        verify(mInitialState).onEfwServicesStopComplete();
    }

    @Test
    public void onFwRestartComplete() {
        mStateMachine.onEfwRestartCompleted();
        verify(mInitialState).onEfwServicesRestartComplete();
    }

    @Test
    public void onFwResumeComplete() {
        mStateMachine.onEfwResumeCompleted();
        verify(mInitialState).onEfwServicesResumeComplete();
    }

    @Test
    public void onFwStopComplete() {
        mStateMachine.onEfwServicesStopCompleted();
        verify(mInitialState).onEfwServicesStopComplete();
    }

    @Test
    public void onAppStopComplete() {
        mStateMachine.onAppServicesStopCompleted();
        verify(mInitialState).onAppServicesStopComplete();
    }

    @Test
    public void transitionTo() {
        when(mFinalState.stateName()).thenReturn("Unit_test_state");
        mStateMachine.transitionTo(mFinalState);

        verify(mInitialState).onExit();
        verify(mFinalState).onEnter();
    }

    @Test
    public void transitionToNull() {
        mStateMachine.transitionTo(null);
        verify(mInitialState, never()).onExit();
        verify(mFinalState, never()).onEnter();
    }

    @Test
    public void onAppServicesRestartComplete() {
        mStateMachine.onAppServicesRestartComplete();
        verify(mInitialState).onAppServicesRestartComplete();
    }

    @Test
    public void onAppServicesResumeComplete() {
        mStateMachine.onAppServicesResumeComplete();
        verify(mInitialState).onAppServicesResumeComplete();
    }

    @Test
    public void onCompletionTimeout() {
        mStateMachine.onCompletionTimeout();
        verify(mInitialState).onCompletionTimeout();
    }

    private class TestState extends VpsAbstractState {
        TestState(IVpsStateMachine stateMachine) {
            super(stateMachine);
        }
    }
}
