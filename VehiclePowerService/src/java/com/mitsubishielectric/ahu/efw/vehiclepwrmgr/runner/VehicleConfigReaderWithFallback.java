/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_VCONF_READ_ERROR;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_VCONF_READ_FALLBACK;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import java.util.Collections;
import java.util.List;
import java.util.Objects;

/**
 * VehicleConfigReaderWithFallback.
 *
 * <p>Implementation of IScriptReader. Serve to convert one raw multiline String (with \n delim)
 * into List<String>.
 */
public final class VehicleConfigReaderWithFallback implements IScriptReader {
    private final IVehicleConfigReader mIVehicleConfigReader;
    private final String mKeyString;
    private final String mFallbackString;
    private final IPreParser mPreParser;

    /**
     * Constructor.
     *
     * @param keyString key string that use for getting data (Map).
     * @param iVehicleConfigReader data access connector.
     */
    public VehicleConfigReaderWithFallback(
            String keyString,
            String fallBackString,
            IVehicleConfigReader iVehicleConfigReader,
            IPreParser preParser) {
        mIVehicleConfigReader =
                Objects.requireNonNull(
                        iVehicleConfigReader,
                        "VehicleConfigReader.constructor(): "
                                + "IVehicleConfigReader argument is NULL");
        mKeyString =
                Objects.requireNonNull(
                        keyString,
                        "VehicleConfigReader.constructor(): " + "String argument is NULL");
        mFallbackString =
                Objects.requireNonNull(
                        fallBackString,
                        "VehicleConfigReader.constructor(): " + "fallbackString argument is NULL");
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
            List<String> result = mPreParser.getLinesFromString(rawString);
            if (result.isEmpty()) {
                logWarning(RUNNER_VCONF_READ_FALLBACK);
                rawString = mIVehicleConfigReader.getString(mFallbackString);
                result = mPreParser.getLinesFromString(rawString);
            }
            return result;
        } catch (VehicleConfigManagerException e) {
            logWarning(RUNNER_VCONF_READ_ERROR, mKeyString + ", Reason : " + e.getMessage());
            return Collections.emptyList();
        }
    }
}
