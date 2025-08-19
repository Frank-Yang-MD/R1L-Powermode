/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.WAITCMD_EXEC_FAILED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.WAITCMD_WAITING_FOR;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IInfoContainer;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.ServiceInfo;
import java.util.List;
import java.util.Objects;

public class WaitCommand implements IScriptCommand {
    private final List<String> mServiceList;
    private final IStartupInfo mStartupInfo;
    private final IInfoContainer mInfoContainer;
    private final int mWaitTimeoutMs;

    public WaitCommand(
            List<String> serviceList,
            IStartupInfo startupInfo,
            IInfoContainer infoContainer,
            int timeoutMs) {
        mServiceList =
                Objects.requireNonNull(serviceList, "WaitCommand() serviceList should be not null");
        mStartupInfo = startupInfo;
        mInfoContainer = infoContainer;
        mWaitTimeoutMs = timeoutMs;
    }

    @Override
    public void execute() {
        try {
            mServiceList.stream()
                    .map(mInfoContainer::getServiceInfo)
                    .map(ServiceInfo::getClassName)
                    .forEach(this::waitNotifyStartComplete);
        } catch (IllegalArgumentException e) {
            logWarning(WAITCMD_EXEC_FAILED, e.getMessage());
        }
    }

    private void waitNotifyStartComplete(String entryName) {
        MLog.d(VPS_FUNCTION_ID, WAITCMD_WAITING_FOR, entryName);
        mStartupInfo.waitForStartupComplete(entryName, mWaitTimeoutMs);
        MLog.d(VPS_FUNCTION_ID, WAITCMD_WAITING_FOR, entryName + " COMPLETE");
    }
}
