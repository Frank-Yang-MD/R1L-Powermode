/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.lib.common.Const.ESM_SERVICE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_START_CMD_CREATE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_START_CMD_EXEC;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_START_CMD_FAIL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_START_CMD_SRV_OK;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_START_CMD_START_EXC;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.IBinder;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;

/** Implementation of start service/application command */
public final class StartCommand implements IScriptCommand {
    /** Service context */
    private final Context mContext;

    /** IBinder interface of ESM */
    private final IBinder mEsmBinder;

    /** Component name for run */
    private final ComponentName mComponentName;

    /** Optional action string if need */
    private String mAction;

    /** Optional wait for start complete notification flag */
    private boolean mWaitForStartComplete;

    /** Optional wait for start complete timeout, ms */
    private int mWaitForStartCompleteTimeoutMs = 0;

    /** Startup info container to update for started services */
    private IStartupInfo mStartupInfo;

    StartCommand(
            Context context,
            IBinder esmBinder,
            ComponentName componentName,
            IStartupInfo startupInfo) {
        mContext = context;
        mEsmBinder = esmBinder;
        mComponentName = componentName;
        mStartupInfo = startupInfo;
        MLog.i(VPS_FUNCTION_ID, RUNNER_START_CMD_CREATE, mComponentName.flattenToString());
    }

    public void setAction(String action) {
        mAction = action;
    }

    public void enableWaitForStartComplete(boolean enable, int timeoutMs) {
        mWaitForStartCompleteTimeoutMs = timeoutMs;
        mWaitForStartComplete = enable;
    }

    @Override
    public void execute() {
        final String flattenComponentName = mComponentName.flattenToString();
        MLog.i(VPS_FUNCTION_ID, RUNNER_START_CMD_EXEC, flattenComponentName);

        try {
            Intent intent = makeIntent();
            ComponentName name = mContext.startService(intent);
            if (name == null) {
                logWarning(RUNNER_START_CMD_FAIL, flattenComponentName);
                return;
            }
            MLog.i(VPS_FUNCTION_ID, RUNNER_START_CMD_SRV_OK, name.getClassName());
            mStartupInfo.onServiceStarted(name.getClassName());
            waitNotifyStartComplete();
        } catch (Exception e) {
            logWarning(RUNNER_START_CMD_START_EXC, e.getMessage());
        }
    }

    private void waitNotifyStartComplete() {
        if (mWaitForStartComplete) {
            mStartupInfo.waitForStartupComplete(
                    mComponentName.getClassName(), mWaitForStartCompleteTimeoutMs);
        }
    }

    private Intent makeIntent() {
        Bundle bundle = new Bundle();
        bundle.putBinder(ESM_SERVICE, mEsmBinder);

        Intent intent = new Intent();
        intent.setComponent(mComponentName);
        intent.putExtras(bundle);

        if (mAction != null) {
            intent.setAction(mAction);
        }
        return intent;
    }
}
