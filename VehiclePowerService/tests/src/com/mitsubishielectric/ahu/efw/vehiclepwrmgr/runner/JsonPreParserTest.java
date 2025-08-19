package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static org.junit.Assert.assertEquals;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyZeroInteractions;
import static org.mockito.Mockito.when;
import static org.powermock.api.mockito.PowerMockito.spy;
import static org.powermock.api.mockito.PowerMockito.whenNew;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.EServiceType;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IInfoContainer;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Set;
import org.json.JSONArray;
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
@PrepareForTest({JSONObject.class, JsonPreParser.class, JSONArray.class})
@SuppressStaticInitializationFor({"com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog"})
public class JsonPreParserTest {
    @Mock IInfoContainer mInfoContainerMock;

    @Mock JSONObject mJsonObjectMock;

    private HashMap<String, String> mJsonConfig;
    private List<String> expectedList;
    private final String START_SEQUENCE_KEY = "start_sequence";
    private final String SERVICE_NAME = "service_name : name";

    JSONArray mJsonArraySpy;
    JsonPreParser mJsonPreParser;

    @Before
    public void setUp() throws Exception {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));

        whenNew(JSONObject.class).withAnyArguments().thenReturn(mJsonObjectMock);
        mJsonArraySpy = spy(Whitebox.newInstance(JSONArray.class));

        mJsonConfig = new HashMap<>();
        expectedList = new ArrayList<>();

        mJsonPreParser = new JsonPreParser(mInfoContainerMock, EServiceType.LATE_APP);
    }

    @Test(expected = IllegalArgumentException.class)
    public void constructorWithNullContainer() {
        mJsonPreParser = new JsonPreParser(null, EServiceType.LATE_APP);
    }

    @Test
    public void getLinesFromStringWithStartSequence() throws Exception {
        mJsonConfig.put(START_SEQUENCE_KEY, SERVICE_NAME);
        when(mJsonObjectMock.keys()).thenReturn(mJsonConfig.keySet().iterator());

        when(mJsonObjectMock.get(START_SEQUENCE_KEY)).thenReturn(mJsonArraySpy);
        when(mJsonArraySpy.length()).thenReturn(1);
        when(mJsonArraySpy.getJSONObject(0)).thenReturn(mJsonObjectMock);
        when(mJsonObjectMock.toString()).thenReturn("expected_string");

        expectedList.add("expected_string");
        assertEquals(expectedList, mJsonPreParser.getLinesFromString("config"));
        verify(mJsonObjectMock).keys();
        verify(mJsonObjectMock).get(START_SEQUENCE_KEY);
        verify(mJsonArraySpy).getJSONObject(0);
        verifyZeroInteractions(mInfoContainerMock);
    }

    @Test
    public void getLinesFromStringWithOtherKey() throws JSONException {
        prepareConfig();

        JSONObject obj = spy(Whitebox.newInstance(JSONObject.class));

        when(mJsonObjectMock.keys()).thenReturn(mJsonConfig.keySet().iterator());
        when(mJsonObjectMock.get(anyString())).thenReturn(obj);
        when(obj.keys()).thenReturn(mJsonConfig.keySet().iterator());
        when(mJsonArraySpy.length()).thenReturn(0);

        assertEquals(Collections.emptyList(), mJsonPreParser.getLinesFromString("config"));
        verify(mJsonObjectMock).keys();

        for (String key : prepareStringKeysSet()) {
            verify(mJsonObjectMock).get(key);
            verify(obj).get(key);
        }
        verify(obj).getBoolean("wait");
        verify(obj, times(0)).get("unexpected");
    }

    @Test
    public void getLinesFromStringWithException() throws Exception {
        mJsonConfig.put(START_SEQUENCE_KEY, SERVICE_NAME);
        when(mJsonObjectMock.keys()).thenReturn(mJsonConfig.keySet().iterator());

        when(mJsonObjectMock.get(START_SEQUENCE_KEY))
                .thenThrow(new JSONException("test_exception"));

        assertEquals(Collections.emptyList(), mJsonPreParser.getLinesFromString("config"));
        verifyZeroInteractions(mInfoContainerMock);
    }

    @Test
    public void getListFromStringExceptionWithoutStartSequence() throws JSONException {
        prepareConfig();

        JSONObject obj = spy(Whitebox.newInstance(JSONObject.class));

        when(mJsonObjectMock.keys()).thenReturn(mJsonConfig.keySet().iterator());
        when(mJsonObjectMock.get(anyString())).thenReturn(obj);
        when(obj.keys()).thenReturn(mJsonConfig.keySet().iterator());
        when(obj.get(anyString())).thenThrow(new JSONException("test_exception"));

        assertEquals(Collections.emptyList(), mJsonPreParser.getLinesFromString("config"));
    }

    private void prepareConfig() {
        mJsonConfig.put("cls", "class");
        mJsonConfig.put("pkg", "package");
        mJsonConfig.put("esm", "ESM");
        mJsonConfig.put("action", "action");
        mJsonConfig.put("wait", "true");
        mJsonConfig.put("unexpected", "non");
    }

    private Set<String> prepareStringKeysSet() {
        Set<String> keys = mJsonConfig.keySet();

        // remove not-String keys
        keys.remove("wait");
        keys.remove("unexpected");

        return keys;
    }
}
