/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.mountmanagement;

import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;
import java.util.concurrent.Executor;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class MountManagerTest {
    @Mock private IStorageManagerAdapter mockStorageManagerAdapter;

    @Mock private IVpdNativeAdapter mockVpdNativeAdapter;

    private MountManager mountManager;

    @Before
    public void setup() {
        mountManager =
                new MountManager(
                        mockStorageManagerAdapter,
                        mockVpdNativeAdapter,
                        new Executor() {
                            @Override
                            public void execute(Runnable command) {
                                command.run();
                            }
                        });
    }

    @Test
    public void mountTest() {
        mountManager.mount();

        verify(mockStorageManagerAdapter, times(1)).mountUnmounted();
    }

    @Test
    public void unmountTest() {
        mountManager.unmount();

        try {
            Thread.sleep(50); // this is needed because MountManager starts unmount operation
            // in a separate thread in order to prevent blocking main event
            // queue
        } catch (InterruptedException e) {
        }

        verify(mockStorageManagerAdapter, times(1)).unmountAll();
        verify(mockVpdNativeAdapter, times(1)).unmountComplete();
    }
}
