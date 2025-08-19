/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.EServiceType;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IInfoContainer;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.ServiceInfo;
import java.util.HashSet;
import java.util.Set;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor("com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog")
public class WaitCommandCreatorTest {
    @Mock private ICommandParser mCommandParser;
    @Mock private IInfoContainer mInfoContainer;
    @Mock private IStartupInfo mStartupInfo;

    @InjectMocks private WaitCommandCreator mCommandCreator;

    private Set<String> mSet;

    private static final String CMD_NAME = "wait_services_started";
    private static final String SERVICE_LIST_PARAM = "service_list";

    @Before
    public void setUp() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "d"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
        mCommandCreator = new WaitCommandCreator(mCommandParser, mStartupInfo, mInfoContainer);
        mSet = new HashSet<>();
        mSet.add(SERVICE_LIST_PARAM);
    }

    @Test
    public void isMatch() {
        when(mCommandParser.parse(CMD_NAME)).thenReturn(true);
        when(mCommandParser.commandName()).thenReturn(CMD_NAME);
        assertTrue(mCommandCreator.isMatch(CMD_NAME));
    }

    @Test
    public void isNotMatch() {
        when(mCommandParser.parse("")).thenReturn(true);
        when(mCommandParser.commandName()).thenReturn("");
        assertFalse(mCommandCreator.isMatch(""));
    }

    @Test
    public void createInvalidCommand() {
        when(mCommandParser.parse("fail")).thenReturn(false);
        IScriptCommand command = mCommandCreator.create("fail", 1);

        assertNotNull(command);
        assertEquals(InvalidFormatCommand.class, command.getClass());
    }

    @Test
    public void createCommandWithEmptyList() {
        when(mCommandParser.parse(CMD_NAME)).thenReturn(true).thenReturn(false);
        when(mCommandParser.commandName()).thenReturn(CMD_NAME);

        verify(mCommandParser, times(0)).argumentValue(anyString());
        IScriptCommand command = mCommandCreator.create(CMD_NAME, 0);
        assertEquals(WaitCommand.class, command.getClass());
    }

    @Test
    public void createValidCommand() {
        when(mCommandParser.parse(CMD_NAME)).thenReturn(true);
        when(mCommandParser.commandName()).thenReturn(CMD_NAME);
        when(mCommandParser.argumentValue(SERVICE_LIST_PARAM)).thenReturn("[TestService]");
        when(mCommandParser.argumentNames()).thenReturn(mSet);
        when(mInfoContainer.getServiceInfo(any(String.class)))
                .thenReturn(
                        new ServiceInfo("ServiceInfo", "", "", "", true, EServiceType.NORMAL_EFW));

        IScriptCommand command = mCommandCreator.create(CMD_NAME, 0);
        command.execute();

        assertNotNull(command);
        assertEquals(WaitCommand.class, command.getClass());
    }
}
