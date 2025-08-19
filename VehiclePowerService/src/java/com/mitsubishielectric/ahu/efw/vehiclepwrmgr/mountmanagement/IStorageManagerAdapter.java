/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.mountmanagement;

/**
 * Represents interface to unmount all volumes connected by user and mount previously unmounted
 * volumes.
 */
public interface IStorageManagerAdapter {
    /** Unmount all volumes connected by user. */
    void unmountAll();

    /** Mount previously unmounted volumes. */
    void mountUnmounted();
}
