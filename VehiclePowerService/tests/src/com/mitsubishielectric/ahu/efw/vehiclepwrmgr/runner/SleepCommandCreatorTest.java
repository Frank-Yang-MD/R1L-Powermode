/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor("com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog")
public class SleepCommandCreatorTest {
    @Mock ICommandParser mICommandParser;
    private SleepCommandCreator commandCreator;

    @Before
    public void init() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
        commandCreator = new SleepCommandCreator(mICommandParser);
    }

    private final String OTHER_COMMAND = "START:serviceName";

    @Test
    public void isMatch() {
        Mockito.when(mICommandParser.parse("sleep")).thenReturn(true);
        Mockito.when(mICommandParser.commandName()).thenReturn("sleep");
        assertTrue(commandCreator.isMatch("sleep"));
    }

    @Test
    public void isNotMatch() {
        Mockito.when(mICommandParser.parse("fail")).thenReturn(false);
        assertFalse(commandCreator.isMatch("fail"));
    }

    @Test
    public void isNotMatchWithWrongCommand() {
        Mockito.when(mICommandParser.parse("fail")).thenReturn(true);
        Mockito.when(mICommandParser.commandName()).thenReturn("");

        assertFalse(commandCreator.isMatch("fail"));
    }

    @Test
    public void createSleepCommandFromInvalidValue() {
        Mockito.when(mICommandParser.parse("sleep")).thenReturn(true);
        Mockito.when(mICommandParser.commandName()).thenReturn("sleep");
        Mockito.when(mICommandParser.argumentValue("value")).thenReturn("-1.11ms");
        IScriptCommand command = commandCreator.create("sleep", 0);
        assertNotNull(command);
        assertEquals(InvalidFormatCommand.class, command.getClass());
    }

    @Test
    public void createCommandWithWrongFormatValue() {
        final String sleepCommand = "01,01";
        Mockito.when(mICommandParser.parse(sleepCommand)).thenReturn(true);
        Mockito.when(mICommandParser.commandName()).thenReturn("sleep");
        Mockito.when(mICommandParser.argumentValue("value")).thenReturn(sleepCommand);

        IScriptCommand command = commandCreator.create(sleepCommand, 0);
        assertNotNull(command);
        assertEquals(SleepCommand.class, command.getClass());
    }

    @Test
    public void createValidSleepCommand() {
        Mockito.when(mICommandParser.parse("sleep")).thenReturn(true);
        Mockito.when(mICommandParser.commandName()).thenReturn("sleep");
        Mockito.when(mICommandParser.argumentValue("value")).thenReturn("10.01ms");
        IScriptCommand command = commandCreator.create("sleep", 0);
        assertNotNull(command);
        assertEquals(SleepCommand.class, command.getClass());
    }

    @Test
    public void createSleepCommandFromInvalidText() {
        IScriptCommand command = commandCreator.create(OTHER_COMMAND, 0);
        assertEquals(InvalidFormatCommand.class, command.getClass());
    }
}
