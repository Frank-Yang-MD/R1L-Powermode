/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IVehiclePowerServiceListener;

/** Subscription / unsubscription interface */
public interface ISubscriptionManager {
    /**
     * Subscribe
     *
     * @param pwrListener client callback
     * @param className client className
     */
    void subscribe(IVehiclePowerServiceListener pwrListener, String className);

    /**
     * Unsubscribe
     *
     * @param pwrListener client callback
     * @param className client className
     */
    void unsubscribe(IVehiclePowerServiceListener pwrListener, String className);
}
