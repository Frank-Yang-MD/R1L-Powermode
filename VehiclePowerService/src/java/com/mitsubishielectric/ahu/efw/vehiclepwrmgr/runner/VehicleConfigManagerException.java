/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import java.io.IOException;

/**
 * VehicleConfigManagerException.
 *
 * <p>Exception that have to be thrown in case of some method in VehicleConfigManager return
 * ERR_FAILED code.
 */
class VehicleConfigManagerException extends IOException {
    // Constructor that accepts a message
    public VehicleConfigManagerException(String message, Throwable e) {
        super(message, e);
    }

    public VehicleConfigManagerException(String message) {
        super(message);
    }
}
