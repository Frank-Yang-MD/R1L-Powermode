/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ism;

import com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr.IBootDaemonStatusListener;

/** This interface is intended for subscribing to ISM to send Boot Daemon's notifications */
public interface IIsmSubscription {
    void subscribeIsm(IBootDaemonStatusListener bootDaemonStatusListener);
}
