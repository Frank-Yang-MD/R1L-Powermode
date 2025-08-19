/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.subscriptionmanager;

import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.spy;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyZeroInteractions;
import static org.mockito.Mockito.when;

import android.os.RemoteException;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EVpsState;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IVehiclePowerServiceListener;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IStateHolder;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.ISubscriptionNotification;
import java.util.concurrent.ThreadLocalRandom;
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
public class SubscriptionRemoteCallbackListTest {
    @Mock private IVehiclePowerServiceListener mListener;

    @Mock private IStateHolder mStateHolder;

    @Mock private ISubscriptionNotification mSubscriptionNotification;

    private SubscriptionRemoteCallbackList uut;

    private final int NUM_OF_LISTENERS = 1;
    private final int INDEX_OF_FIRST_LISTENER = 0;

    @Before
    public void setUp() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
        uut = spy(new SubscriptionRemoteCallbackList(mStateHolder, mSubscriptionNotification));
        when(mStateHolder.getAppState()).thenReturn(EVpsState.APP_START_COLDBOOT);
        when(uut.beginBroadcast()).thenReturn(NUM_OF_LISTENERS);
        when(uut.getBroadcastItem(INDEX_OF_FIRST_LISTENER)).thenReturn(mListener);
    }

    @Test
    public void onCallbackDied() {
        // Before
        Object cookie = String.valueOf(ThreadLocalRandom.current().nextInt());

        // Act
        uut.onCallbackDied(mListener, cookie);

        // Assert
        verify(mSubscriptionNotification, times(1)).onUnsubscribe((String) cookie);
    }

    @Test
    public void notifyListener_AppStartColdboot() throws RemoteException {
        // Act
        uut.notifyAllListeners(EVpsState.APP_START_COLDBOOT);

        // Assert
        verify(mListener, times(1)).onAppStart();
        verify(mListener, never()).onAppRestart();
        verify(mListener, never()).onAppResume();
        verify(mListener, never()).onAppStop();
    }

    @Test
    public void notifyListener_AppStop() throws RemoteException {
        // Act
        uut.notifyAllListeners(EVpsState.APP_STOP);

        // Assert
        verify(mListener, times(1)).onAppStop();
        verify(mListener, never()).onAppStart();
        verify(mListener, never()).onAppRestart();
        verify(mListener, never()).onAppResume();
    }

    @Test
    public void notifyListener_AppResume() throws RemoteException {
        // Act
        uut.notifyAllListeners(EVpsState.APP_RESUME);

        // Assert
        verify(mListener, times(1)).onAppResume();
        verify(mListener, never()).onAppStart();
        verify(mListener, never()).onAppRestart();
        verify(mListener, never()).onAppStop();
    }

    @Test
    public void notifyListener_AppRestart() throws RemoteException {
        // Act
        uut.notifyAllListeners(EVpsState.APP_RESTART);

        // Assert
        verify(mListener, times(1)).onAppRestart();
        verify(mListener, never()).onAppStart();
        verify(mListener, never()).onAppResume();
        verify(mListener, never()).onAppStop();
    }

    @Test
    public void notifyListener_AppStartNa() throws RemoteException {
        // Act
        uut.notifyAllListeners(EVpsState.APP_START_NA);

        // Assert
        verify(mListener, never()).onAppStart();
        verify(mListener, never()).onAppRestart();
        verify(mListener, never()).onAppResume();
        verify(mListener, never()).onAppStop();
    }

    @Test(expected = IllegalArgumentException.class)
    public void notifyListenerStateIsNull_IllegalArgumentException() {
        uut.notifyListener(null, null);
        verifyZeroInteractions(mListener);
        verifyZeroInteractions(mStateHolder);
        verifyZeroInteractions(mSubscriptionNotification);
    }

    @Test(expected = IllegalArgumentException.class)
    public void notifyListenerListenerIsNull_IllegalArgumentException() {
        uut.notifyListener(EVpsState.APP_START_NA, null);
        verifyZeroInteractions(mListener);
        verifyZeroInteractions(mStateHolder);
        verifyZeroInteractions(mSubscriptionNotification);
    }

    @Test
    public void subscribeCatchIllegalArgumentException() {
        // Before
        String pid = String.valueOf(ThreadLocalRandom.current().nextInt());
        when(mStateHolder.getAppState()).thenReturn(null);

        // Act
        uut.subscribe(mock(IVehiclePowerServiceListener.class), pid);
        verifyZeroInteractions(mListener);
    }

    @Test
    public void notifyListener_RemoteException() throws RemoteException {
        // For Logdog, exception can't be without message
        // doThrow(new Exception("message") don't work
        Exception e = mock(RemoteException.class);
        when(e.getMessage()).thenReturn("UNIT_TEST");
        doThrow(e).when(mListener).onAppRestart();
        // Act
        uut.notifyAllListeners(EVpsState.APP_RESTART);

        // Assert
        verify(mListener, times(1)).onAppRestart();
    }

    @Test
    public void subscribe() {
        // Before
        String pid = String.valueOf(ThreadLocalRandom.current().nextInt());

        // Act
        uut.subscribe(mock(IVehiclePowerServiceListener.class), pid);

        // Assert
        verify(mSubscriptionNotification, times(1)).onSubscribe(pid);
    }

    @Test
    public void unsubscribe() {
        // Before
        String pid = String.valueOf(ThreadLocalRandom.current().nextInt());

        // Act
        uut.unsubscribe(mock(IVehiclePowerServiceListener.class), pid);

        // Assert
        verify(mSubscriptionNotification, times(1)).onUnsubscribe(pid);
    }
}
