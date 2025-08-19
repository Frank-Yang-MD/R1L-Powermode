/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.lib.common.Error.ERR_FAILED;

import com.mitsubishielectric.ahu.efw.lib.vehicleconfigservice.VCString;
import com.mitsubishielectric.ahu.efw.lib.vehicleconfigservice.VehicleConfigManager;

/**
 * VehicleConfigReaderAdapter.
 *
 * <p>Class that implements IVehicleConfigReader for data (String) access by key through
 * VehicleConfigManager.
 */
public final class VehicleConfigReaderAdapter implements IVehicleConfigReader {
    private final VehicleConfigManager mVehicleConfigManager;

    /**
     * Constructor.
     *
     * @param vehicleConfigManager data access connector.
     */
    public VehicleConfigReaderAdapter(VehicleConfigManager vehicleConfigManager) {
        if (null == vehicleConfigManager) {
            throw new IllegalArgumentException(
                    "VehicleConfigReaderAdapter.constructor(): "
                            + "VehicleConfigManager argument is NULL");
        }
        mVehicleConfigManager = vehicleConfigManager;
    }

    /**
     * Return data from DB/File by key (Map).
     *
     * @param key a data key.
     * @return String data that connected to key.
     * @throws VehicleConfigManagerException in case if VehicleConfigManager return error code
     *     (ERR_FAILED, in case if DB/File is unreachable or data by given key is absent.
     */
    @Override
    public String getString(String key) throws VehicleConfigManagerException {
        VCString refVCString = new VCString();

        if (ERR_FAILED == mVehicleConfigManager.getString(key, refVCString)) {
            throw new VehicleConfigManagerException(
                    "VehicleConfigManager.getString("
                            + key
                            + ") return VehicleConfigManager"
                            + ".ERR_FAILED");
        }
        return refVCString.value;
    }
}
