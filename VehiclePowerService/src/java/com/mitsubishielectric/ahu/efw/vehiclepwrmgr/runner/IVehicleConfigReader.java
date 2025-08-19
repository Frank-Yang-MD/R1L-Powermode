/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

/**
 * IVehicleConfigReader interface.
 *
 * <p>Interface that represent more Java style method signature instead of VehicleConfigManager
 * class.
 */
public interface IVehicleConfigReader {

    /**
     * Return data from DB/File by key (Map).
     *
     * @param key a data key.
     * @return String data that connected to key.
     * @throws VehicleConfigManagerException in case if VehicleConfigManager return error code
     *     (ERR_FAILED, in case if DB/File is unreachable or data by given key is absent).
     */
    String getString(String key) throws VehicleConfigManagerException;
}
