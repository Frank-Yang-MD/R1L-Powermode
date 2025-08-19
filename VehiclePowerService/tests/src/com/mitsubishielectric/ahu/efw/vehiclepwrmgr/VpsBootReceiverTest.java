/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor("com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog")
@PrepareForTest({VPSLogMsg.class})
public class VpsBootReceiverTest {
    @Mock private Intent mIntentMock;

    @Mock private Context mContextMock;

    @Mock private ComponentName mComponentNameMock;

    @InjectMocks private VpsBootReceiver mVpsBootReceiver;

    @Before
    public void setUp() {
        PowerMockito.mockStatic(VPSLogMsg.class);
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
        mVpsBootReceiver = new VpsBootReceiver();
        when(mIntentMock.getAction()).thenReturn("string");
    }

    @Test
    public void onReceive() {
        // Act
        mVpsBootReceiver.onReceive(mContextMock, mIntentMock);
        // Assert
        verify(mContextMock).startService(any());
    }

    @Test
    public void onReceiveNameIsNotNull() {
        when(mContextMock.startService(any())).thenReturn(mComponentNameMock);
        mVpsBootReceiver.onReceive(mContextMock, mIntentMock);
        verify(mContextMock).startService(any());
    }

    @Test
    public void onReceiveWithException() {
        // Arrange
        VpsBootReceiver vpsBootReceiver = new VpsBootReceiver();
        when(mContextMock.startService(any())).thenThrow(new IllegalStateException("ISE Error"));
        // Act
        vpsBootReceiver.onReceive(mContextMock, mIntentMock);
    }
}
