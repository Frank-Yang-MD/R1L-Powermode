/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IVehiclePowerServiceListener;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EVpsState;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EServiceState;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IBootDaemonStatusListener;

/**
 * API for VehiclePowerService
 */
interface IVehiclePowerService {
    /**
     * Subscribe to power state change notification
     */
    oneway void subscribeApp(in IVehiclePowerServiceListener listener, in String className);

    /**
     * Subscribe to power state change notification
     */
    oneway void subscribeFWService(in IVehiclePowerServiceListener listener, in String className);

    /**
     * Unsubscribe from power state change notifycation
     */
    oneway void unsubscribe(in IVehiclePowerServiceListener listener, in String className);

    /**
     * Send App stop processing complete notification to VehiclePowerService
     */
    oneway void stopCompleteApp(in String className);

    /**
     * Send Fw stop processing complete notification to VehiclePowerService
     */
    oneway void stopCompleteEfw(in String className);

    /**
     * Check whether service has started
     */
    boolean isServiceStarted(in String className);

    /**
     * Notify VPS that service is in full operational mode
     */
    oneway void startComplete(in String className);

    /**
     * Check whether Fw Services startup sequence finished
     */
    boolean isAllEfwServicesStarted();

    /**
     * Check whether Normal App Services startup sequence finished
     */
    boolean normalAppServicesStarted();

    /**
     * Check whether All App Services startup sequence finished
     */
    boolean isAllAppServicesStarted();

    /**
     *  Get current Vps State
     */
    EVpsState getNotifiedEvent();

    /**
     *  Prints Boot KPI marker
     */
    int printMarker(in String markerTAG);

    /**
     * Notify resumeCompleteEfw event
     */
    oneway void resumeCompleteEfw(in String className);

    /**
     * Notify that restartCompleteEfw event
     */
    oneway void restartCompleteEfw(in String className);

    /**
     * Notify resumeCompleteApp event
     */
    oneway void resumeCompleteApp(in String className);

    /**
     * Notify that restartCompleteApp event
     */
    oneway void restartCompleteApp(in String className);

    /**
     *  Get current Service State
     */
    EServiceState getServiceState(in String className);

    /**
     * Check whether all Normal EFW Services called startComplete method
     */
    boolean isNormalEfwServicesStartComplete();

    /**
     * Check whether all Normal APP Services startup sequence finished
     */
    boolean isNormalAppServicesStartComplete();

    /**
     * Check whether all Late EFW Services called startComplete method
     */
    boolean isLateEfwServicesStartComplete();

    /**
     * Check whether all Late APP Services called startComplete method
     */
    boolean isLateAppServicesStartComplete();

    /**
     * Subscribe for Boot Daemon status notifications
     */
    oneway void ismReady(in IBootDaemonStatusListener listener);

    /**
     * Send request to end early HMI to Boot Daemon
     */
    oneway void requestEndEarlyHmi();

    /**
     * Notify about forced completion of Early Services
     */
    oneway void forcedCompletionEarlyServices();
}

