/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.mountmanagement;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.smadapters.MountEventHandler;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class MountEventHandlerTest {
    @Mock private IMountManager mockMountManager;

    private MountEventHandler mountEventHandler;

    @Before
    public void setup() {
        mountEventHandler = new MountEventHandler(mockMountManager);
    }

    @Test
    public void unmountTest() {
        mountEventHandler.process(IncomingEvents.REQUEST_UNMOUNT);

        verify(mockMountManager, times(0)).mount();
        verify(mockMountManager, times(1)).unmount();
    }

    @Test
    public void mountTest() {
        mountEventHandler.process(IncomingEvents.REQUEST_MOUNT);

        verify(mockMountManager, times(0)).unmount();
        verify(mockMountManager, times(1)).mount();
    }

    @Test(expected = IllegalArgumentException.class)
    public void nullEventTest() {
        mountEventHandler.process(null);
    }

    @Test
    public void noFalseCallsTest() {
        List<IncomingEvents> incomingEvents =
                new ArrayList<>(Arrays.asList(IncomingEvents.values()));

        incomingEvents.remove(IncomingEvents.REQUEST_UNMOUNT);
        incomingEvents.remove(IncomingEvents.REQUEST_MOUNT);

        int exceptionCounter = 0;
        for (IncomingEvents event : incomingEvents) {
            try {
                mountEventHandler.process(event);
            } catch (IllegalArgumentException e) {
                exceptionCounter++;
            }
        }

        verify(mockMountManager, times(0)).unmount();
        verify(mockMountManager, times(0)).mount();
        assertEquals(exceptionCounter, incomingEvents.size());
    }
}
