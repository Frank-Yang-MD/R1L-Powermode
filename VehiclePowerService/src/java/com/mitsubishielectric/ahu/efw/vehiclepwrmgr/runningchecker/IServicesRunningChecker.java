/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runningchecker;

/** Interface to check is all services started by VPS running at this moment */
public interface IServicesRunningChecker {

    /**
     * Checks if all services running now
     *
     * @return boolean value which represents services running state
     */
    boolean isAllServicesRunning();
}
