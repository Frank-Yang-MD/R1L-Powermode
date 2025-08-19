/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.stateholder;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EVpsState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IEventReceiver;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor("com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog")
public class StateHolderTest {
    @Mock private IEventReceiver mEventReceiver;

    @InjectMocks private StateHolder mStateHolder;

    @Before
    public void setUp() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
    }

    @Test
    public void onStateChangeColdboot() {
        // Act
        mStateHolder.onStateChange(1);

        // Assert
        verify(mEventReceiver, times(1)).onReceiveEvent(IncomingEvents.APP_START_COLDBOOT);
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_STOP);
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_RESUME);
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_RESTART);
        assertEquals(EVpsState.APP_START_COLDBOOT, mStateHolder.getAppState());
    }

    @Test
    public void onStateChangeStop() {
        // Act
        mStateHolder.onStateChange(2);

        // Assert
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_START_COLDBOOT);
        verify(mEventReceiver, times(1)).onReceiveEvent(IncomingEvents.APP_STOP);
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_RESUME);
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_RESTART);
        assertEquals(EVpsState.APP_STOP, mStateHolder.getAppState());
    }

    @Test
    public void onStateChangeResume() {
        // Act
        mStateHolder.onStateChange(3);

        // Assert
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_START_COLDBOOT);
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_STOP);
        verify(mEventReceiver, times(1)).onReceiveEvent(IncomingEvents.APP_RESUME);
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_RESTART);
        assertEquals(EVpsState.APP_RESUME, mStateHolder.getAppState());
    }

    @Test
    public void onStateChangeRestart() {
        // Act
        mStateHolder.onStateChange(4);

        // Assert
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_START_COLDBOOT);
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_STOP);
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_RESUME);
        verify(mEventReceiver, times(1)).onReceiveEvent(IncomingEvents.APP_RESTART);
        assertEquals(EVpsState.APP_RESTART, mStateHolder.getAppState());
    }

    @Test
    public void onStateChangeZerro() {
        // Act
        mStateHolder.onStateChange(0);

        // Assert
        assertEquals(EVpsState.APP_START_NA, mStateHolder.getAppState());
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_START_COLDBOOT);
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_STOP);
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_RESUME);
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_RESTART);
    }

    @Test
    public void onStateChangeOutOfRange() {
        // Act
        for (int i = 5; i < 256; i++) {
            mStateHolder.onStateChange(0);
        }

        // Assert
        assertEquals(EVpsState.APP_START_NA, mStateHolder.getAppState());
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_START_COLDBOOT);
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_STOP);
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_RESUME);
        verify(mEventReceiver, never()).onReceiveEvent(IncomingEvents.APP_RESTART);
    }

    @Test
    public void onFwRestartTest() {
        mStateHolder.onFwRestart();

        verify(mEventReceiver, times(1)).onReceiveEvent(IncomingEvents.FW_RESTART);
        verifyNoMoreInteractions(mEventReceiver);
    }

    @Test
    public void onFwResumeTest() {
        mStateHolder.onFwResume();

        verify(mEventReceiver, times(1)).onReceiveEvent(IncomingEvents.FW_RESUME);
        verifyNoMoreInteractions(mEventReceiver);
    }

    @Test
    public void onAppRestartTest() {
        mStateHolder.onAppRestart();

        verify(mEventReceiver, times(1)).onReceiveEvent(IncomingEvents.APP_RESTART);
        verifyNoMoreInteractions(mEventReceiver);
    }

    @Test
    public void onAppResumeTest() {
        mStateHolder.onAppResume();

        verify(mEventReceiver, times(1)).onReceiveEvent(IncomingEvents.APP_RESUME);
        verifyNoMoreInteractions(mEventReceiver);
    }

    @Test
    public void onFwStopTest() {
        mStateHolder.onFwStop();

        verify(mEventReceiver, times(1)).onReceiveEvent(IncomingEvents.FW_STOP);
        verifyNoMoreInteractions(mEventReceiver);
    }

    @Test
    public void onCompletionTimeoutTest() {
        mStateHolder.onCompletionTimeout();

        verify(mEventReceiver, times(1)).onReceiveEvent(IncomingEvents.COMPLETION_TIMEOUT);
        verifyNoMoreInteractions(mEventReceiver);
    }

    @Test
    public void onRequestMountTest() {
        mStateHolder.requestMount();

        verify(mEventReceiver, times(1)).onReceiveEvent(IncomingEvents.REQUEST_MOUNT);
        verifyNoMoreInteractions(mEventReceiver);
    }

    @Test
    public void onRequestUnMountTest() {
        mStateHolder.requestUnmount();

        verify(mEventReceiver, times(1)).onReceiveEvent(IncomingEvents.REQUEST_UNMOUNT);
        verifyNoMoreInteractions(mEventReceiver);
    }
}
