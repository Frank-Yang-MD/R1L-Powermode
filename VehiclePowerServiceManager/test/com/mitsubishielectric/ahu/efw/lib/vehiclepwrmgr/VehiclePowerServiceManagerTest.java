/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr;

import static org.junit.Assert.*;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.os.IBinder;
import android.os.RemoteException;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.modules.junit4.PowerMockRunner;
import org.powermock.reflect.Whitebox;

@RunWith(PowerMockRunner.class)
@PrepareForTest(IVehiclePowerService.Stub.class)
public class VehiclePowerServiceManagerTest {
    @Mock private IBinder mBinderMock;

    @Mock private IVehiclePowerService mIVehiclePowerServiceMock;

    @Mock private IVehiclePowerServiceListener mVehiclePowerServiceListenerMock;

    @Mock private IBootDaemonStatusListener mBootDaemonStatusListenerMock;

    private VehiclePowerServiceManager mVehiclePowerServiceManager;

    private static final String TEST_STRING = "TestClassString";

    @Before
    public void setUp() {
        PowerMockito.mockStatic(IVehiclePowerService.Stub.class);
        Whitebox.setInternalState(VehiclePowerServiceManager.class, "sInstance", (IBinder) null);
    }

    @Test(expected = IllegalStateException.class)
    public void privateConstructorBinderIsNullTest() {
        VehiclePowerServiceManager.setBinder(null);
        VehiclePowerServiceManager.getInstance();
    }

    @Test(expected = IllegalStateException.class)
    public void privateConstructorServiceIsNullTest() {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock)).thenReturn(null);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        VehiclePowerServiceManager.getInstance();
    }

    @Test
    public void testGetInstance() {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        assertNotNull(VehiclePowerServiceManager.getInstance());
    }

    @Test
    public void testGetInstanceReturnSavedInstance() {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        assertNotNull(VehiclePowerServiceManager.getInstance());
    }

    @Test
    public void subscribeApp() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        doNothing()
                .when(mIVehiclePowerServiceMock)
                .subscribeApp(mVehiclePowerServiceListenerMock, TEST_STRING);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        mVehiclePowerServiceManager.subscribeApp(mVehiclePowerServiceListenerMock, TEST_STRING);
        verify(mIVehiclePowerServiceMock)
                .subscribeApp(mVehiclePowerServiceListenerMock, TEST_STRING);
    }

    @Test
    public void subscribeFWService() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        doNothing()
                .when(mIVehiclePowerServiceMock)
                .subscribeFWService(mVehiclePowerServiceListenerMock, TEST_STRING);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        mVehiclePowerServiceManager.subscribeFWService(
                mVehiclePowerServiceListenerMock, TEST_STRING);
        verify(mIVehiclePowerServiceMock)
                .subscribeFWService(mVehiclePowerServiceListenerMock, TEST_STRING);
    }

    @Test
    public void unsubscribe() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        doNothing()
                .when(mIVehiclePowerServiceMock)
                .unsubscribe(mVehiclePowerServiceListenerMock, TEST_STRING);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        mVehiclePowerServiceManager.unsubscribe(mVehiclePowerServiceListenerMock, TEST_STRING);
        verify(mIVehiclePowerServiceMock)
                .unsubscribe(mVehiclePowerServiceListenerMock, TEST_STRING);
    }

    @Test
    public void stopCompleteApp() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        doNothing().when(mIVehiclePowerServiceMock).stopCompleteApp(TEST_STRING);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        mVehiclePowerServiceManager.stopCompleteApp(TEST_STRING);
        verify(mIVehiclePowerServiceMock).stopCompleteApp(TEST_STRING);
    }

    @Test
    public void stopCompleteEfw() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        doNothing().when(mIVehiclePowerServiceMock).stopCompleteEfw(TEST_STRING);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        mVehiclePowerServiceManager.stopCompleteEfw(TEST_STRING);
        verify(mIVehiclePowerServiceMock).stopCompleteEfw(TEST_STRING);
    }

    @Test
    public void getNotifiedEvent() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        when(mIVehiclePowerServiceMock.getNotifiedEvent()).thenReturn(EVpsState.APP_RESUME);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        assertEquals(EVpsState.APP_RESUME, mVehiclePowerServiceManager.getNotifiedEvent());
    }

    @Test
    public void isServiceStarted() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        when(mIVehiclePowerServiceMock.isServiceStarted(TEST_STRING)).thenReturn(true);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        assertTrue(mVehiclePowerServiceManager.isServiceStarted(TEST_STRING));
    }

    @Test
    public void startComplete() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        doNothing().when(mIVehiclePowerServiceMock).startComplete(TEST_STRING);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        mVehiclePowerServiceManager.startComplete(TEST_STRING);
        verify(mIVehiclePowerServiceMock).startComplete(TEST_STRING);
    }

    @Test
    public void isAllAppServicesStarted() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        when(mIVehiclePowerServiceMock.isAllAppServicesStarted()).thenReturn(true);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        assertTrue(mVehiclePowerServiceManager.isAllAppServicesStarted());
    }

    @Test
    public void isAllEfwServicesStarted() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        when(mIVehiclePowerServiceMock.isAllEfwServicesStarted()).thenReturn(true);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        assertTrue(mVehiclePowerServiceManager.isAllEfwServicesStarted());
    }

    @Test
    public void normalAppServicesStarted() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        when(mIVehiclePowerServiceMock.normalAppServicesStarted()).thenReturn(true);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        assertTrue(mVehiclePowerServiceManager.normalAppServicesStarted());
    }

    @Test
    public void printMarker() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        when(mIVehiclePowerServiceMock.printMarker(TEST_STRING)).thenReturn(1);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        assertEquals(1, mVehiclePowerServiceManager.printMarker(TEST_STRING));
    }

    @Test
    public void resumeCompleteEfw() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        doNothing().when(mIVehiclePowerServiceMock).resumeCompleteEfw(TEST_STRING);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        mVehiclePowerServiceManager.resumeCompleteEfw(TEST_STRING);
        verify(mIVehiclePowerServiceMock).resumeCompleteEfw(TEST_STRING);
    }

    @Test
    public void restartCompleteEfw() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        doNothing().when(mIVehiclePowerServiceMock).resumeCompleteEfw(TEST_STRING);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        mVehiclePowerServiceManager.restartCompleteEfw(TEST_STRING);
        verify(mIVehiclePowerServiceMock).restartCompleteEfw(TEST_STRING);
    }

    @Test
    public void resumeCompleteApp() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        doNothing().when(mIVehiclePowerServiceMock).resumeCompleteApp(TEST_STRING);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        mVehiclePowerServiceManager.resumeCompleteApp(TEST_STRING);
        verify(mIVehiclePowerServiceMock).resumeCompleteApp(TEST_STRING);
    }

    @Test
    public void restartCompleteApp() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        doNothing().when(mIVehiclePowerServiceMock).restartCompleteApp(TEST_STRING);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        mVehiclePowerServiceManager.restartCompleteApp(TEST_STRING);
        verify(mIVehiclePowerServiceMock).restartCompleteApp(TEST_STRING);
    }

    @Test
    public void getServiceState() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        when(mIVehiclePowerServiceMock.getServiceState(TEST_STRING))
                .thenReturn(EServiceState.STARTED);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        assertEquals(
                EServiceState.STARTED, mVehiclePowerServiceManager.getServiceState(TEST_STRING));
    }

    @Test
    public void isNormalAppServicesStartComplete() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        when(mIVehiclePowerServiceMock.isNormalAppServicesStartComplete()).thenReturn(true);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        assertTrue(mVehiclePowerServiceManager.isNormalAppServicesStartComplete());
    }

    @Test
    public void isNormalEfwServicesStartComplete() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        when(mIVehiclePowerServiceMock.isNormalEfwServicesStartComplete()).thenReturn(true);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        assertTrue(mVehiclePowerServiceManager.isNormalEfwServicesStartComplete());
    }

    @Test
    public void isLateEfwServicesStartComplete() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        when(mIVehiclePowerServiceMock.isLateEfwServicesStartComplete()).thenReturn(true);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        assertTrue(mVehiclePowerServiceManager.isLateEfwServicesStartComplete());
    }

    @Test
    public void isLateAppServicesStartComplete() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        when(mIVehiclePowerServiceMock.isLateAppServicesStartComplete()).thenReturn(true);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        assertTrue(mVehiclePowerServiceManager.isLateAppServicesStartComplete());
    }

    @Test
    public void ismReady() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        doNothing().when(mIVehiclePowerServiceMock).ismReady(mBootDaemonStatusListenerMock);
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        mVehiclePowerServiceManager.ismReady(mBootDaemonStatusListenerMock);
        verify(mIVehiclePowerServiceMock).ismReady(mBootDaemonStatusListenerMock);
    }

    @Test
    public void requestEndEarlyHmi() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        doNothing().when(mIVehiclePowerServiceMock).requestEndEarlyHmi();
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        mVehiclePowerServiceManager.requestEndEarlyHmi();
        verify(mIVehiclePowerServiceMock).requestEndEarlyHmi();
    }

    @Test
    public void forcedCompletionEarlyServices() throws RemoteException {
        when(IVehiclePowerService.Stub.asInterface(mBinderMock))
                .thenReturn(mIVehiclePowerServiceMock);
        doNothing().when(mIVehiclePowerServiceMock).forcedCompletionEarlyServices();
        VehiclePowerServiceManager.setBinder(mBinderMock);
        mVehiclePowerServiceManager = VehiclePowerServiceManager.getInstance();
        mVehiclePowerServiceManager.forcedCompletionEarlyServices();
        verify(mIVehiclePowerServiceMock).forcedCompletionEarlyServices();
    }
}
