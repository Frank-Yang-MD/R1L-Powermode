/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.statemachine;

/** VehiclePowerService state interface */
interface IVpsState {
    /** Enter to the state */
    void onEnter();

    /** Exit from the state */
    void onExit();

    /**
     * Get state name
     *
     * @return state name
     */
    String stateName();

    /** Handle APP_START_COLDBOOT event from VehiclePowerDaemon */
    void onAppStartColdboot();

    /** Handle APP_RESTART event from VehiclePowerDaemon */
    void onAppRestart();

    /** Handle APP_RESUME event from VehiclePowerDaemon */
    void onAppResume();

    /** Handle APP_STOP event from VehiclePowerDaemon */
    void onAppStop();

    /** Handle request to stop FW services VehiclePowerDaemon */
    void onFwStop();

    /** Handle 'normal applications services started' notification */
    void onNormalAppServicesStarted();

    /** Handle 'late app service started' notification */
    void onLateAppServicesStarted();

    /** Handle 'normal EFW services started ' notification */
    void onNormalEfwServicesStarted();

    /** Handle 'late EFW services started' notification */
    void onLateEfwServicesStarted();

    /** Handle 'FW restart' request */
    void onFwRestart();

    /** Handle 'FW resume' notification */
    void onFwResume();

    /** Handle 'app service stop complete' notification */
    void onAppServicesStopComplete();

    /** Handle 'EFW services stop complete' notification */
    void onEfwServicesStopComplete();

    /** Handle 'EFW services resume complete' notification */
    void onEfwServicesResumeComplete();

    /** Handle 'EFW services restart complete' notification */
    void onEfwServicesRestartComplete();

    /** Handle 'App services resume complete' notification */
    void onAppServicesResumeComplete();

    /** Handle 'App services restart complete' notification */
    void onAppServicesRestartComplete();

    /** Handle 'CompletionTimeout' notification */
    void onCompletionTimeout();
}
