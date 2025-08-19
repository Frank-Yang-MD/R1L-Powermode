/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static org.junit.Assert.*;
import static org.mockito.Mockito.verifyZeroInteractions;

import android.content.Context;
import android.os.IBinder;
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
import org.mockito.Mock;
import org.mockito.Mockito;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@PrepareForTest({StartCommand.class, ServiceInfo.class, StartCommandCreator.class})
@SuppressStaticInitializationFor("com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog")
public class StartCommandCreatorTest {
    @Mock Context mContextMock;
    @Mock IBinder mIBinderMock;
    @Mock ICommandParser mICommandParserMock;
    @Mock IInfoContainer mIInfoContainerMock;
    @Mock IStartupInfo mIStartupInfoMock;

    ServiceInfo serviceInfoMock = PowerMockito.mock(ServiceInfo.class);

    Set<String> set = new HashSet<>();

    private final String START_COMMAND = "start";
    private final String WAIT_PARAM_NAME = "wait_startup_completion";

    private StartCommandCreator commandCreator;

    @Before
    public void init() throws Exception {
        PowerMockito.mockStatic(StartCommand.class);
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));

        commandCreator =
                new StartCommandCreator(
                        mContextMock,
                        mIBinderMock,
                        mICommandParserMock,
                        mIInfoContainerMock,
                        mIStartupInfoMock);

        set.add("service");

        Mockito.when(mICommandParserMock.parse(START_COMMAND)).thenReturn(true);
        Mockito.when(mICommandParserMock.commandName()).thenReturn(START_COMMAND);
        Mockito.when(mICommandParserMock.argumentValue("service")).thenReturn("");
        Mockito.when(mICommandParserMock.argumentValue(WAIT_PARAM_NAME)).thenReturn("false");
        Mockito.when(mICommandParserMock.argumentNames()).thenReturn(set);
    }

    @Test
    public void isMatch() {
        assertTrue(commandCreator.isMatch(START_COMMAND));
    }

    @Test
    public void isNotMatch() {
        Mockito.when(mICommandParserMock.parse("")).thenReturn(true);
        Mockito.when(mICommandParserMock.commandName()).thenReturn("");
        assertFalse(commandCreator.isMatch(""));
    }

    @Test
    public void isNotMatchWithNullCommand() {
        Mockito.when(mICommandParserMock.commandName()).thenReturn(null);
        assertFalse(commandCreator.isMatch(START_COMMAND));
    }

    @Test
    public void createRunServiceCommand() {
        ServiceInfo serviceInfo =
                new ServiceInfo(
                        "class", "package", "esmName", "action", false, EServiceType.NORMAL_EFW);

        Mockito.when(mIInfoContainerMock.getServiceInfo("")).thenReturn(serviceInfo);

        IScriptCommand command = commandCreator.create(START_COMMAND, 1);
        assertEquals(StartCommand.class, command.getClass());
    }

    @Test
    public void createCommandWithoutSetAction() {
        Mockito.when(mIInfoContainerMock.getServiceInfo("")).thenReturn(serviceInfoMock);
        Mockito.when(serviceInfoMock.getAction()).thenReturn(null);

        IScriptCommand command = commandCreator.create(START_COMMAND, 1);
        verifyZeroInteractions(mIStartupInfoMock);
    }

    @Test
    public void createCommandWithTimeout() {
        Mockito.when(mIInfoContainerMock.getServiceInfo("")).thenReturn(serviceInfoMock);
        Mockito.when(serviceInfoMock.getAction()).thenReturn("");

        set.add(WAIT_PARAM_NAME);
        commandCreator.create(START_COMMAND, 1);
    }

    @Test
    public void createInvalidCommand() {
        Set<String> set = new HashSet<>();
        set.add("service");
        Mockito.when(mICommandParserMock.parse("fail")).thenReturn(false);
        IScriptCommand command = commandCreator.create("fail", 1);

        assertNotNull(command);
        assertEquals(InvalidFormatCommand.class, command.getClass());
    }
}
