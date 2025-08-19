/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr;

import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EBootDaemonStatus;
import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.EScreenProgress;

/*
 * Needs to notify ISM from VPS about Boot Daemon status.
 */
interface IBootDaemonStatusListener {
    oneway void notifyBootDaemonStatus(in EBootDaemonStatus status, in EScreenProgress screenProgress);
}
