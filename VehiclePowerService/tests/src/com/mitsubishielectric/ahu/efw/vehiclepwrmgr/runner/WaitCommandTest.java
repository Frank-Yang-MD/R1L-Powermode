/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.EServiceType;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IInfoContainer;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.ServiceInfo;
import java.util.Arrays;
import java.util.List;
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
public class WaitCommandTest {
    @Mock private IStartupInfo mStartupInfo;

    @Mock private IInfoContainer mInfoContainer;

    private List<String> mServiceList;

    @Before
    public void setUp() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "d"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
    }

    @Test
    public void execute() {
        mServiceList = Arrays.asList("ServiceA", "ServiceB", "ServiceC");

        // Arrange
        final int WAIT_INTERVAL_MS = 1000;
        when(mInfoContainer.getServiceInfo(any(String.class)))
                .thenReturn(
                        new ServiceInfo("ServiceInfo", "", "", "", true, EServiceType.NORMAL_EFW));
        IScriptCommand waitCommand =
                new WaitCommand(mServiceList, mStartupInfo, mInfoContainer, WAIT_INTERVAL_MS);
        // Act
        waitCommand.execute();

        // Let's check that getServiceInfo() is called for each of the list items
        verify(mInfoContainer, times(mServiceList.size())).getServiceInfo(any(String.class));
    }
}
