/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.extsrvmanagerrunner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ESM_BIND_FAILED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ESM_BIND_SERVICE_RETURN;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_EARLY_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import android.content.Context;
import android.content.Intent;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;

/** Run Extended Service Manager. */
public final class ExtSrvManagerRunner {
    private static final String ESM_PACKAGE =
            "com.mitsubishielectric.ahu.efw.extendedservicemanager";
    private static final String ESM_SERVICE = ".ExtSrvMngService";
    private final ServiceConnected mServiceConnection;
    private final Context mServiceContext;

    public ExtSrvManagerRunner(
            IExtSrvMgrRunNotification extSrvMgrRunNotification, Context context) {
        mServiceContext = context;

        mServiceConnection = new ServiceConnected(extSrvMgrRunNotification);
    }

    public void run() {
        try {
            Intent intent = buildExplicitIntent();
            boolean result =
                    mServiceContext.bindService(
                            intent, mServiceConnection, Context.BIND_AUTO_CREATE);
            MLog.i(VPS_EARLY_FUNCTION_ID, ESM_BIND_SERVICE_RETURN, String.valueOf(result));
            MLog.i(VPS_FUNCTION_ID, ESM_BIND_SERVICE_RETURN, String.valueOf(result));
            mServiceContext.startService(intent);
        } catch (Exception e) {
            logWarning(ESM_BIND_FAILED, e.getMessage());
        }
    }

    private Intent buildExplicitIntent() {
        Intent intent = new Intent(ESM_PACKAGE + ESM_SERVICE);
        intent.setClassName(ESM_PACKAGE, ESM_PACKAGE + ESM_SERVICE);
        return intent;
    }
}
