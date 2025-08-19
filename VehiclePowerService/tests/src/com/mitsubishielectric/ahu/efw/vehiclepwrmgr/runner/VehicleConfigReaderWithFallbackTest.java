/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg;
import java.util.ArrayList;
import java.util.List;
import org.junit.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.powermock.api.mockito.PowerMockito;
import org.powermock.core.classloader.annotations.PrepareForTest;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor({"com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog"})
@PrepareForTest({VPSLogMsg.class, MLog.class})
public class VehicleConfigReaderWithFallbackTest {
    @Mock private IVehicleConfigReader mockIVehicleConfigReader;
    @Mock private IPreParser mockIPreParser;
    private final String keyString = "keyString";
    private final String fallbackString = "fallbackString";
    private final String rawEmptyString = "rawEmptyString";
    private final String rawString = "rawString";
    private List<String> notEmptyList = new ArrayList<>();
    private final List<String> emptyList = new ArrayList<>();

    private VehicleConfigReaderWithFallback uut;

    @Before
    public void setup() {
        PowerMockito.mockStatic(VPSLogMsg.class);
        uut =
                new VehicleConfigReaderWithFallback(
                        keyString, fallbackString, mockIVehicleConfigReader, mockIPreParser);
        notEmptyList.add("element");
    }

    @Test
    public void readWithNoExceptionAndNotEmptyResultTest() throws Exception {
        Mockito.when(mockIVehicleConfigReader.getString(keyString)).thenReturn(rawString);
        Mockito.when(mockIPreParser.getLinesFromString(rawString)).thenReturn(notEmptyList);

        List<String> result = uut.read();
        Assert.assertEquals(result, notEmptyList);
    }

    @Test
    public void readWithNoExceptionAndEmptyResultTest() throws Exception {
        Mockito.when(mockIVehicleConfigReader.getString(keyString)).thenReturn(rawEmptyString);
        Mockito.when(mockIPreParser.getLinesFromString(rawString)).thenReturn(emptyList);

        List<String> result = uut.read();
        Assert.assertEquals(result, emptyList);
        Mockito.verify(mockIVehicleConfigReader, Mockito.times(1)).getString(fallbackString);
    }

    @Test
    public void readWithException() throws Exception {
        VehicleConfigManagerException mockException =
                Mockito.mock(VehicleConfigManagerException.class);

        Mockito.when(mockIVehicleConfigReader.getString(keyString)).thenThrow(mockException);

        uut.read();
        Mockito.verify(mockException, Mockito.times(1)).getMessage();
    }
}
