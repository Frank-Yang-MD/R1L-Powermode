/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.smadapters;

import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.verifyZeroInteractions;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IStartHandler;
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
public class StartHandlerTest {
    @Mock private IStartHandler mIStartHandlerMock;

    @InjectMocks private StartHandler mStartHandler;

    @Before
    public void setUp() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
    }

    /** Test for handling null argument in constructor. */
    @Test(expected = IllegalArgumentException.class)
    public void constructor() {
        StartHandler localStartHandler = new StartHandler(null);
    }

    /** Test for properly method calling inside */
    @Test
    public void testNormalEfwServicesStart() {
        mStartHandler.process(IncomingEvents.NORMAL_EFW_SERVICES_STARTED);
        verify(mIStartHandlerMock, times(1)).onNormalEfwServicesStarted();
        verifyNoMoreInteractions(mIStartHandlerMock);
    }

    @Test
    public void testLateEfwServicesStart() {
        mStartHandler.process(IncomingEvents.LATE_EFW_SERVICES_STARTED);
        verify(mIStartHandlerMock, times(1)).onLateEfwServicesStarted();
        verifyNoMoreInteractions(mIStartHandlerMock);
    }

    @Test
    public void testNormalAppServicesStart() {
        mStartHandler.process(IncomingEvents.NORMAL_APP_SERVICES_STARTED);
        verify(mIStartHandlerMock, times(1)).onNormalAppServicesStarted();
        verifyNoMoreInteractions(mIStartHandlerMock);
    }

    @Test
    public void testLateAppServicesStart() {
        mStartHandler.process(IncomingEvents.LATE_APP_SERVICES_STARTED);
        verify(mIStartHandlerMock, times(1)).onLateAppServicesStarted();
        verifyNoMoreInteractions(mIStartHandlerMock);
    }

    /** Test for properly null argument handling */
    @Test(expected = IllegalArgumentException.class)
    public void processNullArg() {
        try {
            mStartHandler.process(null);
        } finally {
            // For case if exception throws after method calling
            verify(mIStartHandlerMock, never()).onNormalEfwServicesStarted();
            verify(mIStartHandlerMock, never()).onLateAppServicesStarted();
        }
    }

    /** Test for properly wrong argument case handling */
    @Test(expected = IllegalArgumentException.class)
    public void processInvalidArg() {
        try {
            mStartHandler.process(IncomingEvents.APP_STOP);
        } finally {
            // For case if exception throws after method calling
            verifyZeroInteractions(mIStartHandlerMock);
        }
    }
}
