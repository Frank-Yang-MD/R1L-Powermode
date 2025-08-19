/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.smadapters;

import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.verifyZeroInteractions;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine.ICompletionHandler;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class CompletionHandlerTest {
    @Mock private ICompletionHandler mCompletionHandlerMock;

    private CompletionHandler mCompletionHandler;

    @Before
    public void setUp() {
        mCompletionHandler = new CompletionHandler(mCompletionHandlerMock);
    }

    @Test(expected = NullPointerException.class)
    public void constructor() {
        new CompletionHandler(null);
    }

    @Test
    public void testFwRestartComplete() {
        mCompletionHandler.process(IncomingEvents.EFW_SERVICES_RESTART_COMPLETE);
        verify(mCompletionHandlerMock, times(1)).onEfwRestartCompleted();
        verifyNoMoreInteractions(mCompletionHandlerMock);
    }

    @Test
    public void testFwResumeComplete() {
        mCompletionHandler.process(IncomingEvents.EFW_SERVICES_RESUME_COMPLETE);
        verify(mCompletionHandlerMock, times(1)).onEfwResumeCompleted();
        verifyNoMoreInteractions(mCompletionHandlerMock);
    }

    @Test
    public void testFwAppServicesResumeComplete() {
        mCompletionHandler.process(IncomingEvents.EFW_APP_SERVICES_RESUME_COMPLETE);
        verify(mCompletionHandlerMock, times(1)).onAppServicesResumeComplete();
        verifyNoMoreInteractions(mCompletionHandlerMock);
    }

    @Test
    public void testFwAppServicesRestartComplete() {
        mCompletionHandler.process(IncomingEvents.EFW_APP_SERVICES_RESTART_COMPLETE);
        verify(mCompletionHandlerMock, times(1)).onAppServicesRestartComplete();
        verifyNoMoreInteractions(mCompletionHandlerMock);
    }

    @Test
    public void testFwStopComplete() {
        mCompletionHandler.process(IncomingEvents.EFW_SERVICES_STOP_COMPLETE);
        verify(mCompletionHandlerMock, times(1)).onEfwServicesStopCompleted();
        verifyNoMoreInteractions(mCompletionHandlerMock);
    }

    @Test
    public void testAppStopComplete() {
        mCompletionHandler.process(IncomingEvents.APP_SERVICES_STOP_COMPLETE);
        verify(mCompletionHandlerMock, times(1)).onAppServicesStopCompleted();
        verifyNoMoreInteractions(mCompletionHandlerMock);
    }

    @Test
    public void testCompletionTimeout() {
        mCompletionHandler.process(IncomingEvents.COMPLETION_TIMEOUT);
        verify(mCompletionHandlerMock, times(1)).onCompletionTimeout();
        verifyNoMoreInteractions(mCompletionHandlerMock);
    }

    @Test(expected = IllegalArgumentException.class)
    public void processInvalidArg() {
        try {
            mCompletionHandler.process(IncomingEvents.APP_STOP);
        } finally {
            verifyZeroInteractions(mCompletionHandlerMock);
        }
    }
}
