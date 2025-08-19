/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.lib.common.Const.VEHICLE_CONFIG_READER_SERVICE;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.when;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.os.IBinder;
import com.mitsubishielectric.ahu.efw.lib.extendedservicemanager.ExtSrvManager;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.lib.vehicleconfigservice.VehicleConfigManager;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

/** VehicleConfigRunnerTest. */
@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor({"com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog"})
@PrepareForTest({VehicleConfigRunner.class, Thread.class})
public class VehicleConfigRunnerTest {
    @Mock Context mMockContext;

    @Mock IBinder mMockIBinder;

    @Mock ExtSrvManager mMockExtSrvManager;

    @Mock ComponentName mMockComponentName;

    VehicleConfigRunner mVehicleConfigRunner;

    @Before
    public void init() {
        PowerMockito.mockStatic(Thread.class);
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));

        when(mMockComponentName.getClassName()).thenReturn("Unit_test");
        mVehicleConfigRunner =
                new VehicleConfigRunner(mMockContext, mMockIBinder, mMockExtSrvManager);
    }

    @Test(expected = IllegalArgumentException.class)
    public void constructor1stArgExcept() {
        new VehicleConfigRunner(null, mMockIBinder, mMockExtSrvManager);
    }

    @Test(expected = IllegalArgumentException.class)
    public void constructor2stArgExcept() {
        new VehicleConfigRunner(mMockContext, null, mMockExtSrvManager);
    }

    @Test(expected = IllegalArgumentException.class)
    public void constructor3dArgExcept() {
        new VehicleConfigRunner(mMockContext, mMockIBinder, null);
    }

    @Test
    public void startServiceContextNull() {
        when(mMockContext.startService(any(Intent.class))).thenReturn(null);
        Assert.assertNull(mVehicleConfigRunner.getReadyVehicleConfigManager());
    }

    @Test
    public void startServiceContextSecurityException() {
        when(mMockContext.startService(any(Intent.class)))
                .thenThrow(new SecurityException("Unit_test"));
        Assert.assertNull(mVehicleConfigRunner.getReadyVehicleConfigManager());
    }

    @Test
    public void startServiceContextIllegalStateException() {
        when(mMockContext.startService(any(Intent.class)))
                .thenThrow(new IllegalStateException("Unit_test"));
        Assert.assertNull(mVehicleConfigRunner.getReadyVehicleConfigManager());
    }

    @Test
    public void getReadyVehicleConfigManagerIllegalArgumentException() {
        when(mMockContext.startService(any(Intent.class))).thenReturn(mMockComponentName);
        when(mMockExtSrvManager.getService(VEHICLE_CONFIG_READER_SERVICE))
                .thenThrow(new IllegalArgumentException("Unit_test"));
        Assert.assertNull(mVehicleConfigRunner.getReadyVehicleConfigManager());
    }

    @Test
    public void getReadyVehicleConfigManagerInterruptedException() {
        doAnswer(
                        i -> {
                            throw new InterruptedException("test_exception");
                        })
                .when(mMockExtSrvManager)
                .getService(VEHICLE_CONFIG_READER_SERVICE);
        when(mMockContext.startService(any(Intent.class))).thenReturn(mMockComponentName);

        Assert.assertNull(mVehicleConfigRunner.getReadyVehicleConfigManager());
    }

    @Test
    public void getReadyVehicleConfigManager() {
        when(mMockContext.startService(any(Intent.class))).thenReturn(mMockComponentName);
        when(mMockComponentName.getClassName()).thenReturn("Unit_test");
        when(mMockExtSrvManager.getService(VEHICLE_CONFIG_READER_SERVICE)).thenReturn(mMockIBinder);
        Assert.assertEquals(
                VehicleConfigManager.getInstance(),
                mVehicleConfigRunner.getReadyVehicleConfigManager());
    }

    @Test
    public void getReadyVehicleConfigManagerWithNullBinder() {
        when(mMockContext.startService(any(Intent.class))).thenReturn(mMockComponentName);
        when(mMockComponentName.getClassName()).thenReturn("Unit_test");
        when(mMockExtSrvManager.getService(VEHICLE_CONFIG_READER_SERVICE))
                .thenReturn(null)
                .thenReturn(mMockIBinder);
        Assert.assertEquals(
                VehicleConfigManager.getInstance(),
                mVehicleConfigRunner.getReadyVehicleConfigManager());
    }
}
