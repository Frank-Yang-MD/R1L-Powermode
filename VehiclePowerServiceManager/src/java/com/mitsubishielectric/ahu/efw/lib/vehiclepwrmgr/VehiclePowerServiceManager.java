/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr;

import android.os.IBinder;
import android.os.RemoteException;

/** VehiclePowerServiceManager is wrapper class of VehiclePowerService */
public final class VehiclePowerServiceManager {

    private final IVehiclePowerService mService;
    private static IBinder sIBinder = null;
    private static VehiclePowerServiceManager sInstance = null;
    /**
     * Constructor of VehiclePowerServiceManager
     *
     * @throws IllegalStateException if sIBinder is null or Service is not initialized or
     */
    private VehiclePowerServiceManager() {
        if (sIBinder != null) {
            mService = IVehiclePowerService.Stub.asInterface(sIBinder);
            if (mService == null) {
                throw new IllegalStateException("Failed to find VehiclePowerService");
            }
        } else {
            throw new IllegalStateException("IBinder not set");
        }
    }
    /**
     * This method is entry point to get instance of VehiclePowerServiceManager
     *
     * @return VehiclePowerServiceManager instance.
     */
    public static synchronized VehiclePowerServiceManager getInstance() {
        if (sInstance == null) {
            sInstance = new VehiclePowerServiceManager();
        }
        return sInstance;
    }
    /**
     * This method set binder interface to VehiclePowerServiceManager.
     *
     * @param iBinder This is binder interface to VehiclePowerServiceManager.
     */
    public static void setBinder(IBinder iBinder) {
        sIBinder = iBinder;
    }
    /**
     * This method wrap subscribe method for AppServices
     *
     * @param listener implementation of listener interface
     * @throws RemoteException rethrows Androids RemoteException
     */
    public void subscribeApp(IVehiclePowerServiceListener listener, String className)
            throws RemoteException {
        mService.subscribeApp(listener, className);
    }
    /**
     * This method wrap subscribe method for Extended FW Services
     *
     * @param listener implementation of listener interface
     * @throws RemoteException rethrows Androids RemoteException
     */
    public void subscribeFWService(IVehiclePowerServiceListener listener, String className)
            throws RemoteException {
        mService.subscribeFWService(listener, className);
    }
    /**
     * Unsubsribe from VehiclePowerService notifications
     *
     * @param listener implementation of listener interface
     * @throws RemoteException rethrows Androids RemoteException
     */
    public void unsubscribe(IVehiclePowerServiceListener listener, String className)
            throws RemoteException {
        mService.unsubscribe(listener, className);
    }
    /**
     * Notify 'Stop processing complete' to VehiclePowerService
     *
     * @throws RemoteException rethrows Androids RemoteException
     */
    public void stopCompleteApp(String className) throws RemoteException {
        mService.stopCompleteApp(className);
    }
    /**
     * Notify 'Stop processing complete' to VehiclePowerService
     *
     * @throws RemoteException rethrows Androids RemoteException
     */
    public void stopCompleteEfw(String className) throws RemoteException {
        mService.stopCompleteEfw(className);
    }
    /**
     * API function to get last notified event from VPS
     *
     * @return last notified event
     */
    public EVpsState getNotifiedEvent() throws RemoteException {
        return mService.getNotifiedEvent();
    }
    /**
     * Check whether service has been given start command by VPS
     *
     * @param className String containing class name
     * @return boolean flag. True is for started.
     * @throws RemoteException in case of binder connection fails
     */
    public boolean isServiceStarted(String className) throws RemoteException {
        return mService.isServiceStarted(className);
    }
    /**
     * Notifies VPS that service is fully operational
     *
     * @param className String containing class name
     * @throws RemoteException in case of binder connection fails
     */
    public void startComplete(String className) throws RemoteException {
        mService.startComplete(className);
    }
    /**
     * Check whether applications startup sequence completed
     *
     * @return boolean flag. True is for completed.
     * @throws RemoteException in case of binder connection fails
     */
    public boolean isAllAppServicesStarted() throws RemoteException {
        return mService.isAllAppServicesStarted();
    }
    /**
     * Check whether services startup sequence completed
     *
     * @return boolean flag. True is for completed.
     * @throws RemoteException in case of binder connection fails
     */
    public boolean isAllEfwServicesStarted() throws RemoteException {
        return mService.isAllEfwServicesStarted();
    }
    /**
     * Check whether normal app services startup sequence completed
     *
     * @return boolean flag. True is for completed.
     * @throws RemoteException in case of binder connection fails
     */
    public boolean normalAppServicesStarted() throws RemoteException {
        return mService.normalAppServicesStarted();
    }
    /**
     * Prints Boot KPI marker
     *
     * @param markerTAG String containing marker name
     * @return the status code of Log.printMarker() function
     * @throws RemoteException in case of binder connection fails
     */
    public int printMarker(String markerTAG) throws RemoteException {
        return mService.printMarker(markerTAG);
    }
    /**
     * Notifies VPS resumeCompleteEfw event
     *
     * @throws RemoteException in case of binder connection fails
     */
    public void resumeCompleteEfw(String className) throws RemoteException {
        mService.resumeCompleteEfw(className);
    }
    /**
     * Notifies VPS restartCompleteEfw event
     *
     * @throws RemoteException in case of binder connection fails
     */
    public void restartCompleteEfw(String className) throws RemoteException {
        mService.restartCompleteEfw(className);
    }
    /**
     * Notifies VPS resumeCompleteApp event
     *
     * @throws RemoteException in case of binder connection fails
     */
    public void resumeCompleteApp(String className) throws RemoteException {
        mService.resumeCompleteApp(className);
    }
    /**
     * Notifies VPS restartCompleteApp event
     *
     * @throws RemoteException in case of binder connection fails
     */
    public void restartCompleteApp(String className) throws RemoteException {
        mService.restartCompleteApp(className);
    }
    /**
     * Get current EServiceState for a service
     *
     * @throws RemoteException in case of binder connection fails
     */
    public EServiceState getServiceState(String className) throws RemoteException {
        return mService.getServiceState(className);
    }

    /** Check whether all Normal EFW Services called startComplete method */
    public boolean isNormalEfwServicesStartComplete() throws RemoteException {
        return mService.isNormalEfwServicesStartComplete();
    }

    /** Check whether all Normal APP Services startup sequence finished */
    public boolean isNormalAppServicesStartComplete() throws RemoteException {
        return mService.isNormalAppServicesStartComplete();
    }

    /** Check whether all Late EFW Services called startComplete method */
    public boolean isLateEfwServicesStartComplete() throws RemoteException {
        return mService.isLateEfwServicesStartComplete();
    }

    /** Check whether all Late APP Services called startComplete method */
    public boolean isLateAppServicesStartComplete() throws RemoteException {
        return mService.isLateAppServicesStartComplete();
    }

    /** Subscribe for Boot Daemon status notifications */
    public void ismReady(IBootDaemonStatusListener listener) throws RemoteException {
        mService.ismReady(listener);
    }

    /** Send End Early Hmi request to Boot Daemon */
    public void requestEndEarlyHmi() throws RemoteException {
        mService.requestEndEarlyHmi();
    }

    /** Notify about forced completion of Early Services */
    public void forcedCompletionEarlyServices() throws RemoteException {
        mService.forcedCompletionEarlyServices();
    }
}
