/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import java.util.concurrent.BlockingQueue;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ExpectedException;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor("com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog")
public class EventQueueTest {
    @Rule public ExpectedException thrown = ExpectedException.none();

    @Mock private BlockingQueue<IncomingEvents> mQueue;

    @InjectMocks private EventQueue mEventQueue;

    @Before
    public void setUp() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
    }

    /** Check EventQueue will call BlockingQueue.put if onReceiveEvent method calls */
    @Test
    public void onReceiveEvent() throws InterruptedException {
        // Act
        mEventQueue.onReceiveEvent(IncomingEvents.LATE_APP_SERVICES_STARTED);

        // Assert
        verify(mQueue, times(1)).put(IncomingEvents.LATE_APP_SERVICES_STARTED);
    }

    /**
     * Check EventQueue will call BlockingQueue.put and catch the InterruptedException if
     * onReceiveEvent method calls and InterruptException occurs in method put
     */
    @Test
    public void onReceiveEventWhenInterruptException() throws InterruptedException {
        // Arrange
        doThrow(new InterruptedException("test")).when(mQueue).put(any());

        // Act
        mEventQueue.onReceiveEvent(IncomingEvents.LATE_APP_SERVICES_STARTED);

        // Assert
        verify(mQueue, times(1)).put(IncomingEvents.LATE_APP_SERVICES_STARTED);
    }

    /** Check EventQueue will call BlockingQueue.take if getEvent method calls */
    @Test
    public void getEvent() throws InterruptedException {
        // For Logdog
        IncomingEvents events = mock(IncomingEvents.class);
        when(events.name()).thenReturn("Unit_test");
        when(mQueue.take()).thenReturn(events);

        // Act
        mEventQueue.getEvent();

        // Assert
        verify(mQueue, times(1)).take();
    }

    /**
     * Check EventQueue will call BlockingQueue.take and catch the InterruptedException if getEvent
     * method calls and InterruptException occurs in method take
     */
    @Test
    public void getEventWithInterruptException() throws InterruptedException {
        // Arrange
        doThrow(new InterruptedException("test")).when(mQueue).take();

        // Act
        mEventQueue.getEvent();

        // Assert
        verify(mQueue, times(1)).take();
    }
}
