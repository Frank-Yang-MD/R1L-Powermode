/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static org.mockito.ArgumentMatchers.anyList;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import java.util.concurrent.ExecutorService;
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
public class JavaRunnerTest {
    @Mock private IScriptReader mScriptReader;

    @Mock private IScriptExecutor mScriptExecutor;

    @Mock private ExecutorService mExecutorService;

    @InjectMocks private JavaRunner javaRunner;

    @Before
    public void setUp() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
    }

    @Test
    public void start() {
        // Act
        javaRunner.start();

        // Assert
        verify(mExecutorService).execute(javaRunner);
    }

    @Test
    public void run() {
        // Act
        javaRunner.run();

        // Assert
        verify(mScriptReader, times(1)).read();
        verify(mScriptExecutor, times(1)).execute(anyList());
    }
}
