/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;
import org.junit.Assert;
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
public class VehicleConfigReaderTest {
    @Mock IVehicleConfigReader mMockIVehicleConfigReader;

    @Mock IPreParser mPreParser;

    @Before
    public void setUp() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
    }

    @Test(expected = NullPointerException.class)
    public void constructorFirstArgNull() {
        VehicleConfigReader localVehicleConfigReader =
                new VehicleConfigReader(null, mMockIVehicleConfigReader, mPreParser);
    }

    @Test(expected = NullPointerException.class)
    public void constructorSecondArgNull() {
        VehicleConfigReader localVehicleConfigReader =
                new VehicleConfigReader("BobPage".toString(), null, mPreParser);
    }

    @Test(expected = NullPointerException.class)
    public void constructorThirdArgNull() {
        VehicleConfigReader localVehicleConfigReader =
                new VehicleConfigReader("BobPage".toString(), mMockIVehicleConfigReader, null);
    }

    @Test
    public void testExceptionFromRead() throws VehicleConfigManagerException {
        Mockito.when(mMockIVehicleConfigReader.getString(""))
                .thenThrow(VehicleConfigManagerException.class);
        VehicleConfigReader localVehicleConfigReader =
                new VehicleConfigReader("", mMockIVehicleConfigReader, mPreParser);
        Assert.assertEquals(Collections.EMPTY_LIST, localVehicleConfigReader.read());
    }

    @Test
    public void read() throws VehicleConfigManagerException {
        String givenKeyString = "JC";
        String givenString =
                "A bomb is a bad choice for a close-range combat.\n"
                        + "Bravery is not a function of firepower.\n"
                        + "I was never properly trained in its operation.\n"
                        + "You might as well tell me the rest. If I'm gonna kill you, you are already dead"
                        + ".\n"
                        + "My vision is augmented.\n"
                        + "I'm not big into books.\n"
                        + "You're gonna burn alright.\n"
                        + "Did something go wrong?.";

        Mockito.when(mMockIVehicleConfigReader.getString(givenKeyString)).thenReturn(givenString);

        VehicleConfigReader localVehicleConfigReader =
                new VehicleConfigReader(givenKeyString, mMockIVehicleConfigReader, mPreParser);
        Mockito.when(mPreParser.getLinesFromString(givenString))
                .thenReturn(Arrays.asList(givenString.split("\\n")));

        List<String> result = localVehicleConfigReader.read();

        List<String> expectedArray =
                Arrays.asList(
                        "A bomb is a bad choice for a close-range " + "combat.",
                        "Bravery is not a function of firepower.",
                        "I was never properly trained in its operation.",
                        "You might as well tell me the rest. If I'm gonna kill you, you are already dead.",
                        "My vision is augmented.",
                        "I'm not big into books.",
                        "You're gonna burn alright.",
                        "Did something go wrong?.");

        Assert.assertEquals(expectedArray, result);
    }
}
