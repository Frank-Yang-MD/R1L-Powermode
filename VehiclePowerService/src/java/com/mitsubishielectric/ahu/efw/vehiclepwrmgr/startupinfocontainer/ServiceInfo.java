/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer;

/** Data class contains information about service */
public final class ServiceInfo {
    private final String mClassName;
    private final String mPackageName;
    private final String mEsmRegistrationName;
    private final String mAction;
    private final boolean mWaitNeeded;
    private final EServiceType mServiceType;

    public ServiceInfo(
            String className,
            String packageName,
            String esmRegistrationName,
            String action,
            boolean waitNeeded,
            EServiceType serviceType) {
        mClassName = className;
        mPackageName = packageName;
        mEsmRegistrationName = esmRegistrationName;
        mAction = action;
        mWaitNeeded = waitNeeded;
        mServiceType = serviceType;
    }

    /**
     * Service's main class name getter
     *
     * @return main class of service, String
     */
    public String getClassName() {
        return mClassName;
    }

    /**
     * Service package getter
     *
     * @return package getter
     */
    public String getPackageName() {
        return mPackageName;
    }

    /**
     * Get Esm Registration name of the service
     *
     * @return esm registration name, String
     */
    public String getEsmRegistrationName() {
        return mEsmRegistrationName;
    }

    /**
     * Get action needed to start service
     *
     * @return action, String
     */
    public String getAction() {
        return mAction;
    }

    /**
     * Should we wait for this service's startup
     *
     * @return true if wait is needed
     */
    public boolean isWaitNeeded() {
        return mWaitNeeded;
    }

    /**
     * Get service type
     *
     * @return service type
     */
    public EServiceType getServiceType() {
        return mServiceType;
    }
}
