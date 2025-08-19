/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer;

import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;
import static org.powermock.api.support.membermodification.MemberMatcher.method;
import static org.powermock.api.support.membermodification.MemberModifier.suppress;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EServiceState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;
import java.util.Map;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;
import org.powermock.reflect.Whitebox;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor({"com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog"})
public class StartupInfoContainerTest {
    private final String mClassName = "ClassName";

    private StartupInfoContainer mStartupInfoContainer;

    @Mock private IVpdNativeAdapter mVpdNativeAdapterMock;

    @Mock private Map mMapMock;

    @Mock private StartupInfoEntry mStartupInfoEntryMock;

    @Before
    public void setUp() {
        suppress(method(MLog.class, "i"));
        suppress(method(MLog.class, "w"));

        when(mMapMock.get(mClassName)).thenReturn(mStartupInfoEntryMock);

        mStartupInfoContainer = new StartupInfoContainer();
        Whitebox.setInternalState(mStartupInfoContainer, "configContainerByClass", mMapMock);
    }

    @Test
    public void waitForStartCompleteWithPositiveTimeout() {
        final int timeout = 1;
        mStartupInfoContainer.waitForStartupComplete(mClassName, timeout);

        verify(mStartupInfoEntryMock).waitStartComplete(timeout);
        verifyNoMoreInteractions(mStartupInfoEntryMock);
    }

    @Test
    public void waitForStartCompleteWithZeroTimeout() {
        final int timeout = 0;
        mStartupInfoContainer.waitForStartupComplete(mClassName, timeout);

        verify(mStartupInfoEntryMock).waitStartComplete();
        verifyNoMoreInteractions(mStartupInfoEntryMock);
    }

    @Test
    public void waitForStartCompleteWithNegativeTimeout() {
        final int timeout = -1;
        mStartupInfoContainer.waitForStartupComplete(mClassName, timeout);

        verify(mStartupInfoEntryMock).waitStartComplete();
        verifyNoMoreInteractions(mStartupInfoEntryMock);
    }

    @Test
    public void waitForStartCompleteWithNullStartupInfoEntry() {
        when(mMapMock.get(mClassName)).thenReturn(null);

        final int timeout = 1;
        mStartupInfoContainer.waitForStartupComplete(mClassName, timeout);

        verifyNoMoreInteractions(mStartupInfoEntryMock);
    }

    @Test
    public void serviceInfo() {
        StartupInfoContainer container = new StartupInfoContainer();
        String testString = "test";
        ServiceInfo testServiceInfo =
                new ServiceInfo(
                        "klass", "package", "esmName", "aktion", true, EServiceType.NORMAL_EFW);
        ServiceInfo testServiceInfo2 =
                new ServiceInfo(
                        "klass2", "package2", "esmName", "aktion2", false, EServiceType.NORMAL_EFW);
        container.add(testString, testServiceInfo);
        container.add(testString, testServiceInfo2);
        ServiceInfo result = container.getServiceInfo(testString);
        Assert.assertEquals(testServiceInfo.getAction(), result.getAction());
        Assert.assertEquals(testServiceInfo.getClassName(), result.getClassName());
        Assert.assertEquals(testServiceInfo.getPackageName(), result.getPackageName());
        Assert.assertEquals(testServiceInfo.isWaitNeeded(), result.isWaitNeeded());
        Assert.assertEquals(
                testServiceInfo.getEsmRegistrationName(), result.getEsmRegistrationName());
    }

    @Test(expected = IllegalArgumentException.class)
    public void serviceInfoDouble() {
        StartupInfoContainer container = new StartupInfoContainer();
        String testString = "test";
        ServiceInfo testServiceInfo =
                new ServiceInfo(
                        "klass0", "package", "esmName", "aktion", true, EServiceType.NORMAL_EFW);
        String testString2 = "test2";
        ServiceInfo testServiceInfo2 =
                new ServiceInfo(
                        "klass2", "package", "esmName", "aktion", true, EServiceType.NORMAL_EFW);
        container.add(testString, testServiceInfo);
        container.add(testString2, testServiceInfo2);
        container.getServiceInfo("test3");
    }

    @Test
    public void serviceStarted() {
        StartupInfoContainer container = new StartupInfoContainer();
        String testString1 = "test1";
        ServiceInfo testServiceInfo1 =
                new ServiceInfo(
                        "klass1", "package1", "esmName", "aktion1", true, EServiceType.NORMAL_EFW);
        String testString2 = "test3";
        ServiceInfo testServiceInfo2 =
                new ServiceInfo(
                        "klass3", "package2", "esmName", "aktion2", false, EServiceType.NORMAL_EFW);
        container.add(testString1, testServiceInfo1);
        container.add(testString2, testServiceInfo2);
        container.onServiceStarted(testServiceInfo2.getClassName());
        container.onServiceStarted("not exists");
        Assert.assertEquals(container.isServiceStarted(testServiceInfo1.getClassName()), false);
        Assert.assertEquals(container.isServiceStarted(testServiceInfo2.getClassName()), true);
        Assert.assertEquals(container.getAllStartedServicesList().size(), 2);
    }

    @Test
    public void serviceStartedNull() {
        StartupInfoContainer container = new StartupInfoContainer();
        Assert.assertEquals(container.isServiceStarted("not exists"), false);
    }

    @Test
    public void use() {
        StartupInfoContainer container = new StartupInfoContainer();
        Assert.assertNotNull(container.use(mVpdNativeAdapterMock));
    }

    @Test
    public void onCompleteEvents() {
        StartupInfoContainer container = new StartupInfoContainer();
        String testString1 = "test3";
        ServiceInfo testServiceInfo1 =
                new ServiceInfo(
                        "klass3", "package2", "esmName", "aktion2", false, EServiceType.NORMAL_EFW);
        container.add(testString1, testServiceInfo1);
        Assert.assertEquals(container.getServiceState("klass3"), EServiceState.NOT_STARTED);
        container.onServiceStartupCompleted("klass3");
        container.onServiceStartupCompleted("not exists");
        Assert.assertEquals(container.getServiceState("klass3"), EServiceState.STARTED);
        container.onStopComplete("klass3");
        container.onStopComplete("not exists");
        Assert.assertEquals(container.getServiceState("klass3"), EServiceState.STOPPED);
        container.onRestartResumeComplete("klass3");
        container.onRestartResumeComplete("not exists");
        Assert.assertEquals(container.getServiceState("klass3"), EServiceState.STARTED);
    }

    @Test
    public void getServiceStateNotExists() {
        StartupInfoContainer container = new StartupInfoContainer();
        Assert.assertEquals(container.getServiceState("klass5"), EServiceState.NOT_STARTED);
    }

    @Test
    public void startupInfoContainerState() {
        StartupInfoContainer container = new StartupInfoContainer();
        Assert.assertEquals(container.isAllAppsStarted(), false);
        container.setAllAppsStarted(true);
        Assert.assertEquals(container.isAllAppsStarted(), true);
        Assert.assertEquals(container.isAllServicesStarted(), false);
        container.setAllServicesStarted(true);
        Assert.assertEquals(container.isAllServicesStarted(), true);
        Assert.assertEquals(container.isEarlyAppsStarted(), false);
        container.setEarlyAppsStarted(true);
        Assert.assertEquals(container.isEarlyAppsStarted(), true);
    }

    @Test
    public void startupInfoContainerStartedState() {
        StartupInfoContainer container = new StartupInfoContainer();
        String testString1 = "test1";
        ServiceInfo testServiceInfo1 =
                new ServiceInfo(
                        "klass1", "package1", "esmName", "aktion1", true, EServiceType.NORMAL_EFW);
        String testString2 = "test2";
        ServiceInfo testServiceInfo2 =
                new ServiceInfo(
                        "klass3", "package2", "esmName", "aktion2", false, EServiceType.NORMAL_EFW);
        container.add(testString1, testServiceInfo1);
        container.add(testString2, testServiceInfo2);
        Assert.assertEquals(container.isNormalAppServicesStartComplete(), true);
        Assert.assertEquals(container.isLateAppServicesStartComplete(), true);
        Assert.assertEquals(container.isLateEfwServicesStartComplete(), true);
        Assert.assertEquals(container.isNormalEfwServicesStartComplete(), false);
        container.onServiceStartupCompleted("klass1");
        container.onServiceStartupCompleted("klass3");
        ;
        Assert.assertEquals(container.isNormalEfwServicesStartComplete(), true);
    }

    @Test
    public void startupInfoContainerGetShortClassName() {
        StartupInfoContainer container = new StartupInfoContainer();
        String testString1 = "test1";
        ServiceInfo testServiceInfo1 =
                new ServiceInfo(
                        "klass1", "package1", "esmName", "aktion1", true, EServiceType.NORMAL_EFW);
        String testString2 = "test2";
        ServiceInfo testServiceInfo2 =
                new ServiceInfo(
                        "klass3", "package2", "esmName", "aktion2", false, EServiceType.NORMAL_EFW);
        container.add(testString1, testServiceInfo1);
        container.add(testString2, testServiceInfo2);
        Assert.assertEquals(container.getShortClassName("klass1"), testString1);
        Assert.assertEquals(container.getShortClassName("klass3"), testString2);
        Assert.assertEquals(container.getShortClassName("klass2"), "klass2");
    }
}
