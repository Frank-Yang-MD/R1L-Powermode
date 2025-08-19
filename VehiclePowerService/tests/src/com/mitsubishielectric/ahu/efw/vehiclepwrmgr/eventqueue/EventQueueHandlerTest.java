/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue;

import static junit.framework.Assert.assertFalse;
import static junit.framework.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyZeroInteractions;
import static org.mockito.Mockito.when;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IEventHandler;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IEventSource;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.ExecutorService;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.ArgumentCaptor;
import org.mockito.stubbing.Answer;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor({"com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog"})
@PrepareForTest({
    IEventSource.class,
    ExecutorService.class,
    MLog.class,
    EventQueueHandler.class,
    Thread.class
})
public class EventQueueHandlerTest {

    private IEventSource mEventSourceMock = mock(IEventSource.class);
    private ExecutorService mExecutorServiceMock = mock(ExecutorService.class);

    private Map mHandlersMock = mock(HashMap.class);
    private CompletableFuture mCompletableFutureMock = mock(CompletableFuture.class);

    ArgumentCaptor<Runnable> runnableArgumentCaptor = ArgumentCaptor.forClass(Runnable.class);

    private EventQueueHandler mEventQueueHandler;

    private IncomingEvents mEventMock = mock(IncomingEvents.class);
    private IEventHandler mHandlerMock = mock(IEventHandler.class);
    Thread mThreadMock = mock(Thread.class);

    public EventQueueHandlerTest() throws Exception {
        PowerMockito.mockStatic(Thread.class);
        PowerMockito.mockStatic(MLog.class);
        PowerMockito.mockStatic(CompletableFuture.class);

        mEventQueueHandler =
                new EventQueueHandler(mEventSourceMock, mExecutorServiceMock, mHandlersMock);

        PowerMockito.when(
                        CompletableFuture.runAsync(
                                runnableArgumentCaptor.capture(), eq(mExecutorServiceMock)))
                .thenAnswer((Answer<CompletableFuture>) invocation -> mCompletableFutureMock);

        PowerMockito.when(Thread.currentThread())
                .thenAnswer((Answer<Thread>) invocation -> mThreadMock);
    }

    @Test
    public void addEventHandler_addNewEventTest() {
        final String eventName = "InEventName";
        final boolean exist = false;

        IncomingEvents incomingEvent = mock(IncomingEvents.class);
        IEventHandler eventHandler = mock(IEventHandler.class);

        when(incomingEvent.name()).thenReturn(eventName);
        when(mHandlersMock.containsKey(incomingEvent)).thenReturn(exist);

        mEventQueueHandler.addEventHandler(incomingEvent, eventHandler);

        verify(mHandlersMock).put(incomingEvent, eventHandler);
    }

    @Test
    public void addEventHandler_addExistEventTest() {
        final String eventName = "InEventName";
        final boolean exist = true;

        IncomingEvents incomingEvent = mock(IncomingEvents.class);
        IEventHandler eventHandler = mock(IEventHandler.class);

        when(incomingEvent.name()).thenReturn(eventName);
        when(mHandlersMock.containsKey(incomingEvent)).thenReturn(exist);

        mEventQueueHandler.addEventHandler(incomingEvent, eventHandler);

        verify(mHandlersMock).put(incomingEvent, eventHandler);
    }

    @Test
    public void startTest() {
        assertTrue(mEventQueueHandler.start());
    }

    @Test
    public void startSeveralTimesTest() {
        assertTrue(mEventQueueHandler.start());
        assertFalse(mEventQueueHandler.start());
        assertFalse(mEventQueueHandler.start());
    }

    @Test
    public void stopWithoutStartTest() {
        mEventQueueHandler.stop();

        verifyZeroInteractions(mCompletableFutureMock, mExecutorServiceMock);
    }

    @Test
    public void stopAfterStartTest() {
        assertTrue(mEventQueueHandler.start());
        mEventQueueHandler.stop();

        verify(mExecutorServiceMock).shutdownNow();
        verify(mCompletableFutureMock).join();
    }

    @Test
    public void handleEventWithNullEventTest() {
        assertTrue(mEventQueueHandler.start());

        Runnable task = runnableArgumentCaptor.getValue();
        when(mEventSourceMock.getEvent()).thenReturn(null);

        task.run();

        verifyZeroInteractions(mHandlersMock);
    }

    @Test
    public void handleEventWithStopAfterStartEventTest() {
        mEventQueueHandler.start();
        Runnable task = runnableArgumentCaptor.getValue();
        mEventQueueHandler.stop();

        when(mEventSourceMock.getEvent()).thenReturn(null);

        task.run();

        verifyZeroInteractions(mHandlersMock);
    }

    @Test
    public void handleEventWithStopAfterStartAndThreadInterruptedEventTest() {
        when(mThreadMock.isInterrupted()).thenReturn(true);

        mEventQueueHandler.start();
        Runnable task = runnableArgumentCaptor.getValue();
        mEventQueueHandler.stop();

        when(mEventSourceMock.getEvent()).thenReturn(null);

        task.run();

        verifyZeroInteractions(mHandlersMock);
    }

    @Test
    public void handleEventWithStartAndThreadInterruptedEventTest() {
        when(mThreadMock.isInterrupted()).thenReturn(true);

        mEventQueueHandler.start();
        Runnable task = runnableArgumentCaptor.getValue();

        when(mEventSourceMock.getEvent()).thenReturn(null);

        task.run();

        verifyZeroInteractions(mHandlersMock);
    }

    @Test
    public void handleEventWithNullHandlerTest() {
        assertTrue(mEventQueueHandler.start());
        Runnable task = runnableArgumentCaptor.getValue();
        when(mEventSourceMock.getEvent()).thenReturn(mEventMock);
        when(mHandlersMock.get(mEventMock)).thenReturn(null);

        task.run();

        verify(mHandlersMock).get(mEventMock);
    }

    @Test
    public void handleEventWithNotNullHandlerTest() {
        assertTrue(mEventQueueHandler.start());

        Runnable task = runnableArgumentCaptor.getValue();
        when(mEventSourceMock.getEvent()).thenReturn(mEventMock);
        when(mHandlersMock.get(mEventMock)).thenReturn(mHandlerMock);

        task.run();

        verify(mHandlerMock).process(mEventMock);
    }
}
