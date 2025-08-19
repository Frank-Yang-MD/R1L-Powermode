/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.subscriptionmanager;

import static org.mockito.Mockito.verify;

import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EVpsState;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IVehiclePowerServiceListener;
import java.util.concurrent.ThreadLocalRandom;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class SubscriptionManagerTest {
    @Mock private IVehiclePowerServiceListener mListener;

    @Mock private SubscriptionRemoteCallbackList mSubscriptionRemoteCallbackList;

    @InjectMocks private SubscriptionManager mSubscriptionManager;

    @Test
    public void notifyAppRestart() {
        // Act
        mSubscriptionManager.notifyAppRestart();

        // Assert
        verify(mSubscriptionRemoteCallbackList).notifyAllListeners(EVpsState.APP_RESTART);
    }

    @Test
    public void notifyAppResume() {
        // Act
        mSubscriptionManager.notifyAppResume();

        // Assert
        verify(mSubscriptionRemoteCallbackList).notifyAllListeners(EVpsState.APP_RESUME);
    }

    @Test
    public void notifyAppStart() {
        // Act
        mSubscriptionManager.notifyAppStart();

        // Assert
        verify(mSubscriptionRemoteCallbackList).notifyAllListeners(EVpsState.APP_START_COLDBOOT);
    }

    @Test
    public void notifyAppStop() {
        // Act
        mSubscriptionManager.notifyAppStop();

        // Assert
        verify(mSubscriptionRemoteCallbackList).notifyAllListeners(EVpsState.APP_STOP);
    }

    @Test
    public void subscribe() {
        // Before
        String listenerPid = String.valueOf(ThreadLocalRandom.current().nextInt());

        // Act
        mSubscriptionManager.subscribe(mListener, listenerPid);

        // Assert
        verify(mSubscriptionRemoteCallbackList).subscribe(mListener, listenerPid);
    }

    @Test
    public void unsubscribe() {
        // Before
        String listenerPid = String.valueOf(ThreadLocalRandom.current().nextInt());

        // Act
        mSubscriptionManager.unsubscribe(mListener, listenerPid);

        // Assert
        verify(mSubscriptionRemoteCallbackList).unsubscribe(mListener, listenerPid);
    }
}
