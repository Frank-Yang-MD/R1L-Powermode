/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.extsrvmanagerrunner;

import android.os.IBinder;

public interface IExtSrvMgrRunNotification {
    void notifyExtSrvMgrBound(IBinder binder);
}
