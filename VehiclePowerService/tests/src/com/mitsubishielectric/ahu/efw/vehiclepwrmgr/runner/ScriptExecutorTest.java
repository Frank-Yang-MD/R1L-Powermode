/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IEventReceiver;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
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
public class ScriptExecutorTest {
    private IScriptExecutor mScriptExecutor;

    @Mock private IScriptCommandFactory mMockFactory;

    @Mock private IScriptCommand mMockCommand;

    @Mock private IEventReceiver mEventReceiver;

    @Mock private IStartupInfo mIStartupInfo;

    @Before
    public void setup() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
        mScriptExecutor =
                new ScriptExecutor(
                        mMockFactory, mEventReceiver, IncomingEvents.LATE_APP_SERVICES_STARTED);
        when(mMockFactory.createCommand(anyString(), anyInt())).thenReturn(mMockCommand);
    }

    @Test
    public void execute() {
        // Arrange
        List<String> commands = Arrays.asList("cmd1", "cmd2", "cmd3");

        // Act
        mScriptExecutor.execute(commands);

        // Assert
        verify(mMockFactory, times(3)).createCommand(anyString(), anyInt());
        verify(mMockCommand, times(3)).execute();
    }

    @Test
    public void exectuteGotNullFromFactory() {
        when(mMockFactory.createCommand(anyString(), anyInt())).thenReturn(null);
        List<String> commands = Arrays.asList("cmd1");

        mScriptExecutor.execute(commands);

        verify(mMockFactory, times(1)).createCommand(anyString(), anyInt());
        verify(mMockCommand, times(0)).execute();
    }
}
