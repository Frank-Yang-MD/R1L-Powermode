package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.lib.common.Const.ESM_SERVICE;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyZeroInteractions;
import static org.mockito.Mockito.when;
import static org.powermock.api.mockito.PowerMockito.whenNew;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.IBinder;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor({"com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog"})
@PrepareForTest({Intent.class, StartCommand.class, VPSLogMsg.class, MLog.class, Bundle.class})
public class StartCommandTest {
    @Mock Context mContextMock;

    @Mock IBinder mEsmBinderMock;

    @Mock ComponentName mComponentNameMock;

    @Mock IStartupInfo mStartupInfoMock;

    @Mock Bundle mBundleMock;

    @Mock Intent mIntentMock;

    StartCommand mStartCommand;

    static final String COMPONENT_CLASS_NAME = "component_name";

    @Before
    public void setUp() throws Exception {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
        when(mComponentNameMock.flattenToString()).thenReturn(COMPONENT_CLASS_NAME);

        mStartCommand =
                new StartCommand(
                        mContextMock, mEsmBinderMock,
                        mComponentNameMock, mStartupInfoMock);

        whenNew(Intent.class).withNoArguments().thenReturn(mIntentMock);
        whenNew(Bundle.class).withNoArguments().thenReturn(mBundleMock);

        when(mComponentNameMock.getClassName()).thenReturn(COMPONENT_CLASS_NAME);
        when(mContextMock.startService(mIntentMock)).thenReturn(mComponentNameMock);
    }

    @Test
    public void successfulExecuteWithoutAction() {
        mStartCommand.execute();

        verify(mBundleMock).putBinder(ESM_SERVICE, mEsmBinderMock);
        verify(mIntentMock).setComponent(mComponentNameMock);
        verify(mIntentMock).putExtras(mBundleMock);
        verify(mIntentMock, times(0)).setAction(anyString());
        verify(mContextMock).startService(mIntentMock);
        verify(mStartupInfoMock).onServiceStarted(COMPONENT_CLASS_NAME);
    }

    @Test
    public void successfulExecuteWithAction() {
        final String actionName = "action";
        mStartCommand.setAction(actionName);
        mStartCommand.execute();

        verify(mBundleMock).putBinder(ESM_SERVICE, mEsmBinderMock);
        verify(mIntentMock).setComponent(mComponentNameMock);
        verify(mIntentMock).putExtras(mBundleMock);
        verify(mIntentMock).setAction(actionName);
        verify(mContextMock).startService(mIntentMock);
        verify(mStartupInfoMock).onServiceStarted(COMPONENT_CLASS_NAME);
    }

    @Test
    public void successfulExecuteWithNullCommandName() {
        when(mContextMock.startService(mIntentMock)).thenReturn(null);

        mStartCommand.execute();

        verify(mBundleMock).putBinder(ESM_SERVICE, mEsmBinderMock);
        verify(mIntentMock).setComponent(mComponentNameMock);
        verify(mIntentMock).putExtras(mBundleMock);
        verify(mContextMock).startService(mIntentMock);
        verifyZeroInteractions(mStartupInfoMock);
    }

    @Test
    public void successfulExecuteWithTimeout() {
        final int TIMEOUT_MS = 1;
        mStartCommand.enableWaitForStartComplete(true, TIMEOUT_MS);
        mStartCommand.execute();

        verify(mBundleMock).putBinder(ESM_SERVICE, mEsmBinderMock);
        verify(mIntentMock).setComponent(mComponentNameMock);
        verify(mIntentMock).putExtras(mBundleMock);
        verify(mContextMock).startService(mIntentMock);
        verify(mStartupInfoMock).onServiceStarted(COMPONENT_CLASS_NAME);
        verify(mStartupInfoMock).waitForStartupComplete(COMPONENT_CLASS_NAME, TIMEOUT_MS);
    }

    @Test
    public void exceptionOnExecuteMethod() {
        when(mContextMock.startService(mIntentMock))
                .thenThrow(new IllegalStateException("test_exception"));

        mStartCommand.execute();

        verify(mBundleMock).putBinder(ESM_SERVICE, mEsmBinderMock);
        verify(mIntentMock).setComponent(mComponentNameMock);
        verify(mIntentMock).putExtras(mBundleMock);
        verify(mContextMock).startService(mIntentMock);
        verifyZeroInteractions(mStartupInfoMock);
    }
}
