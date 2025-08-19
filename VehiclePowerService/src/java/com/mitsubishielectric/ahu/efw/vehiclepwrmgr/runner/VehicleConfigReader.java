/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_VCONF_READ_ERROR;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import java.util.Collections;
import java.util.List;
import java.util.Objects;

/**
 * VehicleConfigReader.
 *
 * <p>Implementation of IScriptReader. Serve to convert one raw multiline String (with \n delim)
 * into List<String>.
 */
public final class VehicleConfigReader implements IScriptReader {
    private final IVehicleConfigReader mIVehicleConfigReader;
    private final String mKeyString;
    private final IPreParser mPreParser;

    /**
     * Constructor.
     *
     * @param keyString key string that use for getting data (Map).
     * @param iVehicleConfigReader data access connector.
     */
    public VehicleConfigReader(
            String keyString, IVehicleConfigReader iVehicleConfigReader, IPreParser preParser) {
        mIVehicleConfigReader =
                Objects.requireNonNull(
                        iVehicleConfigReader,
                        "VehicleConfigReader.constructor(): "
                                + "IVehicleConfigReader argument is NULL");
        mKeyString =
                Objects.requireNonNull(
                        keyString,
                        "VehicleConfigReader.constructor(): " + "String argument is NULL");
        mPreParser =
                Objects.requireNonNull(
                        preParser,
                        "VehicleConfigReader.constructor(): " + "IPreParser argument is NULL");
    }

    /** @return list of Strings. */
    @Override
    public List<String> read() {
        try {
            String rawString = mIVehicleConfigReader.getString(mKeyString);
            return mPreParser.getLinesFromString(rawString);
        } catch (VehicleConfigManagerException e) {
            logWarning(RUNNER_VCONF_READ_ERROR, mKeyString + ", Reason : " + e.getMessage());
            return Collections.emptyList();
        }
    }
}
