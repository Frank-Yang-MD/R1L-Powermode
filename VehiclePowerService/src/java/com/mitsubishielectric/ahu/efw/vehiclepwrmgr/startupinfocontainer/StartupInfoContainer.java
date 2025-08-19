/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SERVICE_RESUME_RESTART_INVALID_CLASSNAME;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SERVICE_STARTED_INVALID_CLASSNAME;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SERVICE_START_COMPLETE_INVALID_CLASSNAME;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SERVICE_STOP_INVALID_CLASSNAME;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SIC_SERVICEINFO_ALREADY_EXIST;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SIC_SERVICEINFO_WAS_ADDED_TO;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SIC_TRY_TO_ADD_SERVICEINFO;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.WAIT_STARTUP_COMPLETE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.WAIT_STARTUP_FAILED_SERVICE_DOESNT_EXIST;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.WAIT_STARTUP_FINISHED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EServiceState;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative.IVpdNativeAdapter;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ConcurrentSkipListSet;
import java.util.stream.Collectors;

/** Used to store startup info and to help parse startup commands */
public class StartupInfoContainer implements IInfoContainer, IStartupInfo, IServiceListProvider {
    private final Map<String, StartupInfoEntry> configContainer = new ConcurrentHashMap<>();
    private final Map<String, StartupInfoEntry> configContainerByClass = new ConcurrentHashMap<>();
    private final Map<EServiceType, Set<String>> serviceContainerByType = new ConcurrentHashMap<>();
    private final Set<String> startedServices = new ConcurrentSkipListSet<>();
    private volatile boolean mAllServicesStarted = false;
    private volatile boolean mEarlyAppsStarted = false;
    private volatile boolean mAllAppsStarted = false;
    private IVpdNativeAdapter mVpd;

    public StartupInfoContainer() {
        serviceContainerByType.put(EServiceType.LATE_APP, new HashSet<>());
        serviceContainerByType.put(EServiceType.LATE_EFW, new HashSet<>());
        serviceContainerByType.put(EServiceType.NORMAL_APP, new HashSet<>());
        serviceContainerByType.put(EServiceType.NORMAL_EFW, new HashSet<>());
    }

    public StartupInfoContainer use(IVpdNativeAdapter vpd) {
        mVpd = vpd;
        return this;
    }

    public void add(String name, ServiceInfo serviceInfo) {
        MLog.i(VPS_FUNCTION_ID, SIC_TRY_TO_ADD_SERVICEINFO, name);
        if (!configContainer.containsKey(name)) {
            StartupInfoEntry infoEntry = new StartupInfoEntry(serviceInfo, mVpd);
            configContainer.put(name, infoEntry);
            String className = infoEntry.getServiceInfo().getClassName();
            configContainerByClass.put(className, infoEntry);
            serviceContainerByType.get(infoEntry.getServiceInfo().getServiceType()).add(className);
            MLog.i(VPS_FUNCTION_ID, SIC_SERVICEINFO_WAS_ADDED_TO, name);
        } else {
            MLog.w(VPS_FUNCTION_ID, SIC_SERVICEINFO_ALREADY_EXIST, name);
        }
    }

    @Override
    public ServiceInfo getServiceInfo(String entryName) {
        if (!configContainer.containsKey(entryName)) {
            throw new IllegalArgumentException(
                    "getServiceInfo FAILED. Key '" + entryName + "' doesn't exist!");
        }
        return configContainer.get(entryName).getServiceInfo();
    }

    @Override
    public List<ServiceInfo> getAllStartedServicesList() {
        return configContainer.values().stream()
                .map(entry -> entry.getServiceInfo())
                .collect(Collectors.toList());
    }

    @Override
    public String getShortClassName(String className) {
        String result = "";
        for (Map.Entry<String, StartupInfoEntry> entry : configContainer.entrySet()) {
            if (entry.getValue().getServiceInfo().getClassName().equals(className)) {
                result = entry.getKey();
                break;
            }
        }
        return result.isEmpty() ? className : result;
    }

    @Override
    public boolean isServiceStarted(String className) {
        StartupInfoEntry infoEntry = configContainerByClass.get(className);
        if (infoEntry != null) {
            return infoEntry.isStarted();
        } else {
            return false;
        }
    }

    @Override
    public void onServiceStarted(String className) {
        StartupInfoEntry infoEntry = configContainerByClass.get(className);
        if (infoEntry != null) {
            infoEntry.setIsStarted(true);
        } else {
            logWarning(SERVICE_STARTED_INVALID_CLASSNAME, className);
        }
    }

    @Override
    public void onServiceStartupCompleted(String className) {
        startedServices.add(className);
        StartupInfoEntry infoEntry = configContainerByClass.get(className);
        if (infoEntry != null) {
            infoEntry.setIsStartCompleted(true);
        } else {
            logWarning(SERVICE_START_COMPLETE_INVALID_CLASSNAME, className);
        }
    }

    @Override
    public boolean isAllServicesStarted() {
        return mAllServicesStarted;
    }

    @Override
    public boolean isEarlyAppsStarted() {
        return mEarlyAppsStarted;
    }

    @Override
    public boolean isAllAppsStarted() {
        return mAllAppsStarted;
    }

    @Override
    public void setAllServicesStarted(boolean isStarted) {
        mAllServicesStarted = isStarted;
    }

    @Override
    public void setEarlyAppsStarted(boolean isStarted) {
        mEarlyAppsStarted = isStarted;
    }

    @Override
    public void setAllAppsStarted(boolean isStarted) {
        mAllAppsStarted = isStarted;
    }

    @Override
    public void waitForStartupComplete(String className, int timeout) {
        MLog.i(VPS_FUNCTION_ID, WAIT_STARTUP_COMPLETE, timeout, className);

        StartupInfoEntry infoEntry = configContainerByClass.get(className);
        if (infoEntry == null) {
            logWarning(VPS_FUNCTION_ID, WAIT_STARTUP_FAILED_SERVICE_DOESNT_EXIST, className);
            return;
        }
        if (timeout > 0) {
            infoEntry.waitStartComplete(timeout);
        } else {
            infoEntry.waitStartComplete();
        }
        MLog.i(VPS_FUNCTION_ID, WAIT_STARTUP_FINISHED, className);
    }

    @Override
    public EServiceState getServiceState(String className) {
        StartupInfoEntry infoEntry = configContainerByClass.get(className);
        if (infoEntry != null) {
            return infoEntry.getServiceState();
        } else {
            return EServiceState.NOT_STARTED;
        }
    }

    @Override
    public void onStopComplete(String className) {
        StartupInfoEntry infoEntry = configContainerByClass.get(className);
        if (infoEntry != null) {
            infoEntry.setStopCompleted();
        } else {
            logWarning(SERVICE_STOP_INVALID_CLASSNAME, className);
        }
    }

    @Override
    public void onRestartResumeComplete(String className) {
        StartupInfoEntry infoEntry = configContainerByClass.get(className);
        if (infoEntry != null) {
            infoEntry.setResumedRestarted();
        } else {
            logWarning(SERVICE_RESUME_RESTART_INVALID_CLASSNAME, className);
        }
    }

    @Override
    public boolean isNormalEfwServicesStartComplete() {
        return isSetsDifferent(
                serviceContainerByType.get(EServiceType.NORMAL_EFW), startedServices);
    }

    @Override
    public boolean isNormalAppServicesStartComplete() {
        return isSetsDifferent(
                serviceContainerByType.get(EServiceType.NORMAL_APP), startedServices);
    }

    @Override
    public boolean isLateEfwServicesStartComplete() {
        return isSetsDifferent(serviceContainerByType.get(EServiceType.LATE_EFW), startedServices);
    }

    @Override
    public boolean isLateAppServicesStartComplete() {
        return isSetsDifferent(serviceContainerByType.get(EServiceType.LATE_APP), startedServices);
    }

    private boolean isSetsDifferent(final Set<String> container, final Set<String> started) {
        Set<String> result = new HashSet<>(container);
        result.removeAll(started);
        return result.size() == 0;
    }
}
