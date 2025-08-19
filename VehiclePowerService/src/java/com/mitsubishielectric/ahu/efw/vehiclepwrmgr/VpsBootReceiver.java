/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.EXCEPTION_IN_ON_RECEIVE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RECEIVED_ACTION;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SERVICE_START_FAIL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SERVICE_START_SUCCESS;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_EARLY_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.initializeLogDog;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;

/** Receive android.intent.action.LOCKED_BOOT_COMPLETED and start service */
public final class VpsBootReceiver extends BroadcastReceiver {

    @Override
    public void onReceive(Context context, Intent intent) {
        initializeLogDog();
        MLog.i(VPS_EARLY_FUNCTION_ID, RECEIVED_ACTION, intent.getAction());
        try {
            Intent startIntent = new Intent(context, VehiclePowerService.class);
            ComponentName name = context.startService(startIntent);
            if (name != null) {
                MLog.i(VPS_EARLY_FUNCTION_ID, SERVICE_START_SUCCESS, name.toString());
            } else {
                MLog.i(VPS_EARLY_FUNCTION_ID, SERVICE_START_FAIL);
            }
        } catch (Exception e) {
            logWarning(EXCEPTION_IN_ON_RECEIVE, e.getMessage());
        }
    }
}
