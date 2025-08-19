/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.mountmanagement;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SMA_COULDNT_OBTAIN_BINDER;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SMA_MOUNTING_NULL_BINDER;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SMA_MOUNTING_REMOTE_EXCEPTION;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SMA_MOUNTING_RUNTIME_EXCEPTION;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SMA_MOUNTING_UNMOUNTED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SMA_MOUNTING_VOLUME;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SMA_OBTAINING_VOLUME_LIST;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SMA_UNMOUNTING_ALL;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SMA_UNMOUNTING_NULL_BINDER;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SMA_UNMOUNTING_REMOTE_EXCEPTION;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SMA_UNMOUNTING_RUNTIME_EXCEPTION;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SMA_UNMOUNTING_VOLUME;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SMA_VOLUME_LIST_OBTAINED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SMA_VOLUME_MOUNTED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SMA_VOLUME_UNMOUNTED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import android.os.RemoteException;
import android.os.ServiceManager;
import android.os.storage.IStorageManager;
import android.os.storage.VolumeInfo;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;

/**
 * This is an adapter to Android StorageManager. It is used for unmount volumes and mounting
 * previously unmounted.
 */
public class StorageManagerAdapter implements IStorageManagerAdapter {
    private IStorageManager mSm;

    public StorageManagerAdapter() {
        mSm = IStorageManager.Stub.asInterface(ServiceManager.getService("mount"));
        if (mSm == null) {
            logWarning(SMA_COULDNT_OBTAIN_BINDER);
        }
    }

    @Override
    public void unmountAll() {
        if (mSm == null) {
            logWarning(SMA_UNMOUNTING_NULL_BINDER);
            return;
        }
        try {
            MLog.i(VPS_FUNCTION_ID, SMA_UNMOUNTING_ALL);

            MLog.i(VPS_FUNCTION_ID, SMA_OBTAINING_VOLUME_LIST);
            final VolumeInfo[] vols = mSm.getVolumes(0);
            MLog.i(VPS_FUNCTION_ID, SMA_VOLUME_LIST_OBTAINED);

            for (VolumeInfo vol : vols) {
                if (VolumeInfo.TYPE_PUBLIC == vol.getType()) {
                    final String volId = vol.getId();
                    MLog.i(VPS_FUNCTION_ID, SMA_UNMOUNTING_VOLUME, volId);
                    try {
                        mSm.unmount(volId);
                        MLog.i(VPS_FUNCTION_ID, SMA_VOLUME_UNMOUNTED, volId);
                    } catch (RuntimeException e) {
                        logWarning(
                                SMA_UNMOUNTING_RUNTIME_EXCEPTION, volId + "': " + e.getMessage());
                    }
                }
            }
        } catch (RemoteException e) {
            logWarning(SMA_UNMOUNTING_REMOTE_EXCEPTION, e.getMessage());
        }
    }

    @Override
    public void mountUnmounted() {
        if (mSm == null) {
            logWarning(SMA_MOUNTING_NULL_BINDER);
            return;
        }
        try {
            MLog.i(VPS_FUNCTION_ID, SMA_MOUNTING_UNMOUNTED);

            MLog.i(VPS_FUNCTION_ID, SMA_OBTAINING_VOLUME_LIST);
            final VolumeInfo[] vols = mSm.getVolumes(0);
            MLog.i(VPS_FUNCTION_ID, SMA_VOLUME_LIST_OBTAINED);

            for (VolumeInfo vol : vols) {
                if (VolumeInfo.TYPE_PUBLIC == vol.getType()) {
                    final String volId = vol.getId();
                    MLog.i(VPS_FUNCTION_ID, SMA_MOUNTING_VOLUME, volId);
                    try {
                        mSm.mount(volId);
                    } catch (RuntimeException e) {
                        logWarning(SMA_MOUNTING_RUNTIME_EXCEPTION, volId + "': " + e.getMessage());
                    }
                    MLog.i(VPS_FUNCTION_ID, SMA_VOLUME_MOUNTED, volId);
                }
            }
        } catch (RemoteException e) {
            logWarning(SMA_MOUNTING_REMOTE_EXCEPTION, e.getMessage());
        }
    }
}
