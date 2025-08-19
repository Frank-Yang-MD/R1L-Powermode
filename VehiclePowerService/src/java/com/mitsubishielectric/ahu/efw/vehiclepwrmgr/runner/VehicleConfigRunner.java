/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.lib.common.Const.ESM_SERVICE;
import static com.mitsubishielectric.ahu.efw.lib.common.Const.VEHICLE_CONFIG_READER_SERVICE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_VCONF_RUN_BIND_NULL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_VCONF_RUN_DONE_ESM;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_VCONF_RUN_EXC_ESM;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_VCONF_RUN_SRV_FAIL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_VCONF_RUN_SRV_OK;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_VCONF_RUN_START_FAIL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_VCONF_RUN_VPS_FAIL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_VCONF_RUN_VPS_WAIT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_VCONF_RUN_WAIT_ESM;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.os.Bundle;
import android.os.IBinder;
import com.mitsubishielectric.ahu.efw.lib.extendedservicemanager.ExtSrvManager;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.lib.vehicleconfigservice.VehicleConfigManager;

/**
 * VehicleConfigRunner.
 *
 * <p>Serve for run VehicleConfigReader service and getting VehicleConfigManager instance with set
 * read binder. <i> Main propose is get instance of ExtSrvManager, run VehicleConfigReader service
 * by it and get the IBinder. Then return VehicleConfigManager with set the IBinder. We can`t start
 * VehicleConfigReader service out off file/key list because VehicleConfigReader service needed for
 * it itself. </i>
 */
public class VehicleConfigRunner {
    private static final String VEHICLE_CONFIG_PKG =
            "com.mitsubishielectric.ahu.efw" + ".vehicleconfigservice";
    private static final String VEHICLE_CONFIG_CLS = ".VehicleConfigServiceReader";

    private final Context mContext;
    private final IBinder mBinder;
    private final ExtSrvManager mExtSrvMgr;

    /**
     * Constructor.
     *
     * @param context Android context (executor)
     * @param binder Android binder
     */
    public VehicleConfigRunner(Context context, IBinder binder, ExtSrvManager extSrvManager) {
        if (context == null) {
            throw new IllegalArgumentException("Context is NULL!");
        }
        if (binder == null) {
            throw new IllegalArgumentException("IBinder is NULL!");
        }
        if (extSrvManager == null) {
            throw new IllegalArgumentException("ExtSrvManager is NULL!");
        }
        mContext = context;
        mBinder = binder;
        mExtSrvMgr = extSrvManager;
    }

    /** @return VehicleConfigManager instance with read binder */
    public VehicleConfigManager getReadyVehicleConfigManager() {
        boolean startResult = startService();
        if (!startResult) {
            logWarning(RUNNER_VCONF_RUN_VPS_FAIL);
            return null;
        }

        MLog.i(VPS_FUNCTION_ID, RUNNER_VCONF_RUN_VPS_WAIT);
        IBinder binder = waitEsmRegistration();
        if (binder == null) {
            logWarning(RUNNER_VCONF_RUN_BIND_NULL);
            return null;
        }
        VehicleConfigManager.setReaderBinder(binder);
        return VehicleConfigManager.getInstance();
    }

    private Intent makeIntent() {
        Bundle bundle = new Bundle();
        bundle.putBinder(ESM_SERVICE, mBinder);

        Intent intent = new Intent();
        intent.setComponent(
                new ComponentName(VEHICLE_CONFIG_PKG, VEHICLE_CONFIG_PKG + VEHICLE_CONFIG_CLS));
        intent.putExtras(bundle);
        return intent;
    }

    private boolean startService() {
        try {
            ComponentName name = mContext.startService(makeIntent());
            if (name != null) {
                MLog.i(VPS_FUNCTION_ID, RUNNER_VCONF_RUN_SRV_OK, name.getClassName());
            } else {
                logWarning(RUNNER_VCONF_RUN_SRV_FAIL);
                return false;
            }
        } catch (SecurityException | IllegalStateException e) {
            logWarning(RUNNER_VCONF_RUN_START_FAIL, e.getMessage());
            return false;
        }
        return true;
    }

    private IBinder waitEsmRegistration() {
        final byte EXT_SRV_MGR_POOL_INTERVAL = 50;
        IBinder binder = null;
        try {
            MLog.i(VPS_FUNCTION_ID, RUNNER_VCONF_RUN_WAIT_ESM);
            do {
                Thread.sleep(EXT_SRV_MGR_POOL_INTERVAL);
                binder = mExtSrvMgr.getService(VEHICLE_CONFIG_READER_SERVICE);
            } while (binder == null);
            MLog.i(VPS_FUNCTION_ID, RUNNER_VCONF_RUN_DONE_ESM);
        } catch (InterruptedException e) {
            logWarning(RUNNER_VCONF_RUN_EXC_ESM, e.getMessage());
            Thread.currentThread().interrupt();
        } catch (IllegalArgumentException e) {
            logWarning(RUNNER_VCONF_RUN_EXC_ESM, e.getMessage());
        }
        return binder;
    }
}
