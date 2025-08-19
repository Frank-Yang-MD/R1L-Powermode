package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import static org.powermock.api.mockito.PowerMockito.whenNew;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Set;
import org.json.JSONException;
import org.json.JSONObject;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;
import org.powermock.reflect.Whitebox;

@RunWith(PowerMockRunner.class)
@PrepareForTest({JSONObject.class, JsonCommandParser.class})
@SuppressStaticInitializationFor("com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog")
public class JsonCommandParserTest {
    @Mock private JSONObject mJsonObjectMock;

    private HashMap<String, String> mJsonKeys;

    private static final String COMMAND_IDENTIFIER = "command";
    private static final String NOT_COMMAND_IDENTIFIER = "not_command";
    private static final String START_COMMAND_NAME = "start";
    private static final String COMMAND_TEXT = "command_text";

    private JsonCommandParser mJsonCommandParser;

    @Before
    public void setUp() throws Exception {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));

        mJsonCommandParser = new JsonCommandParser();

        whenNew(JSONObject.class).withAnyArguments().thenReturn(mJsonObjectMock);

        mJsonKeys = new HashMap<>();
        mJsonKeys.put(COMMAND_IDENTIFIER, START_COMMAND_NAME);
        mJsonKeys.put(NOT_COMMAND_IDENTIFIER, "wait_timeout");
        when(mJsonObjectMock.keys()).thenReturn(mJsonKeys.keySet().iterator());
    }

    @Test
    public void parseSuccess() throws Exception {

        when(mJsonObjectMock.getString(COMMAND_IDENTIFIER)).thenReturn(START_COMMAND_NAME);

        assertTrue(mJsonCommandParser.parse(COMMAND_TEXT));
        verify(mJsonObjectMock).keys();
        verify(mJsonObjectMock).getString(COMMAND_IDENTIFIER);
    }

    @Test
    public void parseWithoutCommandIdentifier() throws Exception {
        mJsonKeys = new HashMap<>();
        mJsonKeys.put(NOT_COMMAND_IDENTIFIER, "wait_timeout");
        mJsonKeys.put(NOT_COMMAND_IDENTIFIER, "wait_timeout");
        when(mJsonObjectMock.keys()).thenReturn(mJsonKeys.keySet().iterator());

        assertFalse(mJsonCommandParser.parse(COMMAND_TEXT));
        verify(mJsonObjectMock).keys();
        verify(mJsonObjectMock, times(0)).getString(COMMAND_IDENTIFIER);
    }

    @Test
    public void parseWithJsonException() throws Exception {
        when(mJsonObjectMock.getString(anyString())).thenThrow(new JSONException("test_exception"));

        assertFalse(mJsonCommandParser.parse(COMMAND_TEXT));
    }

    @Test
    public void argumentNamesTest() {
        Set<String> keys = new HashSet<>();
        keys.add("key1");
        Whitebox.setInternalState(mJsonCommandParser, "mCommandKeys", keys);

        assertEquals(keys, mJsonCommandParser.argumentNames());
    }

    @Test
    public void argumentValueSuccess() throws Exception {
        final String argValue = "arg_value";
        Whitebox.setInternalState(mJsonCommandParser, "mJsonCommand", mJsonObjectMock);
        when(mJsonObjectMock.getString(argValue)).thenReturn(START_COMMAND_NAME);

        assertEquals(START_COMMAND_NAME, mJsonCommandParser.argumentValue(argValue));
        verify(mJsonObjectMock).getString(argValue);
    }

    @Test
    public void argumentValueWithException() throws Exception {
        final String argValue = "arg_value";
        Whitebox.setInternalState(mJsonCommandParser, "mJsonCommand", mJsonObjectMock);
        when(mJsonObjectMock.getString(argValue)).thenThrow(new JSONException("test_exception"));

        assertEquals("", mJsonCommandParser.argumentValue(argValue));
        verify(mJsonObjectMock).getString(argValue);
    }

    @Test
    public void commandNameTest() {
        Whitebox.setInternalState(mJsonCommandParser, "mCommandName", START_COMMAND_NAME);

        assertEquals(START_COMMAND_NAME, mJsonCommandParser.commandName());
    }
}
