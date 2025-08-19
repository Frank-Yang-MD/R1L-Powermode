package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runningchecker;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.STOPPED_SERVICES_ERROR;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logError;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import android.app.ActivityManager;
import android.content.Context;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IServiceListProvider;
import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;

public class ServicesRunningChecker implements IServicesRunningChecker {
    private final boolean ALL_SERVICES_STARTED_CHECK_ENABLED = true;

    private final IServiceListProvider mServiceListProvider;
    private final Context mContext;

    public ServicesRunningChecker(IServiceListProvider serviceListProvider, Context context) {
        mServiceListProvider =
                Objects.requireNonNull(
                        serviceListProvider, "Service list provider must be not null");
        mContext = Objects.requireNonNull(context, "Context must be not null");
    }

    @Override
    public boolean isAllServicesRunning() {
        List<String> startedServicesClassList =
                mServiceListProvider.getAllStartedServicesList().stream()
                        .map(info -> info.getClassName())
                        .collect(Collectors.toList());

        ActivityManager activityManager =
                (ActivityManager) mContext.getSystemService(Context.ACTIVITY_SERVICE);

        List<String> runningServicesClassList =
                activityManager.getRunningServices(Integer.MAX_VALUE).stream()
                        .map(info -> info.service.getClassName())
                        .collect(Collectors.toList());

        startedServicesClassList.removeAll(runningServicesClassList);

        boolean result = startedServicesClassList.size() == 0;

        if (!result) {
            logError(STOPPED_SERVICES_ERROR, "Not all services running.");
            logWarning(
                    STOPPED_SERVICES_ERROR,
                    "startedServicesClassList.size "
                            + String.valueOf(startedServicesClassList.size()));
            logWarning(STOPPED_SERVICES_ERROR, "startedServicesClassList:");
            for (int i = 0; i < startedServicesClassList.size(); i++) {
                logWarning(
                        STOPPED_SERVICES_ERROR,
                        "i: " + String.valueOf(i) + " value: " + startedServicesClassList.get(i));
            }
            logWarning(
                    STOPPED_SERVICES_ERROR,
                    startedServicesClassList.stream().collect(Collectors.joining(", ")));

            mServiceListProvider
                    .getAllStartedServicesList()
                    .forEach(
                            s_info -> {
                                logWarning(
                                        STOPPED_SERVICES_ERROR,
                                        "ClassName " + s_info.getClassName());
                                logWarning(
                                        STOPPED_SERVICES_ERROR,
                                        "PackageName " + s_info.getPackageName());
                                logWarning(
                                        STOPPED_SERVICES_ERROR,
                                        "EsmRegistrationName " + s_info.getEsmRegistrationName());
                                logWarning(STOPPED_SERVICES_ERROR, "Action " + s_info.getAction());
                                logWarning(
                                        STOPPED_SERVICES_ERROR,
                                        "IsWaitNeeded " + String.valueOf(s_info.isWaitNeeded()));
                            });
        }

        if (!ALL_SERVICES_STARTED_CHECK_ENABLED) {
            return true;
        }

        return result;
    }
}
