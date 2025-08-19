/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.lib.common.Error.ERR_FAILED;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.doAnswer;

import com.mitsubishielectric.ahu.efw.lib.vehicleconfigservice.VCString;
import com.mitsubishielectric.ahu.efw.lib.vehicleconfigservice.VehicleConfigManager;
import junit.framework.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.invocation.InvocationOnMock;
import org.mockito.junit.MockitoJUnitRunner;
import org.mockito.stubbing.Answer;

@RunWith(MockitoJUnitRunner.class)
public class VehicleConfigReaderAdapterTest {
    @Mock VehicleConfigManager mMockVehicleConfigManager;

    @Test(expected = IllegalArgumentException.class)
    public void constructorNullArg() {
        IVehicleConfigReader localVehicleConfigReaderAdapter = new VehicleConfigReaderAdapter(null);
    }

    @Test
    public void getString() throws VehicleConfigManagerException {
        String givenKeyString = "MJ-12";
        String givenResultString =
                "Individuals can remember, but organizations in every possible"
                        + " way prevent this. One artist, one general, one hero or one villain can die, but"
                        + " it is impossible to kill people, a nation, an idea - only if the idea does not "
                        + "grow out of weak-willed ones and is crushed by stronger ones.";

        doAnswer(
                        new Answer() {
                            public Object answer(InvocationOnMock invocation) {
                                Object[] args = invocation.getArguments();
                                ((VCString) args[1]).value = givenResultString;
                                return null;
                            }
                        })
                .when(mMockVehicleConfigManager)
                .getString(eq(givenKeyString), any(VCString.class));

        IVehicleConfigReader localVehicleConfigReaderAdapter =
                new VehicleConfigReaderAdapter(mMockVehicleConfigManager);

        String resultString = localVehicleConfigReaderAdapter.getString(givenKeyString);

        Assert.assertEquals(givenResultString, resultString);
    }

    @Test(expected = VehicleConfigManagerException.class)
    public void getStringException() throws VehicleConfigManagerException {
        String givenKeyString = "John Milton";
        String givenResultString = "Better to reign in Hell, than to serve in Heaven.";

        doAnswer(
                        new Answer() {
                            public Object answer(InvocationOnMock invocation) {
                                Object[] args = invocation.getArguments();
                                ((VCString) args[1]).value = givenResultString;
                                return ERR_FAILED;
                            }
                        })
                .when(mMockVehicleConfigManager)
                .getString(eq(givenKeyString), any(VCString.class));

        IVehicleConfigReader localVehicleConfigReaderAdapter =
                new VehicleConfigReaderAdapter(mMockVehicleConfigManager);

        String resultString = localVehicleConfigReaderAdapter.getString(givenKeyString);
    }
}
