/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.extsrvmanagerrunner;

import static org.hamcrest.CoreMatchers.instanceOf;
import static org.junit.Assert.assertThat;
import static org.mockito.ArgumentMatchers.*;
import static org.mockito.Mockito.*;

import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
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
public class ExtSrvManagerRunnerTest {
    @Mock private IExtSrvMgrRunNotification mIExtSrvMgrRunNotificationMock;

    @Mock private Context mContextMock;

    private ExtSrvManagerRunner mExtSrvManagerRunner;

    @Before
    public void initializeTest() {
        mExtSrvManagerRunner =
                new ExtSrvManagerRunner(mIExtSrvMgrRunNotificationMock, mContextMock);
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
    }

    /** Verifies that the instance of ExtSrvManagerRunner can be created. */
    @Test
    public void createInstance() {
        Assert.assertNotNull(mExtSrvManagerRunner);
    }

    /**
     * Verifies that the function bindService() of the interface Context is being called from the
     * ExtSrvManagerRunner.
     */
    @Test
    public void runMustCallbindServiceOfContext() {
        mExtSrvManagerRunner.run();
        verify(mContextMock, times(1))
                .bindService(any(Intent.class), any(ServiceConnection.class), anyInt());
    }

    /**
     * Verifies that the function startService() of the interface Context is being called from the
     * ExtSrvManagerRunner.
     */
    @Test
    public void runMustCallstartServiceofContext() {
        mExtSrvManagerRunner.run();
        verify(mContextMock, times(1)).startService(any(Intent.class));
    }

    /** Verifies that the function run() processes the exception. */
    @Test
    public void runMustProcessException() {
        SecurityException securityException = new SecurityException();
        when(mContextMock.startService(any(Intent.class))).thenThrow(securityException);

        // Action
        mExtSrvManagerRunner.run();

        // Verification
        assertThat(mExtSrvManagerRunner, instanceOf(ExtSrvManagerRunner.class));
    }
}
