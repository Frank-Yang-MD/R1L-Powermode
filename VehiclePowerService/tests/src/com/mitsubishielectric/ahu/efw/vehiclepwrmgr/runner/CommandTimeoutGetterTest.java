/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static org.junit.Assert.assertEquals;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import java.util.HashSet;
import java.util.Set;
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
public class CommandTimeoutGetterTest {
    @Mock private ICommandParser mCommandParser;

    private static final String SERVICE_LIST_PARAM = "service_list";
    private static final String TIMEOUT_PARAM = "timeout";
    private static final int TIMEOUT_MS = 5000;
    private static final int ZERO_TIMEOUT_MS = 0;

    @Before
    public void setUp() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
    }

    /**
     * Testcase for null pointers as a parameters. Should any of them be null -- we should get
     * default value (0 ms).
     */
    @Test
    public void testNullParameters() {
        final int timeout = CommandTimeoutGetter.getWaitForStartCompleteTimeoutMs(null, null);
        assertEquals(ZERO_TIMEOUT_MS, timeout);
    }

    /** If specified service can't be found we shouldn't crash, but get default value (0 ms). */
    @Test
    public void testServiceDoesNotExist() {
        setupService(ZERO_TIMEOUT_MS);
        final int timeout =
                CommandTimeoutGetter.getWaitForStartCompleteTimeoutMs(
                        mCommandParser, "unknown_service");
        assertEquals(ZERO_TIMEOUT_MS, timeout);
    }

    @Test
    public void testNullCommandParser() {
        assertEquals(
                ZERO_TIMEOUT_MS,
                CommandTimeoutGetter.getWaitForStartCompleteTimeoutMs(null, "service"));
    }

    @Test
    public void testNullParamName() {
        assertEquals(
                ZERO_TIMEOUT_MS,
                CommandTimeoutGetter.getWaitForStartCompleteTimeoutMs(mCommandParser, null));
    }

    @Test
    public void testNullTimeout() {
        setupService(TIMEOUT_MS);

        Mockito.when(mCommandParser.argumentValue(TIMEOUT_PARAM)).thenReturn(null);

        assertEquals(
                ZERO_TIMEOUT_MS,
                CommandTimeoutGetter.getWaitForStartCompleteTimeoutMs(
                        mCommandParser, SERVICE_LIST_PARAM));
    }

    @Test
    public void testEmptyTimeoutValue() {
        setupService(TIMEOUT_MS);

        Mockito.when(mCommandParser.argumentValue(TIMEOUT_PARAM)).thenReturn("");

        assertEquals(
                ZERO_TIMEOUT_MS,
                CommandTimeoutGetter.getWaitForStartCompleteTimeoutMs(
                        mCommandParser, SERVICE_LIST_PARAM));
    }

    @Test
    public void testTimeoutNotMatchPattern() {
        setupService(TIMEOUT_MS);

        Mockito.when(mCommandParser.argumentValue(TIMEOUT_PARAM)).thenReturn("01.01mm");

        assertEquals(
                ZERO_TIMEOUT_MS,
                CommandTimeoutGetter.getWaitForStartCompleteTimeoutMs(
                        mCommandParser, SERVICE_LIST_PARAM));
    }

    /** Testcase for 'perfect condition'. Should get desired timeout as a result. */
    @Test
    public void testTimeoutValue() {
        setupService(TIMEOUT_MS);
        final int timeout =
                CommandTimeoutGetter.getWaitForStartCompleteTimeoutMs(
                        mCommandParser, SERVICE_LIST_PARAM);
        assertEquals(TIMEOUT_MS, timeout);
    }

    /**
     * Small helper for initialization
     *
     * @param timeout desired timeout
     */
    private void setupService(int timeout) {
        final String TEST_SERVICE_NAME = "TestService";
        Set<String> set = new HashSet<>();
        set.add(TIMEOUT_PARAM);

        Mockito.when(mCommandParser.argumentValue(SERVICE_LIST_PARAM))
                .thenReturn(TEST_SERVICE_NAME);
        Mockito.when(mCommandParser.argumentValue(TIMEOUT_PARAM))
                .thenReturn(String.valueOf(timeout));
        Mockito.when(mCommandParser.argumentNames()).thenReturn(set);
    }
}
