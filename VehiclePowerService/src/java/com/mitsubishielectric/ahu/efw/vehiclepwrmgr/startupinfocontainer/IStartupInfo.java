/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer;

import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EServiceState;

public interface IStartupInfo {
    /**
     * check whether start command was issued to particular service by VPS
     *
     * @param className name of the Service's main class
     * @return true if command was issued
     */
    boolean isServiceStarted(String className);

    /**
     * Inform IStartupInfo implementation that start command was issued for a particular service
     *
     * @param className name of Service's main class
     */
    void onServiceStarted(String className);

    /**
     * Inform IStartupInfo that Service has completed it's stat-up activities
     *
     * @param className name of the Service's main class
     */
    void onServiceStartupCompleted(String className);

    /**
     * Chek whether all Firmware services started
     *
     * @return true if all Fw services started
     */
    boolean isAllServicesStarted();

    /**
     * Check whether early App services started
     *
     * @return true if early App services started
     */
    boolean isEarlyAppsStarted();

    /**
     * Check whether all App services started
     *
     * @return true if all App services started
     */
    boolean isAllAppsStarted();

    /**
     * Inform IStartupInfo implementation that all Fw services started
     *
     * @param isStarted true if all Fw services started
     */
    void setAllServicesStarted(boolean isStarted);

    /**
     * Inform IStartupInfo implementation that early App services started
     *
     * @param isStarted true if early App services started
     */
    void setEarlyAppsStarted(boolean isStarted);

    /**
     * Inform IStartupInfo implementation that all App services started
     *
     * @param isStarted true if all App services started
     */
    void setAllAppsStarted(boolean isStarted);

    /**
     * Wait for start complete notification from service
     *
     * @param className name of the Service's main class
     * @param timeoutMs timeout in milliseconds, zero value means an infinite timeout.
     */
    void waitForStartupComplete(String className, int timeoutMs);

    EServiceState getServiceState(String className);

    void onStopComplete(String className);

    void onRestartResumeComplete(String className);

    /** Check whether all Normal EFW Services called startComplete method */
    boolean isNormalEfwServicesStartComplete();

    /** Check whether all Normal APP Services startup sequence finished */
    boolean isNormalAppServicesStartComplete();

    /** Check whether all Late EFW Services called startComplete method */
    boolean isLateEfwServicesStartComplete();

    /** Check whether all Late APP Services called startComplete method */
    boolean isLateAppServicesStartComplete();

    /** Get short class name for logging if the service was started by VPS */
    String getShortClassName(String className);
}
