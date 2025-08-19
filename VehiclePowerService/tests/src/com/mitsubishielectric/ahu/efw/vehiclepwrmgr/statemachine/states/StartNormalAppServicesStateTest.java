/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.states;

import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.verifyZeroInteractions;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IJavaRunner;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.broadcastsender.IBroadcastSender;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.IVpsStateMachine;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnit;
import org.mockito.junit.MockitoRule;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor("com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog")
public class StartNormalAppServicesStateTest {
    @Rule public MockitoRule rule = MockitoJUnit.rule();

    @Mock private IVpsStateMachine mStateMachineMock;

    @Mock private IJavaRunner mRunnerMock;

    @Mock private IStartupInfo mStartupInfoMock;

    @Mock private IVpdNativeAdapter mVpdNativeAdapter;

    @Mock private IBroadcastSender mBroadcastSender;

    private StartNormalAppServicesState mStartNormalAppServicesState;

    @Before
    public void setUp() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
        StartNormalAppServicesState.initInstance(
                mStateMachineMock,
                mRunnerMock,
                mStartupInfoMock,
                mVpdNativeAdapter,
                mBroadcastSender);
        mStartNormalAppServicesState = StartNormalAppServicesState.getInstance();
    }

    @Test
    public void testOnEnter() {
        mStartNormalAppServicesState.onEnter();
        verify(mRunnerMock, times(1)).start();
        verifyNoMoreInteractions(mRunnerMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void testOnExit() {
        mStartNormalAppServicesState.onExit();
        verify(mStartupInfoMock, times(1)).setEarlyAppsStarted(true);
        verify(mVpdNativeAdapter, times(1)).hmiIsReady();
        verifyNoMoreInteractions(mStartupInfoMock);
        verifyZeroInteractions(mRunnerMock);
        verifyZeroInteractions(mStateMachineMock);
    }

    @Test
    public void testOnNormalAppServicesStarted() {
        StartLateEfwServicesState.initInstance(mStateMachineMock, mRunnerMock, mStartupInfoMock);
        mStartNormalAppServicesState.onNormalAppServicesStarted();
        verify(mStateMachineMock, times(1)).transitionTo(StartLateEfwServicesState.getInstance());
        verifyNoMoreInteractions(mStateMachineMock);
        verifyZeroInteractions(mStartupInfoMock);
        verifyZeroInteractions(mRunnerMock);
    }
}
