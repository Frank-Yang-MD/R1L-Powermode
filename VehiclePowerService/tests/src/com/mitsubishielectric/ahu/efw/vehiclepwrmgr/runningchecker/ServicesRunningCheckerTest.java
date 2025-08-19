/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runningchecker;

import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.anyString;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.app.ActivityManager;
import android.content.ComponentName;
import android.content.Context;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.EServiceType;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IServiceListProvider;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.ServiceInfo;
import java.util.ArrayList;
import java.util.List;
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
public class ServicesRunningCheckerTest {
    @Mock private IServiceListProvider mIServiceListProviderMock;

    @Mock private Context mContextMock;

    @Mock private ActivityManager mActivityManagerMock;

    private ServicesRunningChecker mServicesRunningChecker;

    @Before
    public void initializeTest() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "e"));
        mServicesRunningChecker =
                new ServicesRunningChecker(mIServiceListProviderMock, mContextMock);
    }

    /** Verifies that the instance of ServicesRunningChecker can be created. */
    @Test
    public void createInstance() {
        Assert.assertNotNull(mServicesRunningChecker);
    }

    /**
     * Verifies that the function isAllServicesRunning() calls the function
     * getAllStartedServicesList() of the interface IServiceListProvider.
     */
    @Test(expected = NullPointerException.class)
    public void isAllServicesRunningMustCallgetAllStartedServicesList() {
        mServicesRunningChecker.isAllServicesRunning();
        verify(mIServiceListProviderMock, times(1)).getAllStartedServicesList();
    }

    /**
     * Verifies that the function isAllServicesRunning() calls the function getSystemService() of
     * Context.
     */
    @Test(expected = NullPointerException.class)
    public void isAllServicesRunningMustCallgetSystemService() {
        mServicesRunningChecker.isAllServicesRunning();
        verify(mContextMock, times(1)).getSystemService(anyString());
    }

    /**
     * Verifies that the function isAllServicesRunning() calls the function getRunningServices() of
     * the interface ActivityManager.
     */
    @Test
    public void isAllServicesRunningMustCallgetRunningServices() {
        when(mContextMock.getSystemService(anyString())).thenReturn(mActivityManagerMock);
        mServicesRunningChecker.isAllServicesRunning();
        verify(mActivityManagerMock, times(1)).getRunningServices(anyInt());
    }

    /**
     * Verifies that the function isAllServicesRunning() returns false when two lists of services
     * are not equal.
     */
    @Test
    public void isAllServicesRunningMustReturnFalse() {
        // Arrangements
        List<ServiceInfo> testList = new ArrayList<>();
        ServiceInfo info = new ServiceInfo("a", "b", "c", "d", true, EServiceType.NORMAL_EFW);
        testList.add(info);
        when(mIServiceListProviderMock.getAllStartedServicesList()).thenReturn(testList);
        when(mContextMock.getSystemService(anyString())).thenReturn(mActivityManagerMock);

        // Action
        boolean result = mServicesRunningChecker.isAllServicesRunning();

        // Verification
        Assert.assertFalse(result);
    }

    /**
     * Verifies that the function isAllServicesRunning() returns true when two lists of services are
     * equal.
     */
    @Test
    public void isAllServicesRunningMustReturnTrue() {
        // Arrangements
        ComponentName srv = new ComponentName("a", "b");
        List<ActivityManager.RunningServiceInfo> runSrvList = new ArrayList<>();
        ActivityManager.RunningServiceInfo infoRunSrv = new ActivityManager.RunningServiceInfo();
        infoRunSrv.service = srv;
        runSrvList.add(infoRunSrv);

        List<ServiceInfo> refSrvList = new ArrayList<>();
        ServiceInfo infoRefSrv =
                new ServiceInfo(
                        infoRunSrv.service.getClassName(),
                        infoRunSrv.service.getPackageName(),
                        "c",
                        "d",
                        true,
                        EServiceType.NORMAL_EFW);
        refSrvList.add(infoRefSrv);

        when(mIServiceListProviderMock.getAllStartedServicesList()).thenReturn(refSrvList);
        when(mContextMock.getSystemService(anyString())).thenReturn(mActivityManagerMock);
        when(mActivityManagerMock.getRunningServices(anyInt())).thenReturn(runSrvList);

        // Action
        boolean result = mServicesRunningChecker.isAllServicesRunning();

        // Verification
        Assert.assertTrue(result);
    }
}
