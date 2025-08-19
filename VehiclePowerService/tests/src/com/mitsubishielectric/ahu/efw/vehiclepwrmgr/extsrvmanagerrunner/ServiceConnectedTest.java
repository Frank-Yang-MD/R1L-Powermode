/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.extsrvmanagerrunner;

import static org.mockito.Mockito.*;

import android.content.ComponentName;
import android.os.IBinder;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import org.junit.Assert;
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
public class ServiceConnectedTest {
    @Mock IExtSrvMgrRunNotification mIExtSrvMgrRunNotificationMock;

    @Mock ComponentName mComponentNameMock;

    @Mock IBinder mIBinderMock;

    ServiceConnected mServiceConnected;

    @Before
    public void initializeTest() {
        mServiceConnected = new ServiceConnected(mIExtSrvMgrRunNotificationMock);
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
    }

    /** Verifies that the instance of ServiceConnected can be created. */
    @Test
    public void createInstanceTest() {
        mServiceConnected.onServiceDisconnected(mComponentNameMock);
        Assert.assertNotNull(mServiceConnected);
    }

    /**
     * Verifies that the function onServiceConnected() calls the function notifyExtSrvMgrBound() of
     * the interface IExtSrvMgrRunNotification.
     */
    @Test
    public void onServiceConnectedMustCallnotifyExtSrvMgrBound() {
        // Arrangements
        String pkg = "package.name";
        String cls = "class";
        ComponentName component = new ComponentName(pkg, cls);

        // Action
        mServiceConnected.onServiceConnected(component, mIBinderMock);

        // Verification
        verify(mIExtSrvMgrRunNotificationMock, times(1)).notifyExtSrvMgrBound(mIBinderMock);
    }

    /**
     * Verifies that the function onServiceConnected() does not call the function
     * notifyExtSrvMgrBound() of the interface IExtSrvMgrRunNotification, when the notification has
     * been already sent.
     */
    @Test
    public void onServiceConnectedDoesNotCallnotifyExtSrvMgrBound() {
        // Arrangements
        String pkg = "package.name";
        String cls = "class";
        ComponentName component = new ComponentName(pkg, cls);

        // Action
        mServiceConnected.onServiceConnected(component, mIBinderMock);
        mServiceConnected.onServiceConnected(component, mIBinderMock);

        // Verification
        verify(mIExtSrvMgrRunNotificationMock, times(1)).notifyExtSrvMgrBound(mIBinderMock);
    }
}
