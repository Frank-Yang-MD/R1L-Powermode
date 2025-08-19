/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.extsrvmanagerrunner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.ESM_BOUND_SUCCESS;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_EARLY_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;

import android.content.ComponentName;
import android.content.ServiceConnection;
import android.os.IBinder;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;

/**
 * This class is intended as callback and is passed to started ESM service. Started ESM service
 * should call the function onServiceConnected() to notify that connection is OK.
 */
public class ServiceConnected implements ServiceConnection {
    private final IExtSrvMgrRunNotification mExtSrvMgrRunNotification;
    private boolean mConnectedFlag = false;

    public ServiceConnected(IExtSrvMgrRunNotification extSrvMgrRunNotification) {
        mExtSrvMgrRunNotification = extSrvMgrRunNotification;
    }

    @Override
    public synchronized void onServiceConnected(ComponentName name, IBinder service) {
        if (!mConnectedFlag) {
            mConnectedFlag = true;
            MLog.i(VPS_EARLY_FUNCTION_ID, ESM_BOUND_SUCCESS, name.flattenToString());
            MLog.i(VPS_FUNCTION_ID, ESM_BOUND_SUCCESS, name.flattenToString());
            mExtSrvMgrRunNotification.notifyExtSrvMgrBound(service);
        }
    }

    @Override
    public void onServiceDisconnected(ComponentName name) {}
}
