/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr;

/**
 * VehiclePowerService callback listener for other application
 */
interface IVehiclePowerServiceListener {
    /**
     * Receive APP_START_COLDBOOT notification
     */
    oneway void onAppStart();

    /**
     * Receive APP_RESTART notification
     */
    oneway void onAppRestart();

    /**
     * Receive APP_STOP notification
     */
    oneway void onAppStop();

    /**
     * Receive APP_RESUME notification
     */
    oneway void onAppResume();
}
